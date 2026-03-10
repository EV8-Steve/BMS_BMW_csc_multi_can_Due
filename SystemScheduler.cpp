#include "SystemScheduler.h"

#include "BMSModuleManager.h"
#include "BalanceManager.h"
#include "SOCManager.h"
#include "ProtectionManager.h"
#include "ChargerManager.h"
#include "SerialConsoleManager.h"
#include "CSCAddressManager.h"
#include "BMSStateMachine.h"

extern BMSModuleManager bms;
extern BalanceManager balance;
extern SOCManager soc;
extern ProtectionManager protection;
extern ChargerManager charger;
extern SerialConsoleManager console;
extern CSCAddressManager csc;
extern BMSStateMachine stateMachine;

extern float currentact;



uint32_t last10ms;
uint32_t last100ms;
uint32_t last1s;
uint32_t last5s;

uint32_t startupTime;



/*
================================================
Init
================================================
*/

void SystemScheduler::init()
{

    startupTime = millis();

    last10ms = 0;
    last100ms = 0;
    last1s = 0;
    last5s = 0;

}



/*
================================================
Update
================================================
*/

void SystemScheduler::update()
{

    uint32_t now = millis();



    /*
    ================================================
    10ms tasks
    ================================================
    */

    if(now - last10ms >= 10)
    {

        last10ms  += 10;

        bms.updateWatchdogs();

    }



    /*
    ================================================
    100ms tasks
    ================================================
    */

    if(now - last100ms >= 100)
    {

        last100ms  += 100;

        bms.updatePackStatistics();

        protection.update();

        balance.update();

        stateMachine.update();     // <-- drives contactors safely

        console.update();

    }



    /*
    ================================================
    1 second tasks
    ================================================
    */

    if(now - last1s >= 1000)
    {

        last1s  += 1000;

        bms.sendKeepAlive();

        soc.update(currentact);
        soc.checkFullChargeReset(
            bms.getHighCellVolt(),
            currentact
        );

        charger.update(
            bms.getPackVoltage(),
            bms.getHighCellVolt()
        );

    }



    /*
    ================================================
    Auto discovery lock
    ================================================
    */

    if(!csc.isLocked())
    {

        if(now - startupTime > 10000)
        {

            Serial.println("Auto locking module discovery");

            csc.lockDiscovery();

        }

    }



    /*
    ================================================
    Diagnostics
    ================================================
    */

    if(now - last5s >= 5000)
    {

        last5s  += 5000;

        Serial.print("Modules detected: ");
        Serial.println(bms.getNumModules());

    }

}
