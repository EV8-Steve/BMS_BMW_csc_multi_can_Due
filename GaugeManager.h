#pragma once

#include <Arduino.h>
#include "SettingsManager.h"

class GaugeManager
{

public:

    void init();

    void update(float soc);

private:

    float filteredSOC;

    uint8_t computePWM(float soc);

};