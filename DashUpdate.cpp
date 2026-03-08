#include "DashUpdate.h"

#include "BMSModuleManager.h"
#include "BMSStateMachine.h"
#include "BalanceManager.h"

extern BMSModuleManager bms;
extern BMSStateMachine bmsState;
extern BalanceManager balance;

extern float currentact;



void dashEnd()
{
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
}



void dashText(const char* field,const char* txt)
{

    Serial2.print(field);
    Serial2.print("=\"");
    Serial2.print(txt);
    Serial2.print("\"");

    dashEnd();

}



void dashValue(const char* field,int v)
{

    Serial2.print(field);
    Serial2.print("=");
    Serial2.print(v);

    dashEnd();

}



void dashBool(const char* field,bool v)
{

    Serial2.print(field);
    Serial2.print("=");
    Serial2.print(v?1:0);

    dashEnd();

}



void DashUpdate()
{

    float packV = bms.getPackVoltage();

    float high = bms.getHighCellVolt();
    float low  = bms.getLowCellVolt();

    float delta = high-low;

    float avgTemp = bms.getAvgTemperature();



    /*
    SOC / voltage
    */

    dashValue("volt.val",(int)(packV*10));

    dashValue("current.val",(int)currentact);

    dashValue("highcell.val",(int)(high*1000));

    dashValue("lowcell.val",(int)(low*1000));

    dashValue("celldelta.val",(int)(delta*1000));



    /*
    temperature
    */

    dashValue("temp.val",(int)avgTemp);



    /*
    BMS state
    */

    switch(bmsState.getState())
    {

        case BMSStateMachine::BMS_BOOT:
            dashText("stat.txt","Boot");
            break;

        case BMSStateMachine::BMS_READY:
            dashText("stat.txt","Ready");
            break;

        case BMSStateMachine::BMS_PRECHARGE:
            dashText("stat.txt","Precharge");
            break;

        case BMSStateMachine::BMS_DRIVE:
            dashText("stat.txt","Drive");
            break;

        case BMSStateMachine::BMS_ERROR:
            dashText("stat.txt","Fault");
            break;

    }



    /*
    balancing indicator
    */

    dashBool("cellbal.val", balance.isBalancing());

}
