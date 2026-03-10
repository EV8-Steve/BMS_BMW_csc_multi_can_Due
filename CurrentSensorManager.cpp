#include "CurrentSensorManager.h"

extern SettingsManager settingsManager;

float currentact = 0;


/*
================================================
Initialisation
================================================
*/

void CurrentSensorManager::init()
{

    currentAct = 0;

}


/*
================================================
Main Update
================================================
*/

void CurrentSensorManager::update()
{

    BMSSettings &settings = settingsManager.get();

    switch(settings.currentSensorType)
    {

        case CURRENT_SENSOR_ANALOG_SINGLE:
        case CURRENT_SENSOR_ANALOG_DUAL:
            processAnalog();
        break;

        default:
        break;

    }

}


/*
================================================
Process Incoming CAN Frames
================================================
*/

void CurrentSensorManager::processCAN(CAN_FRAME &frame)
{

    BMSSettings &settings = settingsManager.get();

    switch(settings.currentSensorType)
    {

        case CURRENT_SENSOR_LEM_CAB300:

            if(frame.id == 0x3C2)
            {
                processCAB300(frame);
            }

        break;


        case CURRENT_SENSOR_LEM_CAB500:

            if(frame.id == 0x3C1)
            {
                processCAB500(frame);
            }

        break;


        case CURRENT_SENSOR_ISA_IVT:

            if(frame.id == 0x521)
            {
                processISA(frame);
            }

        break;

        default:
        break;

    }

}


/*
================================================
LEM CAB300
================================================
*/

void CurrentSensorManager::processCAB300(CAN_FRAME &frame)
{

    int16_t raw = (frame.data.bytes[0] << 8) | frame.data.bytes[1];

    currentAct = raw * 0.1;
    currentact = getCurrent();

}


/*
================================================
LEM CAB500
================================================
*/

void CurrentSensorManager::processCAB500(CAN_FRAME &frame)
{

    int16_t raw = (frame.data.bytes[0] << 8) | frame.data.bytes[1];

    currentAct = raw * 0.1;
    currentact = getCurrent();

}


/*
================================================
ISA IVT-S
================================================
*/

void CurrentSensorManager::processISA(CAN_FRAME &frame)
{

    int32_t raw =
        (frame.data.bytes[2] << 24) |
        (frame.data.bytes[3] << 16) |
        (frame.data.bytes[4] << 8) |
        frame.data.bytes[5];

    currentAct = raw * 0.001;
        currentact = getCurrent();

}


/*
================================================
Analog Sensors
================================================
*/

void CurrentSensorManager::processAnalog()
{

    int raw = analogRead(A0);

    float voltage = raw * (3.3 / 4095.0);

    currentAct = (voltage - 1.65) * 100;

}


/*
================================================
Return Current
================================================
*/

float CurrentSensorManager::getCurrent()
{

    BMSSettings &settings = settingsManager.get();

    float current = currentAct;

    if(settings.currentInvert)
        current = -current;

    current *= settings.currentMultiplier;

    if(abs(current) < settings.currentDeadband)
        current = 0;

    return current;

}
