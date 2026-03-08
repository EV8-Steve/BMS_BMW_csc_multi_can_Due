#include "BalanceManager.h"
#include "BMSModuleManager.h"
#include "config.h"

#include <math.h>

extern BMSModuleManager bms;
extern float currentact;



BalanceManager::BalanceManager()
{
    lastBalanceTime = 0;
    balancingActive = false;
    dutyState = false;
}



/*
================================================
Init
================================================
*/

void BalanceManager::init()
{

    lastBalanceTime = millis();
    dutyState = false;
    balancingActive = false;

}



/*
================================================
Update
================================================
*/

void BalanceManager::update()
{

    uint32_t now = millis();


    /*
    Toggle duty state
    */

    if(now - lastBalanceTime > BALANCE_DUTY_TIME)
    {

        dutyState = !dutyState;

        lastBalanceTime = now;

    }


    /*
    OFF phase → disable balancing
    */

    if(!dutyState)
    {

        disableBalancing();

        balancingActive = false;

        return;

    }


    /*
    ON phase → evaluate cells
    */

    evaluateCells();

}



/*
================================================
Balancing state query
================================================
*/

bool BalanceManager::isBalancing()
{
    return balancingActive;
}



/*
================================================
Evaluate cells
================================================
*/

void BalanceManager::evaluateCells()
{

    float highCell = bms.getHighCellVolt();
    float lowCell  = bms.getLowCellVolt();

    float delta = highCell - lowCell;


    /*
    --------------------------------
    Do not balance while current flows
    --------------------------------
    */

    if(fabs(currentact) > BALANCE_CURRENT_LIMIT)
    {

        disableBalancing();
        balancingActive = false;
        return;

    }


    /*
    --------------------------------
    Voltage threshold
    --------------------------------
    */

    if(highCell < BALANCE_START_VOLTAGE)
    {

        disableBalancing();
        balancingActive = false;
        return;

    }


    /*
    --------------------------------
    Cell delta threshold
    --------------------------------
    */

    if(delta < BALANCE_DELTA)
    {

        disableBalancing();
        balancingActive = false;
        return;

    }


    balancingActive = true;

    applyBalancing();

}

/*
================================================
Disable balancing
================================================
*/

void BalanceManager::disableBalancing()
{

    int modules = bms.getNumModules();

    for(int m=0; m<modules; m++)
    {

        bms.sendBalanceMask(m,0);

    }

}
