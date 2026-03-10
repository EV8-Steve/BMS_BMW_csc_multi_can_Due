#include "CANManager.h"
#include "BMSModuleManager.h"
#include "CurrentSensorManager.h"
#include "ChargerManager.h"
#include "OutlanderChargerStatus.h"

extern OutlanderChargerStatus outlanderStatus;

extern BMSModuleManager bms;
extern CurrentSensorManager currentSensor;
extern ChargerManager charger;



/*
================================================
CAN Listener for Bus 0
================================================
*/

class CAN0Listener : public CANListener
{

public:

    void gotFrame(CAN_FRAME *frame, int mailbox)
    {

        bms.decodecan(0, *frame, 0);

        currentSensor.processCAN(*frame);
        outlanderStatus.processCAN(*frame);

    }

};



/*
================================================
CAN Listener for Bus 1
================================================
*/

class CAN1Listener : public CANListener
{

public:

    void gotFrame(CAN_FRAME *frame, int mailbox)
    {

        bms.decodecan(1, *frame, 0);
        currentSensor.processCAN(*frame);
        outlanderStatus.processCAN(*frame);

    }

};



CAN0Listener listener0;
CAN1Listener listener1;



/*
================================================
Init CAN system
================================================
*/

void CANManager::init()
{

    Can0.begin(CAN_SPEED);
    Can1.begin(CAN_SPEED);

    Can0.attachObj(&listener0);
    Can1.attachObj(&listener1);

}
