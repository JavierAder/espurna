#pragma once

#include "../analog_inputs.h"
#include "../settings.h"
#include "../settings_convert.h"
#include "AnalogSensor.h"

class RawAnalogSensor : public AnalogSensor
{
public:
    RawAnalogSensor()
    {
    }

    unsigned char id() const override
    {
        return 123; // TODO
    }

    unsigned char count() const override
    {
        return 3;
    }
    // Descriptive name of the sensor
    String description() const override
    {
        return F("Raw Analog Sensor");
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
        // FORMAT:rawAnalogSensors=analog_devide_id1,channel1,mult1;analog_devide_id2,channel2,mult2;...
        configSensors = getSetting("rawAnalogSensors");
        std::vector<String> configs = AnalogInputs::Inst()->splitConfig(configSensors, ';');

        for (auto config : configs)
        {
            std::vector<String> dataSensor = AnalogInputs::Inst()->splitConfig(config);
            if (dataSensor.size() != 3)
            {
                // error:wrong number of items in config string
                // TODO: print debug?
                continue;
            }
            int device_id = convert<int>(dataSensor[0]);
            int channel = convert<int>(dataSensor[1]);
            double mult = convert<double>(dataSensor[2]);
            RawAnalogSensor* ras = new RawAnalogSensor();
            ras->setInDevice(device_id);
            ras->setPin(channel);
            ras->setMult(mult);
            rass.push_back(ras);
        }

        return rass;
    }
};
