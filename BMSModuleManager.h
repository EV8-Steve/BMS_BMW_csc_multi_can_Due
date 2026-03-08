#pragma once

#include <Arduino.h>
#include <due_can.h>

#include "config.h"
#include "BMSModule.h"

class BMSModuleManager
{

public:

    BMSModuleManager();

    void decodecan(uint8_t bus, CAN_FRAME &frame, int debug);

    void sendKeepAlive();

    void updateWatchdogs();

    void updatePackStatistics();

    bool modulesHealthy();

    int getNumModules();

    float getPackVoltage();

    float getHighCellVolt();

    float getLowCellVolt();

    float getHighTemperature();

    float getLowTemperature();

    float getAvgTemperature();

    float getCellVoltage(int module,int cell);

    float getTemperature(int module,int sensor);

    void sendBalanceMask(int module,uint16_t mask);

    /*
    Discovery / Renumber Support
    */

    void clearBus(uint8_t bus);
    void clearAllModules();

    /*
    Diagnostic helpers
    */

    uint8_t getModuleBus(int module);
    uint8_t getModuleID(int module);
    bool moduleAlive(int module);

private:

    int findModule(uint8_t bus,uint8_t id);

    int registerModule(uint8_t bus,uint8_t id);

    BMSModule modules[MAX_MODULES];

    uint8_t moduleBus[MAX_MODULES];
    uint8_t moduleID[MAX_MODULES];

    int numModules;

    float packVoltage;

    float highCell;
    float lowCell;

    float highTemp;
    float lowTemp;

};