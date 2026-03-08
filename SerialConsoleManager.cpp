#include "SerialConsoleManager.h"

#include "BMSModuleManager.h"
#include "SOCManager.h"
#include "CSCAddressManager.h"
#include "SettingsManager.h"

extern BMSModuleManager bms;
extern SOCManager soc;
extern CSCAddressManager csc;
extern SettingsManager settings;

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
Battery Menu
================================================
*/

void SerialConsoleManager::printBatteryMenu()
{

    auto &s = settings.get();

    Serial.println();
    Serial.println("===== Battery Settings =====");

    Serial.println("CELL LIMITS");

    Serial.print("v Cell OV limit: ");
    Serial.println(s.cellOverVoltage);

    Serial.print("u Cell UV limit: ");
    Serial.println(s.cellUnderVoltage);

    Serial.println();

    Serial.println("CHARGE TARGET");

    Serial.print("t Cell charge target: ");
    Serial.println(s.cellChargeTarget);

    Serial.print("y Charge hysteresis: ");
    Serial.println(s.cellChargeHysteresis);

    Serial.println();

    Serial.println("BALANCING");

    Serial.print("b Balance voltage: ");
    Serial.println(s.balanceVoltage);

    Serial.print("d Balance delta: ");
    Serial.println(s.balanceDelta);

    Serial.println();

    Serial.println("PACK");

    Serial.print("c Pack capacity Ah: ");
    Serial.println(s.packCapacityAh);

    Serial.print("s Series cells: ");
    Serial.println(s.seriesCells);

    Serial.print("p Parallel strings: ");
    Serial.println(s.parallelStrings);

    Serial.print("x Max discharge current: ");
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

    auto &s = settings.get();

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

    settings.save();
    printBatteryMenu();

}



/*
================================================
Charger Menu
================================================
*/

void SerialConsoleManager::printChargerMenu()
{

    auto &s = settings.get();

    Serial.println();
    Serial.println("===== Charger Settings =====");

    Serial.print("v Max charge current: ");
    Serial.println(s.maxChargeCurrent);

    Serial.print("e End charge current: ");
    Serial.println(s.endChargeCurrent);

    Serial.print("t Charger type: ");
    Serial.println(s.chargerType);

    Serial.print("c Charger CAN period: ");
    Serial.println(s.chargerCanPeriod);

    Serial.print("d Direct HV control: ");
    Serial.println(s.chargerDirectHV);

    Serial.println("q Return");

}



/*
================================================
Charger Menu Handler
================================================
*/

void SerialConsoleManager::handleChargerMenu(char cmd)
{

    auto &s = settings.get();

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
            Serial.println("Enter charger type number:");
            while(!Serial.available());
            s.chargerType = Serial.parseInt();
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

    settings.save();
    printChargerMenu();

}



/*
================================================
Current Sensor Menu
================================================
*/

void SerialConsoleManager::printCurrentSensorMenu()
{

    auto &s = settings.get();

    Serial.println();
    Serial.println("===== Current Sensor Settings =====");

    Serial.print("t Sensor type: ");
    Serial.println(s.currentSensorType);

    Serial.print("i Invert current: ");
    Serial.println(s.currentInvert);

    Serial.print("m Multiplier: ");
    Serial.println(s.currentMultiplier);

    Serial.print("d Deadband: ");
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

    auto &s = settings.get();

    switch(cmd)
    {

        case 't':
            Serial.println("Enter sensor type:");
            while(!Serial.available());
            s.currentSensorType = Serial.parseInt();
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

    settings.save();
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
