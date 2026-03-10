#include "SerialConsoleManager.h"

#include "BMSModuleManager.h"
#include "SOCManager.h"
#include "CSCAddressManager.h"
#include "SettingsManager.h"
#include "BalanceManager.h"

extern BMSModuleManager bms;
extern SOCManager soc;
extern CSCAddressManager csc;
extern SettingsManager settingsManager;
extern BalanceManager balance;

extern float currentact;



/*
================================================
Init
================================================
*/

void SerialConsoleManager::init()
{
    csvStreaming = false;
    lastCSV = 0;
    mode = MODE_MAIN;
}

void SerialConsoleManager::printPackSummary()
{

    Serial.println();
    Serial.println("===== PACK SUMMARY =====");

    Serial.print("Modules:        ");
    Serial.println(bms.getNumModules());

    Serial.print("Pack Voltage:   ");
    Serial.print(bms.getPackVoltage());
    Serial.println(" V");

    float high = bms.getHighCellVolt();
    float low  = bms.getLowCellVolt();

    Serial.print("High Cell:      ");
    Serial.print(high);
    Serial.println(" V");

    Serial.print("Low Cell:       ");
    Serial.print(low);
    Serial.println(" V");

    Serial.print("Delta:          ");
    Serial.print(high - low);
    Serial.println(" V");

    Serial.println();

    Serial.print("High Temp:      ");
    Serial.print(bms.getHighTemperature());
    Serial.println(" C");

    Serial.print("Low Temp:       ");
    Serial.print(bms.getLowTemperature());
    Serial.println(" C");

    Serial.println();

    Serial.print("Pack Current:   ");
    Serial.print(currentact);
    Serial.println(" A");

    Serial.print("SOC:            ");
    Serial.print(soc.getSOC());
    Serial.println(" %");

    Serial.println();

}
void SerialConsoleManager::printCellTable()
{

    Serial.println();
    Serial.println("===== CELL VOLTAGE TABLE =====");
    Serial.println();

    int modules = bms.getNumModules();

    if(modules == 0)
    {
        Serial.println("No modules detected");
        return;
    }

    /*
    Header row
    */

    Serial.print("Module  Bus  ID | ");

    for(int c = 0; c < 16; c++)
    {
        Serial.print("C");
        Serial.print(c);
        Serial.print("     ");
    }

    Serial.println();

    Serial.println("---------------------------------------------------------------------------------------------------------------");

    /*
    Print each module
    */

    for(int m = 0; m < modules; m++)
    {

        Serial.print(m);
        Serial.print("       ");

        Serial.print(bms.getModuleBus(m));
        Serial.print("    ");

        Serial.print(bms.getModuleID(m));
        Serial.print("  | ");

        for(int c = 0; c < 16; c++)
        {

            float v = bms.getCellVoltage(m,c);

            if(v < 2.0)
                Serial.print("----  ");
            else
            {
                Serial.print(v,3);
                Serial.print("  ");
            }

        }

        Serial.println();

    }

}
void SerialConsoleManager::printTemperatureTable()
{

    Serial.println();
    Serial.println("===== TEMPERATURE TABLE =====");
    Serial.println();

    int modules = bms.getNumModules();

    if(modules == 0)
    {
        Serial.println("No modules detected");
        return;
    }

    /*
    Header
    */

    Serial.println("Module  Bus  ID |  T0 (C)   T1 (C)");
    Serial.println("-------------------------------------");

    /*
    Print module temperatures
    */

    for(int m = 0; m < modules; m++)
    {

        Serial.print(m);
        Serial.print("       ");

        Serial.print(bms.getModuleBus(m));
        Serial.print("    ");

        Serial.print(bms.getModuleID(m));
        Serial.print("  |  ");

        float t0 = bms.getTemperature(m,0);
        float t1 = bms.getTemperature(m,1);

        if(t0 < -50 || t0 > 150)
            Serial.print("----   ");
        else
        {
            Serial.print(t0,1);
            Serial.print("    ");
        }

        if(t1 < -50 || t1 > 150)
            Serial.print("----");
        else
            Serial.print(t1,1);

        Serial.println();

    }

}
void SerialConsoleManager::printBalanceStatus()
{

    Serial.println();
    Serial.println("=== Balance Status ===");

    int modules = bms.getNumModules();

    for(int m=0; m<modules; m++)
    {

        Serial.print("Module ");
        Serial.print(m);
        Serial.print(": ");

        uint16_t mask = balance.getModuleMask(m);

        for(int c=0;c<CELLS_PER_MODULE;c++)
        {
            if(mask & (1 << c))
                Serial.print("B");
            else
                Serial.print(".");
        }

        Serial.println();

    }

}


