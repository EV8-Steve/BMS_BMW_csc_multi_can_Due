#pragma once

class ProtectionManager
{

public:

    void init();

    void update();

    bool chargeAllowed();

    bool dischargeAllowed();

    bool moduleFaultActive();

private:
    bool moduleFault;

    bool allowCharge;

    bool allowDischarge;

    void checkVoltages();

    void checkTemperature();

};