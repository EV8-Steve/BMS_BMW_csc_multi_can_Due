#pragma once

#include "BMSSettings.h"

class SettingsManager
{
public:
    void load();
    void save();

    BMSSettings& get();

    void factoryDefaults();

private:
    BMSSettings settings;
};

extern SettingsManager settingsManager;