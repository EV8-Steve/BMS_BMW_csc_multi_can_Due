#include "OutlanderChargerStatus.h"
#include "due_can.h"
#include <Arduino.h>


OutlanderChargerStatus outlanderStatus;

/*
================================================
Process Incoming CAN Frames
================================================
*/

void OutlanderChargerStatus::processCAN(CAN_FRAME &frame)
{

    /*
    --------------------------------
    0x377 – DC-DC + charger status
    --------------------------------
    */

    if(frame.id == 0x377)
    {

        uint16_t aux =
            (frame.data.bytes[0] << 8) |
             frame.data.bytes[1];

        auxVoltage = aux * 0.01;       // 0.01V / bit


        uint16_t ac =
            (frame.data.bytes[2] << 8) |
             frame.data.bytes[3];

        acCurrent = ac * 0.1;          // 0.1A / bit


        temp1 = frame.data.bytes[4] - 40;
        temp2 = frame.data.bytes[5] - 40;
        temp3 = frame.data.bytes[6] - 40;


        status = frame.data.bytes[7];

    }



    /*
    --------------------------------
    0x389 – charger measurements
    --------------------------------
    */

    if(frame.id == 0x389)
    {

        chargerVoltage = frame.data.bytes[0] * 2.0;   // scaled x2

        dcCurrent = frame.data.bytes[7] * 0.1;        // 0.1A / bit

    }



    /*
    --------------------------------
    0x38A – EVSE information
    --------------------------------
    */

    if(frame.id == 0x38A)
    {

        // EVSE duty cycle
        // useful to determine available current

        // byte 3 = duty cycle %

    }

}
