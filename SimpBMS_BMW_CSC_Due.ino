#include <due_can.h>

#include "config.h"
#include "CANManager.h"

#include "BMSModuleManager.h"
#include "BMSStateMachine.h"
#include "BalanceManager.h"
#include "SOCManager.h"
#include "ProtectionManager.h"
#include "PrechargeManager.h"
#include "ChargerManager.h"
#include "IOManager.h"

#include "SystemScheduler.h"

#include "SerialConsoleManager.h"
#include "SettingsManager.h"
#include "CSCAddressManager.h"

#include "CurrentSensorManager.h"

#include "DashUpdate.h"



/*
================================================
Global objects
================================================
*/
CANManager canManager;

BMSModuleManager bms;

BalanceManager balance;

SOCManager soc;

ProtectionManager protection;

PrechargeManager precharge;

ChargerManager charger;

IOManager io;

SystemScheduler scheduler;

SerialConsoleManager console;

SettingsManager settingsManager;

CSCAddressManager csc;

CurrentSensorManager currentSensor;

BMSStateMachine stateMachine;



extern float currentact;



/*
================================================
Setup
================================================
*/

void setup()
{

    Serial.begin(SERIAL_BAUD);

    Serial2.begin(115200);



    canManager.init();



    settingsManager.load();



    io.init();

    balance.init();

    scheduler.init();



    console.printMainMenu();



}



/*
================================================
Loop
================================================
*/

void loop()
{

    scheduler.update();

    stateMachine.update();

}
