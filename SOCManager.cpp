#include "SOCManager.h"
#include <Arduino.h>


void SOCManager::init(float capacityAh)
{

    packCapacity = capacityAh;

    soc = 100.0;

    lastUpdate = millis();

}



void SOCManager::update(float current)
{

    uint32_t now = millis();

    float dt = (now - lastUpdate) / 3600000.0;  // hours

    lastUpdate = now;


    float deltaAh = current * dt;

    float deltaSOC = (deltaAh / packCapacity) * 100.0;

    soc -= deltaSOC;


    if (soc > 100) soc = 100;

    if (soc < 0) soc = 0;

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