void SerialConsoleManager::printCSVHeader()
{

    Serial.println();
    Serial.println("CSV telemetry stream enabled");
    Serial.println();

    Serial.print("time_ms,");
    Serial.print("modules,");
    Serial.print("pack_voltage,");
    Serial.print("current,");
    Serial.print("high_cell,");
    Serial.print("low_cell,");
    Serial.print("cell_delta,");
    Serial.print("high_temp,");
    Serial.print("low_temp,");
    Serial.print("soc");

    Serial.println();

}
void SerialConsoleManager::printCSVTelemetry()
{

    uint32_t now = millis();

    float packV = bms.getPackVoltage();
    float high  = bms.getHighCellVolt();
    float low   = bms.getLowCellVolt();

    Serial.print(now);
    Serial.print(",");

    Serial.print(bms.getNumModules());
    Serial.print(",");

    Serial.print(packV,3);
    Serial.print(",");

    Serial.print(currentact,2);
    Serial.print(",");

    Serial.print(high,3);
    Serial.print(",");

    Serial.print(low,3);
    Serial.print(",");

    Serial.print(high - low,3);
    Serial.print(",");

    Serial.print(bms.getHighTemperature(),1);
    Serial.print(",");

    Serial.print(bms.getLowTemperature(),1);
    Serial.print(",");

    Serial.print(soc.getSOC(),1);

    Serial.println();

}
void SerialConsoleManager::printModuleMap()
{

    Serial.println();
    Serial.println("===== MODULE MAP =====");
    Serial.println();

    int modules = bms.getNumModules();

    if(modules == 0)
    {
        Serial.println("No modules discovered");
        return;
    }

    Serial.println("Global  Bus  CSC_ID");
    Serial.println("--------------------");

    for(int m = 0; m < modules; m++)
    {

        Serial.print(m);
        Serial.print("       ");

        Serial.print(bms.getModuleBus(m));
        Serial.print("    ");

        Serial.println(bms.getModuleID(m));

    }

}

/*
================================================
Console Update
================================================
*/

void SerialConsoleManager::update()
{

    if (Serial.available())
    {

        char cmd = Serial.read();

        switch(mode)
        {

            case MODE_MAIN:

                switch(cmd)
                {

                    case 'p': printPackSummary(); break;
                    case 'c': printCellTable(); break;
                    case 't': printTemperatureTable(); break;

                    case 'v':

                        csvStreaming = !csvStreaming;

                        if(csvStreaming)
                            printCSVHeader();

                    break;

                    case 'k': printModuleMap(); break;

                    case 'z': printBalanceStatus(); break;

                    case 'b':
                        mode = MODE_BATTERY_SETTINGS;
                        printBatteryMenu();
                    break;

                    case 'e':
                        mode = MODE_CHARGER_SETTINGS;
                        printChargerMenu();
                    break;

                    case 's':
                        mode = MODE_CURRENT_SENSOR_SETTINGS;
                        printCurrentSensorMenu();
                    break;

                    case 'd': runCSCDiscovery(); break;

                    case 'l':
                        csc.lockDiscovery();
                        Serial.println("CSC discovery locked");
                    break;

                    case 'x': renumberBus(0); break;
                    case 'y': renumberBus(1); break;

                    case 'm': printMainMenu(); break;

                }

            break;



            case MODE_BATTERY_SETTINGS:
                handleBatteryMenu(cmd);
            break;



            case MODE_CHARGER_SETTINGS:
                handleChargerMenu(cmd);
            break;



            case MODE_CURRENT_SENSOR_SETTINGS:
                handleCurrentSensorMenu(cmd);
            break;

        }

    }



    if(csvStreaming)
    {

        if(millis() - lastCSV > 500)
        {

            lastCSV = millis();

            printCSVTelemetry();

        }

    }

}

