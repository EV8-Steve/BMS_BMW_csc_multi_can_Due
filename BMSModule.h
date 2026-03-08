#pragma once

#include <Arduino.h>
#include <due_can.h>

class BMSModule
{

public:

    BMSModule();

    void decodecan(uint32_t id, CAN_FRAME &msg, bool debug);
    void decodetemp(CAN_FRAME &msg, int variant);

    void markAlive();
    bool alive();

    void setExists(bool ex);
    bool isExisting();

    float getCellVoltage(int cell);
    float getHighCellV();
    float getLowCellV();

    float getTemperature(int sensor);

    float getModuleVoltage();

	void clearVoltages();
private:

    float cellVolt[16];
    float temperature[2];

    float moduleVolt;

    bool exists;

    uint32_t lastMessage;

};