#include "AnalogSensor.h"

class RawAnalogSensor : public AnalogSensor
{
public:
    unsigned char id() const override
    {
        return 123; // TODO
    }

    unsigned char count() const override
    {
        return 2;
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

        return 0;
    }
};