/*
================================================
Main Menu
================================================
*/


void SerialConsoleManager::printMainMenu()
{
    Serial.println();
    Serial.println("===== SimpBMS Console =====");

    Serial.println();
    Serial.println("Diagnostics:");
    Serial.println("p - Pack summary");
    Serial.println("c - Cell voltage table");
    Serial.println("t - Temperature table");
    Serial.println("k - Module map");
    Serial.println("z - Balance status");

    Serial.println();
    Serial.println("Telemetry:");
    Serial.println("v - Toggle CSV stream");

    Serial.println();
    Serial.println("CSC tools:");
    Serial.println("d - Run CSC discovery");
    Serial.println("l - Lock discovery");
    Serial.println("x - Renumber bus 0");
    Serial.println("y - Renumber bus 1");

    Serial.println();
    Serial.println("Settings:");
    Serial.println("b - Battery settings");
    Serial.println("e - Charger settings");
    Serial.println("s - Current sensor settings");

    Serial.println();
}


/*
================================================
Battery Menu
================================================
*/

void SerialConsoleManager::printBatteryMenu()
{

    auto &s = settingsManager.get();

    Serial.println();
    Serial.println("===== Battery Settings =====");

    Serial.println("CELL LIMITS");

    Serial.print("v Cell Over Voltag limit: (V) ");
    Serial.println(s.cellOverVoltage);

    Serial.print("u Cell Under Voltage limit: (V) ");
    Serial.println(s.cellUnderVoltage);

    Serial.println();

    Serial.println("CHARGE TARGET");

    Serial.print("t Cell charge target: (V)");
    Serial.println(s.cellChargeTarget);

    Serial.print("y Charge hysteresis (V): ");
    Serial.print(s.cellChargeHysteresis,3);
    Serial.print(" (");
    Serial.print(s.cellChargeHysteresis * 1000,0);
    Serial.println(" mV)");

    Serial.println();

    Serial.println("BALANCING");

    Serial.print("b Balance start voltage: (V) ");
    Serial.println(s.balanceVoltage);

    Serial.print("d Balance delta: (V) ");
    Serial.println(s.balanceDelta);
    Serial.print(" (");
    Serial.print(s.balanceDelta * 1000,0);
    Serial.println(" mV)");

    Serial.println();

    Serial.println("PACK");

    Serial.print("c Pack capacity Ah: ");
    Serial.println(s.packCapacityAh);

    Serial.print("s Series cells: ");
    Serial.println(s.seriesCells);

    Serial.print("p Parallel strings: ");
    Serial.println(s.parallelStrings);

    Serial.print("x Max discharge current:(A) ");
    Serial.println(s.maxDischargeCurrent);

    Serial.println();

    Serial.println("FUEL GAUGE");

    Serial.print("g Gauge low PWM: ");
    Serial.println(s.gaugelow);

    Serial.print("m Gauge mid PWM: ");
    Serial.println(s.gaugemid);

    Serial.print("h Gauge high PWM: ");
    Serial.println(s.gaugehigh);

    Serial.print("i Gauge invert: ");
    Serial.println(s.gaugeinvert);

    Serial.println();
    Serial.println("q Return");

}



/*
================================================
Battery Menu Handler
================================================
*/

