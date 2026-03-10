#include "SOCManager.h"
#include <Arduino.h>
#include "SettingsManager.h"

extern SettingsManager settingsManager;

void SOCManager::init(float capacityAh)
{

    packCapacity = capacityAh;

    soc = 100.0;

    lastUpdate = millis();

}



void SOCManager::update(float current)
{

    if(packCapacity <= 0)
        return;

    uint32_t now = millis();

    float dt = (now - lastUpdate) / 3600000.0;

    lastUpdate = now;

    if(dt > 0.01)
        dt = 0.01;


    float deltaAh = current * dt;

    float deltaSOC = (deltaAh / packCapacity) * 100.0;

    soc -= deltaSOC;


    if (soc > 100) soc = 100;

    if (soc < 0) soc = 0;

}

void SOCManager::checkFullChargeReset(float highCell, float current)
{

    auto &settings = settingsManager.get();

    if(highCell >= settings.cellChargeTarget &&
       fabs(current) <= settings.endChargeCurrent)
    {

        if(soc < 99.5)
        {
            soc = 100.0;

            Serial.println("SOC reset to 100% (full charge detected)");
        }

    }

}


float SOCManager::getSOC()
{
    return soc;
}



void SOCManager::setSOC(float newSOC)
{

    soc = newSOC;

    if (soc > 100) soc = 100;

    if (soc < 0) soc = 0;

}
