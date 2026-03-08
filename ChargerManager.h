#pragma once

#include "config.h"
#include "SOCManager.h"
#include "ProtectionManager.h"

class ChargerManager
{

public:

    void init();

    void update(float packVoltage, float highCell);

    bool chargerEnabled();

    float getChargeVoltage();
    float getChargeCurrent();

private:

    bool chargerEnable;

    float chargeVoltage;
    float chargeCurrent;

};