void SerialConsoleManager::handleBatteryMenu(char cmd)
{

    auto &s = settingsManager.get();

    switch(cmd)
    {

        case 'v':
            Serial.println("Enter cell OV limit:");
            while(!Serial.available());
            s.cellOverVoltage = Serial.parseFloat();
        break;

        case 'u':
            Serial.println("Enter cell UV limit:");
            while(!Serial.available());
            s.cellUnderVoltage = Serial.parseFloat();
        break;

        case 't':
            Serial.println("Enter charge target voltage:");
            while(!Serial.available());
            s.cellChargeTarget = Serial.parseFloat();
        break;

        case 'y':
            Serial.println("Enter charge hysteresis:");
            while(!Serial.available());
            s.cellChargeHysteresis = Serial.parseFloat();
        break;

        case 'b':
            Serial.println("Enter balance start voltage:");
            while(!Serial.available());
            s.balanceVoltage = Serial.parseFloat();
        break;

        case 'd':
            Serial.println("Enter balance delta:");
            while(!Serial.available());
            s.balanceDelta = Serial.parseFloat();
        break;

        case 'c':
            Serial.println("Enter pack capacity Ah:");
            while(!Serial.available());
            s.packCapacityAh = Serial.parseFloat();
        break;

        case 's':
            Serial.println("Enter series cells:");
            while(!Serial.available());
            s.seriesCells = Serial.parseInt();
        break;

        case 'p':
            Serial.println("Enter parallel strings:");
            while(!Serial.available());
            s.parallelStrings = Serial.parseInt();
        break;

        case 'x':
            Serial.println("Enter max discharge current:");
            while(!Serial.available());
            s.maxDischargeCurrent = Serial.parseFloat();
        break;

        case 'g':
            Serial.println("Enter PWM for 0% SOC:");
            while(!Serial.available());
            s.gaugelow = constrain(Serial.parseInt(),0,255);
        break;

        case 'm':
            Serial.println("Enter PWM for 50% SOC:");
            while(!Serial.available());
            s.gaugemid = constrain(Serial.parseInt(),0,255);
        break;

        case 'h':
            Serial.println("Enter PWM for 100% SOC:");
            while(!Serial.available());
            s.gaugehigh = constrain(Serial.parseInt(),0,255);
        break;

        case 'i':
            s.gaugeinvert = !s.gaugeinvert;
        break;

        case 'q':
            mode = MODE_MAIN;
            printMainMenu();
            return;

    }

    settingsManager.save();
    printBatteryMenu();

}



/*
================================================
Charger Menu
================================================
*/

void SerialConsoleManager::printChargerMenu()
{

    auto &s = settingsManager.get();

    Serial.println();
    Serial.println("===== Charger Settings =====");

    Serial.print("v Max charge current: (A)");
    Serial.println(s.maxChargeCurrent);

    Serial.print("e End charge current: (A)");
    Serial.println(s.endChargeCurrent);

    Serial.print("t Charger type: ");

    switch(s.chargerType)
    {
        case CHARGER_RELAY:
            Serial.println("Relay");
            break;

        case CHARGER_OUTLANDER:
            Serial.println("Outlander");
            break;

        case CHARGER_MG:
            Serial.println("MG EV");
            break;


    }
    Serial.print("d Direct HV control: ");
     Serial.println(s.chargerDirectHV);
    Serial.print("c Charger CAN period: ");
    Serial.println(s.chargerCanPeriod);

    Serial.println("q Return");

}



/*
================================================
Charger Menu Handler
================================================
*/

