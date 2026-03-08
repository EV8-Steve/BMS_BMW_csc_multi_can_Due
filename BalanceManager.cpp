#include "BalanceManager.h"
#include "BMSModuleManager.h"
#include "config.h"
#include "SettingsManager.h"

#include <math.h>

struct BalanceCandidate
{
    float voltage;
    uint8_t module;
    uint8_t cell;
};

extern BMSModuleManager bms;
extern float currentact;
extern SettingsManager settingsManager;



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

    for(int i=0;i<MAX_MODULES;i++)
    {
        moduleCooldown[i] = 0;
    }

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



void BalanceManager::applyBalancing()
{

    uint32_t now = millis();

    int modules = bms.getNumModules();
    if(modules > MAX_MODULES)
        modules = MAX_MODULES;

    if(modules == 0)
        return;

    auto &settings = settingsManager.get();

    float lowCell  = bms.getLowCellVolt();
    float highCell = bms.getHighCellVolt();

    balancingActive = false;

    BalanceCandidate candidates[MAX_MODULES * CELLS_PER_MODULE];
    int candidateCount = 0;

    /*
    Scan all cells
    */

    for(int m=0; m<modules; m++)
    {
        if(now < moduleCooldown[m])
            continue;

        for(int c=0; c<CELLS_PER_MODULE; c++)
        {

            float v = bms.getCellVoltage(m,c);

            if(v < 2.0)
                continue;

            if(v > settings.balanceVoltage &&
               v > (lowCell + settings.balanceDelta))
            {
                if(candidateCount < (MAX_MODULES * CELLS_PER_MODULE))
                {
                    candidates[candidateCount].voltage = v;
                    candidates[candidateCount].module  = m;
                    candidates[candidateCount].cell    = c;

                    candidateCount++;
                }
            }

        }
    }

    /*
    Nothing to balance
    */

    if(candidateCount == 0)
    {
        disableBalancing();
        return;
    }

    /*
    Sort candidates by voltage (highest first)
    */

    for(int i=0;i<candidateCount-1;i++)
    {
        for(int j=i+1;j<candidateCount;j++)
        {
            if(candidates[j].voltage > candidates[i].voltage)
            {
                BalanceCandidate temp = candidates[i];
                candidates[i] = candidates[j];
                candidates[j] = temp;
            }
        }
    }

    /*
    Build module masks
    */

    uint16_t masks[MAX_MODULES];

    for(int i=0;i<MAX_MODULES;i++)
        masks[i] = 0;

    int activeCells = 0;

    for(int i=0;i<candidateCount;i++)
    {

        if(activeCells >= MAX_BALANCE_CELLS_GLOBAL)
            break;

        uint8_t m = candidates[i].module;
        uint8_t c = candidates[i].cell;

        masks[m] |= (1 << c);

        activeCells++;

    }

    /*
    Send masks to modules
    */

    for(int m=0;m<modules;m++)
    {

        if(masks[m] != 0)
        {
            balancingActive = true;
            moduleCooldown[m] = now + BALANCE_MODULE_COOLDOWN_MS;
        }

        bms.sendBalanceMask(m,masks[m]);

    }

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

    auto &settings = settingsManager.get();

    if(highCell < settings.balanceVoltage)
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

    if(delta < settings.balanceDelta)
    {

        disableBalancing();
        balancingActive = false;
        return;

    }




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
    if(modules > MAX_MODULES)
        modules = MAX_MODULES;

    if(modules == 0)
        return;

    for(int m=0; m<modules; m++)
    {
        bms.sendBalanceMask(m,0);
    }

}
