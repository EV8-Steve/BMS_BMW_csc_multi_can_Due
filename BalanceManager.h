#pragma once

#include <Arduino.h>
#include "config.h"

class BalanceManager
{

public:

    BalanceManager();

    void init();
    void update();

    bool isBalancing();

private:

    void evaluateCells();
    void applyBalancing();
    void disableBalancing();

    uint32_t lastBalanceTime;
    uint32_t moduleCooldown[MAX_MODULES];

    bool balancingActive;
    bool dutyState;

};