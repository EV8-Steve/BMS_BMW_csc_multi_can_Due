#include "ProtectionManager.h"
#include "BMSModuleManager.h"

extern BMSModuleManager bms;



void ProtectionManager::init()
{

    allowCharge = true;
    allowDischarge = true;

}



void ProtectionManager::update()
{

    checkVoltages();

    checkTemperature();

}



void ProtectionManager::checkVoltages()
{

    float highCell = bms.getHighCellVolt();
    float lowCell = bms.getLowCellVolt();


    if(highCell > OVER_VOLTAGE_LIMIT)
    {

        allowCharge = false;

        Serial.println("Charge disabled: Overvoltage");

    }
    else
    {

        allowCharge = true;

    }



    if(lowCell < UNDER_VOLTAGE_LIMIT)
    {

        allowDischarge = false;

        Serial.println("Discharge disabled: Undervoltage");

    }
    else
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
