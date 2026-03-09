#include "PrechargeManager.h"
#include "IOManager.h"

extern IOManager io;



void PrechargeManager::init()
{
    prechargeComplete = false;
    io.setInverterEnable(false);
    startTime = 0;
}


void PrechargeManager::update(float packVoltage)
{

    if(prechargeComplete)
        return;

    if(startTime==0)
    {
        startTime=millis();
        io.setPrecharge(true);
    }

    if(millis()-startTime>3000)
    {
        io.setMainContactor(true);
        io.setPrecharge(false);
        prechargeComplete=true;
    }

}



bool PrechargeManager::ready()
{
    return prechargeComplete;
}
