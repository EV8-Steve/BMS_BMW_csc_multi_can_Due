#include "IOManager.h"

void IOManager::init()
{

    pinMode(PIN_MAIN_CONTACTOR, OUTPUT);
    pinMode(PIN_PRECHARGE, OUTPUT);
    pinMode(PIN_CHARGER_ENABLE, OUTPUT);
    pinMode(PIN_INVERTER_ENABLE, OUTPUT);
    pinMode(PIN_FAULT_OUTPUT, OUTPUT);

    pinMode(PIN_IGNITION, INPUT_PULLUP);
    pinMode(PIN_CHARGE_REQ, INPUT_PULLUP);
    pinMode(PIN_DISCHARGE_REQ, INPUT_PULLUP);
    pinMode(PIN_DRIVE_STATE, INPUT_PULLUP);

}

void IOManager::update()
{

    ign = digitalRead(PIN_IGNITION);

    chargeReq = digitalRead(PIN_CHARGE_REQ);

    dischargeReq = digitalRead(PIN_DISCHARGE_REQ);

    driveState = digitalRead(PIN_DRIVE_STATE);

}

bool IOManager::ignition()
{
    return ign;
}

bool IOManager::chargeRequest()
{
    return chargeReq;
}

bool IOManager::dischargeRequest()
{
    return dischargeReq;
}

bool IOManager::vehicleInDrive()
{
    return driveState;
}

void IOManager::setMainContactor(bool state)
{
    digitalWrite(PIN_MAIN_CONTACTOR, state);
}

void IOManager::setPrecharge(bool state)
{
    digitalWrite(PIN_PRECHARGE, state);
}

void IOManager::setChargerEnable(bool state)
{
    digitalWrite(PIN_CHARGER_ENABLE, state);
}

void IOManager::setInverterEnable(bool state)
{
    digitalWrite(PIN_INVERTER_ENABLE, state);
}

void IOManager::setFault(bool state)
{
    digitalWrite(PIN_FAULT_OUTPUT, state);
}