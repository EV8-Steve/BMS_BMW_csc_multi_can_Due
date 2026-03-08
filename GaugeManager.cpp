#include "GaugeManager.h"
#include "SettingsManager.h"
#include "config.h"

extern SettingsManager settings;



void GaugeManager::init()
{
    pinMode(GAUGE_PWM_PIN, OUTPUT);
    filteredSOC = 0;
}



void GaugeManager::update(float soc)
{

    /* SOC smoothing */

    filteredSOC = filteredSOC * 0.9f + soc * 0.1f;

    uint8_t pwm = computePWM(filteredSOC);

    analogWrite(GAUGE_PWM_PIN, pwm);

}



uint8_t GaugeManager::computePWM(float soc)
{

    if(soc < 0) soc = 0;
    if(soc > 100) soc = 100;

    auto &s = settings.get();

    float pwm;


    /*
    --------------------------------
    Piecewise interpolation
    --------------------------------
    */

    if(soc <= 50)
    {

        float ratio = soc / 50.0;

        pwm = s.gaugelow + ratio * (s.gaugemid - s.gaugelow);

    }
    else
    {

        float ratio = (soc - 50.0) / 50.0;

        pwm = s.gaugemid + ratio * (s.gaugehigh - s.gaugemid);

    }


    /*
    --------------------------------
    Gauge inversion
    --------------------------------
    */

    if(s.gaugeinvert)
    {
        pwm = s.gaugehigh - (pwm - s.gaugelow);
    }


    if(pwm < 0) pwm = 0;
    if(pwm > 255) pwm = 255;

    return (uint8_t)pwm;

}
