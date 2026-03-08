#include "BMSModuleManager.h"
#include "CSCAddressManager.h"

extern CSCAddressManager csc;

extern CANRaw Can0;
extern CANRaw Can1;



BMSModuleManager::BMSModuleManager()
{

    numModules = 0;

    packVoltage = 0;
    highCell = 0;
    lowCell = 5;

    highTemp = -100;
    lowTemp = 200;

}



/*
================================================
Find existing module by bus + id
================================================
*/

int BMSModuleManager::findModule(uint8_t bus,uint8_t id)
{

    for(int i=0;i<numModules;i++)
    {

        if(moduleBus[i] == bus && moduleID[i] == id)
            return i;

    }

    return -1;

}



/*
================================================
Register new module
================================================
*/

int BMSModuleManager::registerModule(uint8_t bus,uint8_t id)
{

    int existing = findModule(bus,id);

    if(existing >= 0)
        return existing;

    if(numModules >= MAX_MODULES)
        return -1;

    moduleBus[numModules] = bus;
    moduleID[numModules] = id;

    modules[numModules].setExists(true);

    numModules++;
    if(numModules > MAX_MODULES)
        numModules = MAX_MODULES;

    Serial.print("CSC discovered: Bus ");
    Serial.print(bus);
    Serial.print(" ID ");
    Serial.print(id);
    Serial.print(" → Global ");
    Serial.println(numModules-1);

    return numModules-1;

}



/*
================================================
Decode incoming CAN
================================================
*/

void BMSModuleManager::decodecan(uint8_t bus, CAN_FRAME &frame, int debug)
{

    if(frame.id == 0xF0)
    {
        csc.handleUnassigned(bus, frame);
        return;
    }



    /*
    --------------------------------
    Voltage frames
    --------------------------------
    */

    if(frame.id > 0x99 && frame.id < 0x180)
    {

        uint8_t id = frame.id & 0x0F;

        int module = findModule(bus,id);

        if(module < 0)
            module = registerModule(bus,id);

        if(module < 0)
            return;

        modules[module].markAlive();

        modules[module].decodecan(frame.id,frame,debug);

        return;

    }



    /*
    --------------------------------
    Temperature frames
    --------------------------------
    */

    if((frame.id & 0xFF0) == 0x180)
    {

        uint8_t id = frame.id & 0x0F;

        int module = findModule(bus,id);

        if(module < 0)
            module = registerModule(bus,id);

        if(module < 0)
            return;

        modules[module].markAlive();
        modules[module].decodetemp(frame,0);

    }

}



/*
================================================
Keep modules awake
================================================
*/

void BMSModuleManager::sendKeepAlive()
{

    CAN_FRAME frame;

    frame.id = 0x0A0;
    frame.length = 8;

    frame.data.bytes[0] = 0x03;

    for(int i=1;i<8;i++)
        frame.data.bytes[i] = 0xFF;

    Can0.sendFrame(frame);
    Can1.sendFrame(frame);

}



/*
================================================
Module watchdogs
================================================
*/

void BMSModuleManager::updateWatchdogs()
{

    for(int i=0;i<numModules;i++)
    {

        if(!modules[i].alive())
        {

            Serial.print("CSC timeout module ");
            Serial.println(i);


            modules[i].setExists(false);
            modules[i].clearVoltages();

        }

    }

}



/*
================================================
Pack statistics
================================================
*/

void BMSModuleManager::updatePackStatistics()
{

    packVoltage = 0;

    highCell = 0;
    lowCell = 5;

    highTemp = -100;
    lowTemp = 200;

    for(int i=0;i<numModules;i++)
    {

        if(!modules[i].isExisting())
            continue;

        packVoltage += modules[i].getModuleVoltage();

        float hc = modules[i].getHighCellV();
        float lc = modules[i].getLowCellV();

        if(hc > highCell) highCell = hc;
        if(lc < lowCell) lowCell = lc;

        float ht = modules[i].getTemperature(0);
        float lt = modules[i].getTemperature(1);

        if(ht > highTemp) highTemp = ht;
        if(lt < lowTemp) lowTemp = lt;

    }

}



