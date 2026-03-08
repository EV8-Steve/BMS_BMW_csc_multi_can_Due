#pragma once

#include <Arduino.h>

class SystemScheduler
{

public:

    void init();

    void update();

private:

    uint32_t last10ms;
    uint32_t last20ms;
    uint32_t last100ms;
    uint32_t last500ms;
    uint32_t last1s;
    uint32_t last5s;

};