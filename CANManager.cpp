#include "CANManager.h"
#include "BMSModuleManager.h"

extern BMSModuleManager bms;



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
