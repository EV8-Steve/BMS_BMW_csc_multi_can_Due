#pragma once
#include <Arduino.h>

/*
================================================
Enumerations
================================================
*/

enum CurrentSensorType
{
    CURRENT_SENSOR_ANALOG_SINGLE = 0,
    CURRENT_SENSOR_ANALOG_DUAL   = 1,
    CURRENT_SENSOR_LEM_CAB300    = 2,
    CURRENT_SENSOR_LEM_CAB500    = 3,
    CURRENT_SENSOR_ISA_IVT       = 4
};

enum ChargerType
{
    CHARGER_RELAY      = 0,
    CHARGER_BRUSA      = 1,
    CHARGER_VOLT       = 2,
    CHARGER_ELTEK      = 3,
    CHARGER_ELCON      = 4,
    CHARGER_VICTRON    = 5,
    CHARGER_OUTLANDER  = 6
};


/*
================================================
Settings Structure
All values stored in engineering units
================================================
*/

struct BMSSettings
{

/*
Battery configuration
*/

float cellOverVoltage;       // volts
float cellUnderVoltage;      // volts

float cellChargeTarget;      // volts
float cellChargeHysteresis;  // volts

float balanceVoltage;        // volts
float balanceDelta;          // volts

float packCapacityAh;

uint16_t seriesCells;
uint16_t parallelStrings;


/*
Current limits
*/

float maxChargeCurrent;
float endChargeCurrent;

float maxDischargeCurrent;


/*
Temperature limits
*/

int16_t overTemp;
int16_t underTemp;


/*
Charger configuration
*/

uint8_t chargerType;
uint16_t chargerCanPeriod;
bool chargerDirectHV;


/*
Current sensor configuration
*/

uint8_t currentSensorType;
bool currentInvert;

float currentMultiplier;
float currentDeadband;

uint16_t currentCanID;

/*
fuel gauge settings
*/

uint8_t gaugelow;
uint8_t gaugemid;
uint8_t gaugehigh;
bool gaugeinvert;

};