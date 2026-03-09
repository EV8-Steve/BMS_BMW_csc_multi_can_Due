#include "ProtectionManager.h"
#include "BMSModuleManager.h"

extern BMSModuleManager bms;


void ProtectionManager::init()
{
    allowCharge = true;
    allowDischarge = true;
    moduleFault = false;
}



void ProtectionManager::update()
{

    checkVoltages();

    checkTemperature();

    if(!bms.modulesHealthy())
    {
        moduleFault = true;

        Serial.println("Module communication lost");
    }

}




void ProtectionManager::checkVoltages()
{

    float highCell = bms.getHighCellVolt();
    float lowCell  = bms.getLowCellVolt();

    if(highCell > OVER_VOLTAGE_LIMIT)
    {
        allowCharge = false;
        Serial.println("Charge disabled: Overvoltage");
    }

    if(highCell < (OVER_VOLTAGE_LIMIT - 0.05))
    {
        allowCharge = true;
    }

    if(lowCell < UNDER_VOLTAGE_LIMIT)
    {
        allowDischarge = false;
        Serial.println("Discharge disabled: Undervoltage");
    }

    if(lowCell > (UNDER_VOLTAGE_LIMIT + 0.05))
    {
        allowDischarge = true;
    }

}



void ProtectionManager::checkTemperature()
{

    float highTemp = bms.getHighTemperature();
    float lowTemp = bms.getLowTemperature();


    if(highTemp > OVER_TEMP_LIMIT)
    {

        allowCharge = false;
        allowDischarge = false;

        Serial.println("Pack over temperature");

    }



    if(lowTemp < UNDER_TEMP_LIMIT)
    {

        allowCharge = false;

        Serial.println("Charging disabled: Low temperature");

    }

}



bool ProtectionManager::chargeAllowed()
{
    return allowCharge;
}



bool ProtectionManager::dischargeAllowed()
{
    return allowDischarge;
}

bool ProtectionManager::moduleFaultActive()
{
    return moduleFault;
}
