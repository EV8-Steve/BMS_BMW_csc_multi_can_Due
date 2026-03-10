#include "MGCharger.h"
MGCharger mgcharger;

extern CANRaw Can0;

void MGCharger::init()
{
    lastSend = 0;
}

void MGCharger::update(float voltage, float current, bool enable)
{

    if(millis() - lastSend < 100)
        return;

    lastSend = millis();

    send29C(current, enable);
}

void MGCharger::send29C(float current, bool enable)
{

    CAN_FRAME frame;

    frame.id = 0x29C;
    frame.length = 8;

    uint8_t req = current * 20;   // 20 = 1A

    frame.data.bytes[0] = 0x08;   // max current limit
    frame.data.bytes[1] = enable ? 0x08 : 0x00;
    frame.data.bytes[2] = 0x24;
    frame.data.bytes[3] = req;

    frame.data.bytes[4] = 0x00;
    frame.data.bytes[5] = enable ? 0x8C : 0x00;
    frame.data.bytes[6] = enable ? 0x5A : 0x00;
    frame.data.bytes[7] = enable ? 0x3C : 0x00;

    Can0.sendFrame(frame);
}
