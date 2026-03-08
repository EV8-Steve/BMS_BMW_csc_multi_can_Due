#pragma once

class ProtectionManager
{

public:

    void init();

    void update();

    bool chargeAllowed();

    bool dischargeAllowed();

private:

    bool allowCharge;

    bool allowDischarge;

    void checkVoltages();

    void checkTemperature();

};