#pragma once

#include <Arduino.h>
#include <stdint.h>
#include "config.h"

class SOCManager
{

public:

    void init(float capacityAh);

    void update(float current);

    float getSOC();

    void setSOC(float soc);

    void checkFullChargeReset(float highCell, float current);

private:

    float packCapacity;
    float soc;

    uint32_t lastUpdate;

};