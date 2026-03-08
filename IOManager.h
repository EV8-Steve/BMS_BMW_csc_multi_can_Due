#pragma once
#include <Arduino.h>

#define PIN_MAIN_CONTACTOR 22
#define PIN_PRECHARGE      23
#define PIN_CHARGER_ENABLE 24
#define PIN_INVERTER_ENABLE 25

#define PIN_FAULT_OUTPUT   26

#define PIN_IGNITION       30
#define PIN_CHARGE_REQ     31
#define PIN_DISCHARGE_REQ  32
#define PIN_DRIVE_STATE    33


class IOManager
{

public:

    void init();

    void update();

    bool ignition();

    bool chargeRequest();

    bool dischargeRequest();

    bool vehicleInDrive();

    void setMainContactor(bool state);

    void setPrecharge(bool state);

    void setChargerEnable(bool state);

    void setInverterEnable(bool state);

    void setFault(bool state);

private:

    bool ign;
    bool chargeReq;
    bool dischargeReq;
    bool driveState;

};