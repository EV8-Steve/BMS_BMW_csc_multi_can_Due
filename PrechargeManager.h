#pragma once

#include <arduino.h>

class PrechargeManager
{

public:

    void init();
    void update(float packVoltage);

    bool ready();

private:

    bool prechargeComplete;
    uint32_t startTime;

};