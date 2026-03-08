#include "ChargerManager.h"
#include "SettingsManager.h"
#include "ProtectionManager.h"

extern SettingsManager settingsManager;
extern ProtectionManager protection;



void ChargerManager::update(float packVoltage, float highCell)
{

    BMSSettings &settings = settingsManager.get();

    if(!protection.chargeAllowed())
    {
        chargerEnable = false;
        return;
    }

    chargerEnable = true;

    chargeVoltage = settings.cellChargeTarget;

    chargeCurrent = settings.maxChargeCurrent;

    if(highCell >= settings.cellChargeTarget)
    {
        chargeCurrent = settings.endChargeCurrent;
    }

}
