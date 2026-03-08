#pragma once

#include <Arduino.h>

class BMSStateMachine
{

public:

    enum State
    {
        BMS_BOOT,
        BMS_READY,
        BMS_PRECHARGE,
        BMS_DRIVE,
        BMS_ERROR
    };

    BMSStateMachine();

    void update();

    State getState();

private:

    State state;

    bool ignitionLatch;

};