void SerialConsoleManager::handleChargerMenu(char cmd)
{

    auto &s = settingsManager.get();

    switch(cmd)
    {

        case 'v':
            Serial.println("Enter max charge current:");
            while(!Serial.available());
            s.maxChargeCurrent = Serial.parseFloat();
        break;

        case 'e':
            Serial.println("Enter end charge current:");
            while(!Serial.available());
            s.endChargeCurrent = Serial.parseFloat();
        break;

    case 't':
    {
        Serial.println();
        Serial.println("Select charger type:");
        Serial.println("0 - Relay");
        Serial.println("1 - Outlander");
        Serial.println("2 - MG EV");

        while(!Serial.available());

        int type = Serial.parseInt();

        if(type >= 0 && type <= 2)
        {
            s.chargerType = type;
            Serial.print("Charger set to ");
            Serial.println(type);
        }
        else
        {
            Serial.println("Invalid charger type");
        }
    }
    break;

        case 'c':
            Serial.println("Enter charger CAN period:");
            while(!Serial.available());
            s.chargerCanPeriod = Serial.parseInt();
        break;

        case 'd':
           s.chargerDirectHV = !s.chargerDirectHV;
        break;

        case 'q':
            mode = MODE_MAIN;
            printMainMenu();
            return;

    }
    settingsManager.save();
    printChargerMenu();


}



/*
================================================
Current Sensor Menu
================================================
*/

void SerialConsoleManager::printCurrentSensorMenu()
{

    auto &s = settingsManager.get();

    Serial.println();
    Serial.println("===== Current Sensor Settings =====");

    Serial.print("t Sensor type: ");

    switch(s.currentSensorType)
    {
        case SENSOR_NONE:
            Serial.println("None");
        break;

        case SENSOR_LEM_CAB500:
            Serial.println("LEM CAB500");
        break;

        case SENSOR_SHUNT:
            Serial.println("Analog Shunt");
        break;

        case SENSOR_CAN:
            Serial.println("CAN Sensor");
        break;

        default:
            Serial.println("Unknown");
        break;
    }

    Serial.print("i Invert current (0/1): ");
    Serial.println(s.currentInvert ? 1 : 0);

    Serial.print("m Multiplier (A/LSB): ");
    Serial.println(s.currentMultiplier);

    Serial.print("d Deadband (A): ");
    Serial.println(s.currentDeadband);

    Serial.print("c CAN ID: ");
    Serial.println(s.currentCanID);

    Serial.println("q Return");

}



/*
================================================
Current Sensor Menu Handler
================================================
*/

void SerialConsoleManager::handleCurrentSensorMenu(char cmd)
{

    auto &s = settingsManager.get();

    switch(cmd)
    {

    case 't':
    {
        Serial.println();
        Serial.println("Select current sensor:");
        Serial.println("0 - None");
        Serial.println("1 - LEM CAB500");
        Serial.println("2 - Analog shunt");
        Serial.println("3 - CAN sensor");

        while(!Serial.available());

        int type = Serial.parseInt();

        if(type >= 0 && type <= 3)
        {
            s.currentSensorType = type;

            Serial.print("Sensor set to ");
            Serial.println(type);
        }
        else
        {
            Serial.println("Invalid sensor type");
        }
    }
    break;

    case 'i':
        s.currentInvert = !s.currentInvert;
    break;

        case 'm':
            Serial.println("Enter multiplier:");
            while(!Serial.available());
            s.currentMultiplier = Serial.parseFloat();
        break;

        case 'd':
            Serial.println("Enter deadband:");
            while(!Serial.available());
            s.currentDeadband = Serial.parseFloat();
        break;

        case 'c':
            Serial.println("Enter CAN ID:");
            while(!Serial.available());
            s.currentCanID = Serial.parseInt();
        break;

        case 'q':
            mode = MODE_MAIN;
            printMainMenu();
            return;

    }

    settingsManager.save();
    printCurrentSensorMenu();

}



/*
================================================
CSC Tools
================================================
*/

void SerialConsoleManager::runCSCDiscovery()
{

    Serial.println("Running CSC discovery on both CAN buses");

    renumberBus(0);
    renumberBus(1);

}



void SerialConsoleManager::renumberBus(uint8_t bus)
{

    Serial.print("Renumbering CSCs on bus ");
    Serial.println(bus);

    csc.renumberBus(bus);

}
