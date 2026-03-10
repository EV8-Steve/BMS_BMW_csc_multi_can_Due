#include "BMSModule.h"
#include "config.h"

BMSModule::BMSModule()
{

    exists = false;
    lastMessage = 0;

    moduleVolt = 0;

    for(int i=0;i<16;i++)
        cellVolt[i] = 0;

    for(int i=0;i<2;i++)
        temperature[i] = 0;
    
    
}



/*
================================================
Voltage frame decode
================================================
*/

void BMSModule::decodecan(uint32_t id, CAN_FRAME &msg, bool debug)
{

    lastMessage = millis();

    exists = true;

    int group = (id >> 4) & 0x07;


    int cellBase = group * 2;

    for(int i=0;i<2;i++)
    {

        uint16_t raw = msg.data.bytes[i*2] + ((msg.data.bytes[i*2+1] & 0x3F) << 8);

        int cell = cellBase + i;

        if(cell < 16)
        {
            cellVolt[cell] = raw * 0.001;
        }

    }

}



/*
================================================
Temperature frame decode
================================================
*/

void BMSModule::decodetemp(CAN_FRAME &msg, int variant)
{

    lastMessage = millis();

    temperature[0] = msg.data.bytes[0] - 40;
    temperature[1] = msg.data.bytes[1] - 40;

}



/*
================================================
Alive tracking
================================================
*/

void BMSModule::markAlive()
{
    lastMessage = millis();
}



bool BMSModule::alive()
{
    return (millis() - lastMessage) < MODULE_TIMEOUT;
}



/*
================================================
Existence flag
================================================
*/

void BMSModule::setExists(bool ex)
{
    exists = ex;
}



bool BMSModule::isExisting()
{
    return exists;
}



/*
================================================
Cell voltage access
================================================
*/

float BMSModule::getCellVoltage(int cell)
{
    if(cell < 0 || cell >= 16)
        return 0;

    return cellVolt[cell];
}


/*
================================================
High/low cell
================================================
*/

float BMSModule::getHighCellV()
{

    float v = 0;

    for(int i=0;i<16;i++)
        if(cellVolt[i] > 2.0 && cellVolt[i] > v)
            v = cellVolt[i];

    return v;

}



float BMSModule::getLowCellV()
{

    float v = 5;
    
    for(int i=0;i<16;i++)
        if(cellVolt[i] > 2.0 && cellVolt[i] < v)
            v = cellVolt[i];

    return v;

}



/*
================================================
Temperature access
================================================
*/
float BMSModule::getTemperature(int sensor)
{
    if(sensor < 0 || sensor >= 2)
        return 0;

    return temperature[sensor];
}


/*
================================================
Module voltage
================================================
*/

float BMSModule::getModuleVoltage()
{

    float v = 0;

    for(int i=0;i<16;i++)
        if(cellVolt[i] > 2.0)
            v += cellVolt[i];

    return v;

}

/*
================================================
Clear Module voltages
================================================
*/

void BMSModule::clearVoltages()
{
    for(int i = 0; i < 16; i++)
        cellVolt[i] = 0;

    for(int i = 0; i < 2; i++)
        temperature[i] = 0;

    moduleVolt = 0;
}
