#pragma once

#include "../analog_inputs.h"
#include "../settings.h"
#include "../settings_convert.h"
#include "AnalogSensor.h"

class RawAnalogSensor : public AnalogSensor
{
public:
    unsigned char id() const override
    {
        return SENSOR_RAW_ANALOG_ID;
    }

    unsigned char count() const override
    {
        return 3;
    }
    // Descriptive name of the sensor
    String description() const override
    {
        char buffer[34];
        snprintf_P(buffer, sizeof(buffer),
                   PSTR("Raw Analog Sensor Dev: %2d Pin: %2d"), _in_device, _pin);
        return String(buffer);
    }

    // Type for slot # index
    unsigned char type(unsigned char index) const override
    {
        if (index == 0)
        {
            return MAGNITUDE_COUNT;
        }

        if (index == 1)
        {
            return MAGNITUDE_VOLTAGE;
        }
        if (index == 2)
        {
            return MAGNITUDE_ANALOG;
        }

        return MAGNITUDE_NONE;
    }

    // Current value for slot # index
    double value(unsigned char index) override
    {
        if (index == 0)
        {
            return _sampledValue();
        }
        if (index == 1)
        {
            return _sampledVoltageValue();
        }
        if (index == 2)
        {
            return _sampledVoltageValue() * getMult();
        }

        return 0;
    }

    double getMult()
    {
        return _mult;
    }
    void setMult(double mult)
    {
        _mult = mult;
    }

protected:
    double _mult{1.0};
};

class RawAnalogSensorConfig
{

public:
    RawAnalogSensorConfig()
    {
    }
    std::vector<RawAnalogSensor *> getSensors()
    {
        std::vector<RawAnalogSensor *> rass;
        using namespace espurna::settings::internal;
        String configSensors;
        // FORMAT:rawAnalogSensors=RA1;RA2...
        // FORMAT RAx = analog_devide_id1,channel1,mult1,delay,samples

        configSensors = getSetting("rawAnalogSensors");
        DEBUG_MSG_P(PSTR("rawAnalogSensors: %s\n"), configSensors.c_str());

        std::vector<String> configs = AnalogInputs::Inst()->splitConfig(configSensors, ';');
        DEBUG_MSG_P(PSTR("rawAnalogSensors Configs number: %d\n"), configs.size());

        for (String config : configs)
        {
            DEBUG_MSG_P(PSTR("rawAnalogSensors Config: %s\n"), config.c_str());

            std::vector<String> dataSensor = AnalogInputs::Inst()->splitConfig(config);
            if (dataSensor.size() != 5)
            {
                DEBUG_MSG_P(PSTR("Incorrect number of parameters in configuration\n"));
                continue;
            }
            int device_id = convert<int>(dataSensor[0]);
            int channel = convert<int>(dataSensor[1]);
            double mult = convert<double>(dataSensor[2]);
            int samples = convert<int>(dataSensor[3]);
            int delay = convert<int>(dataSensor[4]);

            RawAnalogSensor *ras = new RawAnalogSensor();
            ras->setInDevice(device_id);
            ras->setPin(channel);
            ras->setMult(mult);
            ras->setSamples(samples);
            ras->setDelay(delay);
            rass.push_back(ras);
        }

        return rass;
    }
};
