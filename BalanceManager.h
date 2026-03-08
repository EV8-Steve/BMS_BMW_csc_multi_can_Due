#pragma once

#include <Arduino.h>

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

    bool balancingActive;
    bool dutyState;

};