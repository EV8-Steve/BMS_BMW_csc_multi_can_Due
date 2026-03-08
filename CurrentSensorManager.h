#pragma once

#include <Arduino.h>
#include <due_can.h>
#include "BMSSettings.h"
#include "SettingsManager.h"

class CurrentSensorManager
{

public:

    void init();
    void update();

    void processCAN(CAN_FRAME &frame);

    float getCurrent();

private:

    float currentAct;

    void processCAB300(CAN_FRAME &frame);
    void processCAB500(CAN_FRAME &frame);
    void processISA(CAN_FRAME &frame);

    void processAnalog();

};