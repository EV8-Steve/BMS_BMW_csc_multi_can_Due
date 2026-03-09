#include "BMSStateMachine.h"

#include "BMSModuleManager.h"
#include "IOManager.h"
#include "ProtectionManager.h"
#include "PrechargeManager.h"

extern BMSModuleManager bms;
extern IOManager io;
extern ProtectionManager protection;
extern PrechargeManager precharge;



BMSStateMachine::BMSStateMachine()
{
    state = BMS_BOOT;
    ignitionLatch = false;
}



void BMSStateMachine::update()
{

    bool ign = io.ignition();



    switch(state)
    {

        case BMS_BOOT:

        if(bms.getNumModules() > 0 && bms.modulesHealthy())
        {
            state = BMS_READY;
            Serial.println("BMS READY");
        }
            break;



        case BMS_READY:

            if(ign && !ignitionLatch)
            {
                ignitionLatch = true;

                precharge.init();

                state = BMS_PRECHARGE;

                Serial.println("PRECHARGE START");
            }

            if(!ign)
            {
                ignitionLatch = false;
            }

            break;



        case BMS_PRECHARGE:

            precharge.update(bms.getPackVoltage());

            if(precharge.ready())
            {

                io.setInverterEnable(true);

                state = BMS_DRIVE;

                Serial.println("DRIVE ENABLED");

            }
            if(!protection.dischargeAllowed())
            {
                Serial.println("FAULT DURING PRECHARGE");

                io.setMainContactor(false);
                state = BMS_ERROR;
            }

            break;



    case BMS_DRIVE:

        if(protection.moduleFaultActive())
        {

            Serial.println("MODULE FAULT");

            if(!io.vehicleInDrive())
            {
                io.setMainContactor(false);
                state = BMS_ERROR;
            }

        }

        if(!protection.dischargeAllowed())
        {

            Serial.println("DISCHARGE FAULT");

            if(!io.vehicleInDrive())
            {

                io.setMainContactor(false);

                state = BMS_ERROR;

            }

        }

        if(!ign)
        {
            io.setMainContactor(false);
            io.setInverterEnable(false);

            state = BMS_READY;
        }

        break;



    case BMS_ERROR:

        io.setFault(true);

        break;

    }

    }



BMSStateMachine::State BMSStateMachine::getState()
{

    return state;

}
