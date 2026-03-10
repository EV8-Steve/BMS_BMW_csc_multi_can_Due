#pragma once

#include <Arduino.h>
#include <due_can.h>

class MGCharger
{

public:

    void init();

    void update(float voltage, float current, bool enable);

private:

    uint32_t lastSend;

    void send29C(float current, bool enable);

};