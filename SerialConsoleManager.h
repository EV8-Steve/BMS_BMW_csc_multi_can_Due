#pragma once

#include <Arduino.h>

#include "config.h"
#include "SettingsManager.h"

class SerialConsoleManager
{

public:

    void init();

    void update();

    void printMainMenu();
    
    void printBalanceStatus();



private:

    /*
    ========================================================
    Console Modes
    ========================================================
    */

    enum ConsoleMode
    {
        MODE_MAIN,
        MODE_BATTERY_SETTINGS,
        MODE_CHARGER_SETTINGS,
        MODE_CURRENT_SENSOR_SETTINGS
    };

    ConsoleMode mode;



    /*
    ========================================================
    CSV Telemetry
    ========================================================
    */

    bool csvStreaming;

    uint32_t lastCSV;



    /*
    ========================================================
    Telemetry
    ========================================================
    */

    void printPackSummary();

    void printCellTable();

    void printTemperatureTable();

    void printCSVHeader();

    void printCSVTelemetry();

    void printModuleMap();   // <-- FIX



    /*
    ========================================================
    Menus
    ========================================================
    */

    void printBatteryMenu();

    void printChargerMenu();

    void printCurrentSensorMenu();



    /*
    ========================================================
    Menu Handlers
    ========================================================
    */

    void handleBatteryMenu(char cmd);

    void handleChargerMenu(char cmd);

    void handleCurrentSensorMenu(char cmd);



    /*
    ========================================================
    CSC Service Tools
    ========================================================
    */

    void runCSCDiscovery();

    void renumberBus(uint8_t bus);

};