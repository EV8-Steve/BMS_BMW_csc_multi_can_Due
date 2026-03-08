#pragma once

#include <due_can.h>

class OutlanderCharger
{

public:

    void init();

    void update(float voltage, float current, bool enable);

private:

    uint32_t lastSend;

    void send285(bool enable);
    void send286(float voltage);
    void send287(float current);

};