#include "OutlanderCharger.h"

extern CANRaw Can0;

OutlanderCharger outlander;



void OutlanderCharger::init()
{
    lastSend = 0;
}



void OutlanderCharger::update(float voltage, float current, bool enable)
{

    if(millis() - lastSend < 100)
        return;

    lastSend = millis();

    send285(enable);
    send286(voltage);
    send287(current);

}



/*
================================================
Enable / Disable Charger
================================================
*/

void OutlanderCharger::send285(bool enable)
{

    CAN_FRAME frame;

    frame.id = 0x285;
    frame.length = 8;

    frame.data.bytes[0] = enable ? 1 : 0;

    for(int i=1;i<8;i++)
        frame.data.bytes[i] = 0;

    Can0.sendFrame(frame);

}



/*
================================================
Set Charger Voltage
================================================
*/

void OutlanderCharger::send286(float voltage)
{

    CAN_FRAME frame;

    frame.id = 0x286;
    frame.length = 8;

    uint16_t v = voltage * 10;

    frame.data.bytes[0] = v >> 8;
    frame.data.bytes[1] = v;

    for(int i=2;i<8;i++)
        frame.data.bytes[i] = 0;

    Can0.sendFrame(frame);

}



/*
================================================
Set Charger Current
================================================
*/

void OutlanderCharger::send287(float current)
{

    CAN_FRAME frame;

    frame.id = 0x287;
    frame.length = 8;

    uint16_t c = current * 10;

    frame.data.bytes[0] = c >> 8;
    frame.data.bytes[1] = c;

    for(int i=2;i<8;i++)
        frame.data.bytes[i] = 0;

    Can0.sendFrame(frame);

}