/*
================================================
Balancing support
================================================
*/

float BMSModuleManager::getCellVoltage(int module,int cell)
{
    return modules[module].getCellVoltage(cell);
}



void BMSModuleManager::sendBalanceMask(int module,uint16_t mask)
{

    if(module >= numModules)
        return;

    uint8_t bus = moduleBus[module];
    uint8_t id = moduleID[module];

    CAN_FRAME frame;

    frame.id = 0x0A0 + id;
    frame.length = 8;

    frame.data.bytes[0] = 0x10;

    frame.data.bytes[1] = mask >> 8;
    frame.data.bytes[2] = mask & 0xFF;

    for(int i=3;i<8;i++)
        frame.data.bytes[i] = 0;

    if(bus == 0)
        Can0.sendFrame(frame);
    else
        Can1.sendFrame(frame);

}



/*
================================================
Getters
================================================
*/

int BMSModuleManager::getNumModules()
{
    return numModules;
}



float BMSModuleManager::getPackVoltage()
{
    return packVoltage;
}



float BMSModuleManager::getHighCellVolt()
{
    return highCell;
}



float BMSModuleManager::getLowCellVolt()
{
    return lowCell;
}



float BMSModuleManager::getHighTemperature()
{
    return highTemp;
}



float BMSModuleManager::getLowTemperature()
{
    return lowTemp;
}



float BMSModuleManager::getAvgTemperature()
{

    float total = 0;
    int count = 0;

    for(int i=0;i<numModules;i++)
    {
        if(!modules[i].isExisting())
            continue;

        total += modules[i].getTemperature(0);
        total += modules[i].getTemperature(1);

        count += 2;
    }

    if(count == 0)
        return 0;

    return total / count;

}
/*
================================================
Temperature access
================================================
*/

float BMSModuleManager::getTemperature(int module,int sensor)
{

    if(module >= numModules)
        return 0;

    return modules[module].getTemperature(sensor);

}


/*
================================================
Module diagnostic helpers
================================================
*/

uint8_t BMSModuleManager::getModuleBus(int module)
{

    if(module >= numModules)
        return 0;

    return moduleBus[module];

}



uint8_t BMSModuleManager::getModuleID(int module)
{

    if(module >= numModules)
        return 0;

    return moduleID[module];

}



bool BMSModuleManager::moduleAlive(int module)
{

    if(module >= numModules)
        return false;

    return modules[module].alive();

}


/*
================================================
Health check
================================================
*/

bool BMSModuleManager::modulesHealthy()
{

    for(int i=0;i<numModules;i++)
    {

        if(!modules[i].alive())
            return false;

    }

    return true;

}


/*
================================================
Clear modules on one CAN bus
================================================
*/

void BMSModuleManager::clearBus(uint8_t bus)
{

    int newCount = 0;

    for(int i = 0; i < numModules; i++)
    {

        if(moduleBus[i] != bus)
        {

            if(newCount != i)
            {
                modules[newCount] = modules[i];
                moduleBus[newCount] = moduleBus[i];
                moduleID[newCount] = moduleID[i];
            }

            newCount++;

        }

    }

    numModules = newCount;

    Serial.print("Modules remaining after clearing bus ");
    Serial.print(bus);
    Serial.print(": ");
    Serial.println(numModules);

}



/*
================================================
Clear ALL modules
================================================
*/

void BMSModuleManager::clearAllModules()
{

    numModules = 0;

    for(int i = 0; i < MAX_MODULES; i++)
    {

        moduleBus[i] = 0;
        moduleID[i] = 0;

        modules[i].setExists(false);
        modules[i].clearVoltages();

    }

    packVoltage = 0;

    highCell = 0;
    lowCell = 5;

    highTemp = -100;
    lowTemp = 200;

    Serial.println("All module records cleared");

}
