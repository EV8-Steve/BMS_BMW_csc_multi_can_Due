#pragma once

/*
================================================
Hardware Platform
================================================
*/

#define MCU_DUE

#define CAN_SPEED 500000

#define GAUGE_PWM_PIN 6

/*
================================================
BMW CSC Layout
================================================
*/

#define MAX_CAN_BUSES 2
#define MAX_CSC_PER_BUS 6

#define MAX_MODULES (MAX_CAN_BUSES * MAX_CSC_PER_BUS)

#define CELLS_PER_MODULE 16
#define TEMP_SENSORS_PER_MODULE 2



/*
================================================
CAN Watchdog
================================================
*/

#define MODULE_TIMEOUT 3000      // ms before module considered dead
#define CAN_TIMEOUT     1000     // ms before CAN watchdog triggers



/*
================================================
Scheduler Timing
================================================
*/

#define TASK_FAST_MS     10
#define TASK_MEDIUM_MS   100
#define TASK_SLOW_MS     1000
#define TASK_DIAG_MS     5000



/*
================================================
Balancing Configuration
================================================
*/

#define BALANCE_DUTY_TIME      1000     // ms between balance checks
#define BALANCE_START_VOLTAGE  3.90     // cell voltage to start balancing
#define BALANCE_DELTA          0.010    // cell delta before balancing
#define BALANCE_HYSTERESIS     0.005    // hysteresis below highest cell
#define MAX_BALANCE_CELLS_PER_MODULE 6
#define BALANCE_CURRENT_LIMIT  2.0f
#define BALANCE_MODULE_COOLDOWN_MS 30000
#define MAX_BALANCE_CELLS_GLOBAL 12

/*
================================================
Cell Voltage Safety Limits
================================================
*/

#define OVER_VOLTAGE_LIMIT     4.20
#define UNDER_VOLTAGE_LIMIT    3.10



/*
================================================
Temperature Safety Limits
================================================
*/

#define OVER_TEMP_LIMIT        50
#define UNDER_TEMP_LIMIT      -10



/*
================================================
Charger Configuration
================================================
*/

#define CHARGER_UPDATE_PERIOD  100   // ms between CAN messages



/*
================================================
Current sensor Configuration
================================================
*/

#define SENSOR_NONE       0
#define SENSOR_LEM_CAB500 1
#define SENSOR_SHUNT      2
#define SENSOR_CAN        3


/*
================================================
CSC Discovery
================================================
*/

#define CSC_DISCOVERY_TIMEOUT  10000   // auto lock discovery after startup
#define CSC_DISCOVERY_STABLE_TIME  5000


/*
================================================
Serial Debug
================================================
*/

#define SERIAL_BAUD 115200