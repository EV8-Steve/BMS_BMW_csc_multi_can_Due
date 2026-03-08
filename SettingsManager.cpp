#include "SettingsManager.h"
#include <DueFlashStorage.h>

SettingsManager settingsManager;

DueFlashStorage flash;




/*
================================================
Load Settings From Flash
================================================
*/

void SettingsManager::load()
{

    byte* ptr = (byte*)&settings    ;

    for(unsigned int i=0;i<sizeof(BMSSettings);i++)
    {
        ptr[i] = flash.read(i);
    }

    if(settings.packCapacityAh <= 0)
    {
        factoryDefaults();
    }

}



/*
================================================
Save Settings To Flash
================================================
*/

void SettingsManager::save()
{

    byte* ptr = (byte*)&settings;

    for(unsigned int i=0;i<sizeof(BMSSettings);i++)
    {
        flash.write(i,ptr[i]);
    }

}



/*
================================================
Factory Defaults
================================================
*/

void SettingsManager::factoryDefaults()
{

    settings.cellOverVoltage = 4.20;
    settings.cellUnderVoltage = 3.00;

    settings.cellChargeTarget = 4.10;
    settings.cellChargeHysteresis = 0.02;

    settings.balanceVoltage = 4.10;
    settings.balanceDelta = 0.01;

    settings.packCapacityAh = 200;

    settings.seriesCells = 96;
    settings.parallelStrings = 1;

    settings.maxChargeCurrent = 150;
    settings.endChargeCurrent = 10;

    settings.maxDischargeCurrent = 400;

    settings.overTemp = 60;
    settings.underTemp = -20;

    settings.chargerType = CHARGER_OUTLANDER;
    settings.chargerCanPeriod = 100;
    settings.chargerDirectHV = false;

    settings.currentSensorType = CURRENT_SENSOR_LEM_CAB500;
    settings.currentInvert = false;

    settings.currentMultiplier = 1.0;
    settings.currentDeadband = 0.2;

    settings.currentCanID = 0x3C1;

    settings.gaugelow = 100;
    settings.gaugemid = 170;
    settings.gaugehigh = 220;
    settings.gaugeinvert = false;

}



/*
================================================
Access Settings
================================================
*/

BMSSettings& SettingsManager::get()
{
    return settings;
}
