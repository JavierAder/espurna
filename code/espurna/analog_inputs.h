#pragma once

#include "types.h"
#include "gpio.h"
#include "settings.h"
#include "settings_convert.h"
#include "espurna.h"
#include "i2c.h"

// Device to read from
#define IN_DEVICE_ESP8266_ID 0     // standar ESP8266 analog input (pin A0)
#define IN_DEVICE_MUX_ID 1         // generic mux using ESP8266 ADC
#define IN_DEVICE_ADS1115_GND_ID 2 // ADS1115 via i2c with address 1001000 /four channels)
#define IN_DEVICE_ADS1115_VDD_ID 3 // ADS1115 via i2c with address 1001001 (four channels)
#define IN_DEVICE_ADS1115_SDA_ID 4 // ADS1115 via i2c with address 1001010 (four channels)
#define IN_DEVICE_ADS1115_SCL_ID 5 // ADS1115 via i2c with address 1001011 (four channels)

#define ADS1115_GND_ADDRESS 0b1001000
#define ADS1115_VDD_ADDRESS 0b1001001
#define ADS1115_SDA_ADDRESS 0b1001010
#define ADS1115_SCL_ADDRESS 0b1001011

// Based in Adafruit

/*=========================================================================
    POINTER REGISTER
    -----------------------------------------------------------------------*/
#define ADS1X15_REG_POINTER_MASK (0x03)      ///< Point mask
#define ADS1X15_REG_POINTER_CONVERT (0x00)   ///< Conversion
#define ADS1X15_REG_POINTER_CONFIG (0x01)    ///< Configuration
#define ADS1X15_REG_POINTER_LOWTHRESH (0x02) ///< Low threshold
#define ADS1X15_REG_POINTER_HITHRESH (0x03)  ///< High threshold
/*=========================================================================*/

/*=========================================================================
    CONFIG REGISTER
    -----------------------------------------------------------------------*/
#define ADS1X15_REG_CONFIG_OS_MASK (0x8000) ///< OS Mask
#define ADS1X15_REG_CONFIG_OS_SINGLE \
    (0x8000) ///< Write: Set to start a single-conversion
#define ADS1X15_REG_CONFIG_OS_BUSY \
    (0x0000) ///< Read: Bit = 0 when conversion is in progress
#define ADS1X15_REG_CONFIG_OS_NOTBUSY \
    (0x8000) ///< Read: Bit = 1 when device is not performing a conversion

#define ADS1X15_REG_CONFIG_MUX_MASK (0x7000) ///< Mux Mask
#define ADS1X15_REG_CONFIG_MUX_DIFF_0_1 \
    (0x0000) ///< Differential P = AIN0, N = AIN1 (default)
#define ADS1X15_REG_CONFIG_MUX_DIFF_0_3 \
    (0x1000) ///< Differential P = AIN0, N = AIN3
#define ADS1X15_REG_CONFIG_MUX_DIFF_1_3 \
    (0x2000) ///< Differential P = AIN1, N = AIN3
#define ADS1X15_REG_CONFIG_MUX_DIFF_2_3 \
    (0x3000)                                     ///< Differential P = AIN2, N = AIN3
#define ADS1X15_REG_CONFIG_MUX_SINGLE_0 (0x4000) ///< Single-ended AIN0 100 : AINP = AIN0 and AINN = GND
#define ADS1X15_REG_CONFIG_MUX_SINGLE_1 (0x5000) ///< Single-ended AIN1 101 : AINP = AIN1 and AINN = GND
#define ADS1X15_REG_CONFIG_MUX_SINGLE_2 (0x6000) ///< Single-ended AIN2 110 : AINP = AIN2 and AINN = GND
#define ADS1X15_REG_CONFIG_MUX_SINGLE_3 (0x7000) ///< Single-ended AIN3 111 : AINP = AIN3 and AINN = GND

constexpr uint16_t MUX_BY_CHANNEL[] = {
    ADS1X15_REG_CONFIG_MUX_SINGLE_0, ///< Single-ended AIN0
    ADS1X15_REG_CONFIG_MUX_SINGLE_1, ///< Single-ended AIN1
    ADS1X15_REG_CONFIG_MUX_SINGLE_2, ///< Single-ended AIN2
    ADS1X15_REG_CONFIG_MUX_SINGLE_3  ///< Single-ended AIN3
}; ///< MUX config by channel

#define ADS1X15_REG_CONFIG_PGA_MASK (0x0E00)   ///< PGA Mask
#define ADS1X15_REG_CONFIG_PGA_6_144V (0x0000) ///< +/-6.144V range = Gain 2/3
#define ADS1X15_REG_CONFIG_PGA_4_096V (0x0200) ///< +/-4.096V range = Gain 1
#define ADS1X15_REG_CONFIG_PGA_2_048V \
    (0x0400)                                   ///< +/-2.048V range = Gain 2 (default)
#define ADS1X15_REG_CONFIG_PGA_1_024V (0x0600) ///< +/-1.024V range = Gain 4
#define ADS1X15_REG_CONFIG_PGA_0_512V (0x0800) ///< +/-0.512V range = Gain 8
#define ADS1X15_REG_CONFIG_PGA_0_256V (0x0A00) ///< +/-0.256V range = Gain 16

#define ADS1X15_REG_CONFIG_MODE_MASK (0x0100)   ///< Mode Mask
#define ADS1X15_REG_CONFIG_MODE_CONTIN (0x0000) ///< Continuous conversion mode
#define ADS1X15_REG_CONFIG_MODE_SINGLE \
    (0x0100) ///< Power-down single-shot mode (default)

#define ADS1X15_REG_CONFIG_RATE_MASK (0x00E0) ///< Data Rate Mask

#define ADS1X15_REG_CONFIG_CMODE_MASK (0x0010) ///< CMode Mask
#define ADS1X15_REG_CONFIG_CMODE_TRAD \
    (0x0000)                                     ///< Traditional comparator with hysteresis (default)
#define ADS1X15_REG_CONFIG_CMODE_WINDOW (0x0010) ///< Window comparator

#define ADS1X15_REG_CONFIG_CPOL_MASK (0x0008) ///< CPol Mask
#define ADS1X15_REG_CONFIG_CPOL_ACTVLOW \
    (0x0000) ///< ALERT/RDY pin is low when active (default)
#define ADS1X15_REG_CONFIG_CPOL_ACTVHI \
    (0x0008) ///< ALERT/RDY pin is high when active

#define ADS1X15_REG_CONFIG_CLAT_MASK \
    (0x0004) ///< Determines if ALERT/RDY pin latches once asserted
#define ADS1X15_REG_CONFIG_CLAT_NONLAT \
    (0x0000)                                   ///< Non-latching comparator (default)
#define ADS1X15_REG_CONFIG_CLAT_LATCH (0x0004) ///< Latching comparator

#define ADS1X15_REG_CONFIG_CQUE_MASK (0x0003) ///< CQue Mask
#define ADS1X15_REG_CONFIG_CQUE_1CONV \
    (0x0000) ///< Assert ALERT/RDY after one conversions
#define ADS1X15_REG_CONFIG_CQUE_2CONV \
    (0x0001) ///< Assert ALERT/RDY after two conversions
#define ADS1X15_REG_CONFIG_CQUE_4CONV \
    (0x0002) ///< Assert ALERT/RDY after four conversions
#define ADS1X15_REG_CONFIG_CQUE_NONE \
    (0x0003) ///< Disable the comparator and put ALERT/RDY in high state (default)
/*=========================================================================*/

/** Gain settings */
typedef enum
{
    GAIN_TWOTHIRDS = ADS1X15_REG_CONFIG_PGA_6_144V,
    GAIN_ONE = ADS1X15_REG_CONFIG_PGA_4_096V,
    GAIN_TWO = ADS1X15_REG_CONFIG_PGA_2_048V,
    GAIN_FOUR = ADS1X15_REG_CONFIG_PGA_1_024V,
    GAIN_EIGHT = ADS1X15_REG_CONFIG_PGA_0_512V,
    GAIN_SIXTEEN = ADS1X15_REG_CONFIG_PGA_0_256V
} adsGain_t;

#define RATE_ADS1115_8SPS (0x0000)   ///< 8 samples per second
#define RATE_ADS1115_16SPS (0x0020)  ///< 16 samples per second
#define RATE_ADS1115_32SPS (0x0040)  ///< 32 samples per second
#define RATE_ADS1115_64SPS (0x0060)  ///< 64 samples per second
#define RATE_ADS1115_128SPS (0x0080) ///< 128 samples per second (default)
#define RATE_ADS1115_250SPS (0x00A0) ///< 250 samples per second
#define RATE_ADS1115_475SPS (0x00C0) ///< 475 samples per second
#define RATE_ADS1115_860SPS (0x00E0) ///< 860 samples per second

using Delay = espurna::duration::critical::Microseconds;

struct AnalogInputResult
{
    int raw_value;
    double voltage; // conversion form raw_value to voltage is device dependent
    uint8_t error;  // overflow of maximum value, connection error, etc
};
// Singleton, configurated in boot time (or runtime)

struct ADS1115Config

{
    uint8_t address; // i2c address

    uint8_t datarate;
    uint8_t mode;
    uint8_t gain;
    Delay delay;
    uint8_t error;

    bool configured;
};

struct MUXConfig
{

    std::vector<uint8_t> gpios;
    Delay delay;
    uint8_t error;
    bool configured;
};

// Singleton
class AnalogInputs
{
public:
    static constexpr int RawBits8266{10};

    static constexpr double RawMin8266{0.0};
    static constexpr double RawMax8266{(1 << RawBits8266) - 1};

    static AnalogInputs *createInst();

    // singleton: global instance
    static AnalogInputs *Inst();

    // hook for possible adaptation via subclasing
    static void setCustomAnalogInputs(AnalogInputs *custom)
    {
        AnalogInputs::_inst = custom;
    }

    // Helper ; TODO find a better place for it
    std::vector<String> splitConfig(String str)
    {
        return splitConfig(str, ',');
    }
    std::vector<String> splitConfig(String str, char separator)
    {
        if (str == nullptr)
            str = "";
        std::vector<String> strings;
        uint startIndex = 0, endIndex = 0;
        for (uint i = 0; i <= str.length(); i++)
        {

            // If we reached the end of the word or the end of the input.
            if (str[i] == separator || i == str.length())
            {
                endIndex = i;
                String temp;
                temp = str.substring(startIndex, endIndex);
                strings.push_back(temp);
                startIndex = endIndex + 1;
            }
        }
        return strings;
    }
    // called from sensor.cpp before loads sensors
    void setup()
    {
        using namespace espurna::settings::internal;
        String config;
        DEBUG_MSG_P(PSTR("Setup of Analog Inputs\n"));

        // FORMAT: analogMux=DelayBeforeRead(Microsecs),GPIO0,GPIO1....
        config = getSetting("analogMux");
        DEBUG_MSG_P(PSTR("analogMux: %s\n"), config.c_str());
        setupMUX(splitConfig(config));

        // FORMAT: ADS1115xxx= f=delay,datarate,gain, mode
        // delay=int, microsecs, currently not used
        // datarate= 0..7; 0= 8 SPS, 1 = 16 SPS...7= 860 SPS
        // gain= 0..5; 0 = +-6.144 V, 1 = +-4.096 V... +-5=0.256 V
        // mode= 0 continuous 1 single shot, currently not used; always in mode single shot
        config = getSetting("ADS1115GND");
        DEBUG_MSG_P(PSTR("ADS1115GND: %s\n"), config.c_str());
        ads1115gnd = createADS1115Config(splitConfig(config));
        ads1115gnd.address = ADS1115_GND_ADDRESS;

        config = getSetting("ADS1115VDD");
        DEBUG_MSG_P(PSTR("ADS1115VDD: %s\n"), config.c_str());
        ads1115vdd = createADS1115Config(splitConfig(config));
        ads1115vdd.address = ADS1115_VDD_ADDRESS;

        config = getSetting("ADS1115SDA");
        DEBUG_MSG_P(PSTR("ADS1115SDA: %s\n"), config.c_str());
        ads1115sda = createADS1115Config(splitConfig(config));
        ads1115sda.address = ADS1115_SDA_ADDRESS;

        config = getSetting("ADS1115SCL");
        DEBUG_MSG_P(PSTR("ADS1115SCL: %s\n"), config.c_str());
        ads1115scl = createADS1115Config(splitConfig(config));
        ads1115scl.address = ADS1115_SCL_ADDRESS;
    }

    void setupMUX(std::vector<String> configs)
    {
        using namespace espurna::settings::internal;
        if (configs.size() == 0)
        {
            DEBUG_MSG_P(PSTR("No config for generic mux found\n"));
            return;
        }

        mux.configured = true;
        if (configs.size() < 2)
        {
            DEBUG_MSG_P(PSTR("Incorrect number of parameters in configuration\n"));
            mux.error = SENSOR_ERROR_CONFIG;
            return;
        }
        mux.delay = Delay{convert<int>(configs[0])};
        for (uint i = 1; i < configs.size(); i++)
        {

            uint8_t muxGPIO = convert<int>(configs[i]);
            // locks gpio and mode
            if (!gpioLock(muxGPIO))
            {
                DEBUG_MSG_P(PSTR("ERROR In Mux Configuration: Gpio %d can't be locked\n"),
                            muxGPIO);
                mux.error = SENSOR_ERROR_GPIO_USED;
                return;
            }
            pinMode(muxGPIO, OUTPUT);
            mux.gpios.push_back(muxGPIO);
        }
    }

    ADS1115Config createADS1115Config(std::vector<String> configs)
    {
        using namespace espurna::settings::internal;
        ADS1115Config config;
        config.configured = false;
        config.error = 0;
        if (configs.size() == 0)
        {
            DEBUG_MSG_P(PSTR("No configuration found for ADS1115 device. Skipped\n"));
            return config;
        }

        config.configured = true;
        if (configs.size() != 4)
        {
            DEBUG_MSG_P(PSTR("Incorrect number of parameters in configuration\n"));
            config.error = SENSOR_ERROR_CONFIG;
            return config;
        }
        config.delay = Delay{convert<int>(configs[0])};
        config.datarate = convert<int>(configs[1]);
        config.gain = convert<int>(configs[2]);
        config.mode = convert<int>(configs[3]);
        config.error = checkADS111Config(config);

        if (config.error)
        {
            DEBUG_MSG_P(PSTR("Error in configuration for ADS1115 device\n"));
        }
        else
        {
            DEBUG_MSG_P(PSTR("ADS1115 device correctly configured!\n"));
        }
        return config;
    }
    uint8_t checkADS111Config(ADS1115Config config)
    {
        // TODO
        return 0;
    }

    AnalogInputs()
    {
        mux.configured = false;
        ads1115gnd.configured = false;
        ads1115vdd.configured = false;
        ads1115sda.configured = false;
        ads1115scl.configured = false;
        mux.error = 0;
        ads1115gnd.error = 0;
        ads1115vdd.error = 0;
        ads1115sda.error = 0;
        ads1115scl.error = 0;
    }

    // standard analog read of ESP8266
    AnalogInputResult analogRead()
    {

        return analogRead(IN_DEVICE_ESP8266_ID, 0);
    }
    AnalogInputResult analogRead(uint8_t device_id, uint8_t pin)
    {
        AnalogInputResult result;
        // IMPORTANT: also calculate the voltage
        // if (device_id == IN_DEVICE_ESP8266_ID)
        //	normal read via ::analogRead
        // else if (device_id == IN_DEVICE_ADS1115_GND_ID )
        //	read via i2c using pin and ads1115gnd
        // etc
        if (device_id == IN_DEVICE_ESP8266_ID)
        {
            return analogRead8266(pin);
        }

        if (device_id == IN_DEVICE_MUX_ID)
        {
            return analogReadMUX(pin);
        }

        if (device_id == IN_DEVICE_ADS1115_GND_ID)
        {
            return analogReadADS1115(ads1115gnd, pin);
        }

        if (device_id == IN_DEVICE_ADS1115_VDD_ID)
        {
            return analogReadADS1115(ads1115vdd, pin);
        }
        if (device_id == IN_DEVICE_ADS1115_SDA_ID)
        {
            return analogReadADS1115(ads1115sda, pin);
        }
        if (device_id == IN_DEVICE_ADS1115_SCL_ID)
        {
            return analogReadADS1115(ads1115scl, pin);
        }

        result.error = SENSOR_ERROR_SUPPORT; // analog device not defined

        return result;
    }

protected:
    AnalogInputResult analogRead8266(uint8_t pin)
    {

        AnalogInputResult result;
        result.error = 0;
        result.raw_value = ::analogRead(pin);
        result.voltage = result.raw_value / AnalogInputs::RawMax8266;
        return result;
    }
    AnalogInputResult analogReadMUX(uint8_t pin)
    {

        AnalogInputResult result;
        result.error = 0;
        if (!mux.configured)
        {
            result.error = SENSOR_ERROR_CONFIG; // error mux not configured
            return result;
        }
        if (mux.error)
        {
            result.error = mux.error; // configuration error in mux
            return result;
        }

        for (uint i = 0; i < mux.gpios.size(); i++)
        {
            uint8_t gpio = mux.gpios[i];
            int bit = pin & 0x01;
            if (bit)
            {
                digitalWrite(gpio, HIGH);
            }
            else
            {
                digitalWrite(gpio, LOW);
            }
            pin = pin >> 1;
        }
        // TODO:
        // If pin >0 -> error

        // delay before analog Read
        espurna::time::critical::delay(mux.delay);

        result.raw_value = ::analogRead(0); // 0 or A0?
        result.voltage = result.raw_value / AnalogInputs::RawMax8266;
        return result;
    }

    AnalogInputResult analogReadADS1115(ADS1115Config config, uint8 pin)
    {
        AnalogInputResult result;

        if (!config.configured)
        {
            result.error = SENSOR_ERROR_CONFIG; // error ads1115 not configured
            return result;
        }
        if (config.error)
        {
            result.error = config.error; // configuration error in ADS1115
            return result;
        }
        if (pin > 3)
        {
            result.error = SENSOR_ERROR_OTHER; // error ads1115 channel incorrect
            return result;
        }

        startADCReadingADS1115(config, pin, /*continuous=*/false);

        // Wait for the conversion to complete
        uint16_t timeout = getReadTimeoutADS1115(config.datarate);
        // getReadTimeout returns, say, 16 mls for datarate 128, 4 mls for datarate 826
        int16_t delayBetweenChecks = timeout / 4;
        unsigned long firstCheck = ::millis();
        while (!conversionCompleteADS1115(config))
        {
            unsigned long now = ::millis();
            if ((now - firstCheck) > timeout)
            {
                result.error = SENSOR_ERROR_TIMEOUT;
                return result;
            }
            else
            {
                ::delay(delayBetweenChecks);
            }
        }

        // Read the conversion results
        int16_t raw = getLastConversionResultsADS1115(config);

        result.error = 0;
        result.raw_value = raw;
        result.voltage = computeVolts(gainToBits(config.gain), raw);

        return result;
    }

    void startADCReadingADS1115(ADS1115Config configADS, uint8_t channel, bool continuous)
    {
        // Start with default values
        uint16_t config =
            ADS1X15_REG_CONFIG_CQUE_1CONV |   // Set CQUE to any value other than-- 1:0
                                              // None so we can use it in RDY mode
            ADS1X15_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)-- 2
            ADS1X15_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)-- 3
            ADS1X15_REG_CONFIG_CMODE_TRAD;    // Traditional comparator (default val)-- 4

        // Set data rate -- 7:5
        uint16_t m_dataRate = dataRateToBits(configADS.datarate);
        config |= m_dataRate;

        // continuous o single-shot -- 8
        if (continuous)
        {
            config |= ADS1X15_REG_CONFIG_MODE_CONTIN;
        }
        else
        {
            config |= ADS1X15_REG_CONFIG_MODE_SINGLE;
        }

        // Set PGA/voltage range -- 11:9
        adsGain_t m_gain = gainToBits(configADS.gain);
        config |= m_gain;

        // Set channels --14:12
        uint16_t mux = MUX_BY_CHANNEL[channel];
        config |= mux;

        // Set 'start single-conversion' bit -- 15
        config |= ADS1X15_REG_CONFIG_OS_SINGLE;

        // Write config register to the ADC
        // writeRegister(ADS1X15_REG_POINTER_CONFIG, config);
        i2c_write_uint16(configADS.address, ADS1X15_REG_POINTER_CONFIG, config);

        // Set ALERT/RDY to RDY mode.
        // writeRegister(ADS1X15_REG_POINTER_HITHRESH, 0x8000);
        // writeRegister(ADS1X15_REG_POINTER_LOWTHRESH, 0x0000);
        i2c_write_uint16(configADS.address, ADS1X15_REG_POINTER_HITHRESH, 0x8000);
        i2c_write_uint16(configADS.address, ADS1X15_REG_POINTER_LOWTHRESH, 0x0000);
    }

    bool conversionCompleteADS1115(ADS1115Config config)
    {
        // return (readRegister(ADS1X15_REG_POINTER_CONFIG) & 0x8000) != 0;
        return (i2c_read_uint16(config.address, ADS1X15_REG_POINTER_CONFIG) & 0x8000) != 0;
    }

    int16_t getLastConversionResultsADS1115(ADS1115Config config)
    {
        // Read the conversion results
        // uint16_t res = readRegister(ADS1X15_REG_POINTER_CONVERT) >> m_bitShift;
        uint16_t res = i2c_read_uint16(config.address, ADS1X15_REG_POINTER_CONVERT);
        return (int16_t)res;
    }
    // timeout in milliseconds for reading ADS1115 before assume error (connection i2c, etc)
    // depends on datarate, currently only for oneshot mode
    uint16_t getReadTimeoutADS1115(uint8_t datarate)
    {
        switch (datarate)
        {
        case 0:
            return 150; // RATE_ADS1115_8SPS, 8 sps, 1 sample =125 mls;
        case 1:
            return 100; // RATE_ADS1115_16SPS, 16 sps, 1 sample 62.5 nls;
        case 2:
            return 60; // RATE_ADS1115_32SPS, 32 sps, 1 sample 32.25 mls;
        case 3:
            return 30; // RATE_ADS1115_64SPS; 64 sps, 1 sample 15.625 mls
        case 4:
            return 20; // RATE_ADS1115_128SPS; 128 sps, 1 sample 7.8125 mls
        case 5:
            return 10; // RATE_ADS1115_250SPS; 250 sps, 1 sample 4 mls
        case 6:
            return 5; // RATE_ADS1115_475SPS; 475 sps, 1 sample 2.11 mls
        case 7:
            return 4; // RATE_ADS1115_860SPS; 860 sps, 1 sample 1.16 mls
        }
        return 20; // RATE_ADS1115_128SPS; 128 sps, 1 sample 7.8125 mls
    }
    float computeVolts(adsGain_t gain, int16_t counts)
    {
        // see data sheet Table 3
        float fsRange;
        switch (gain)
        {
        case GAIN_TWOTHIRDS:
            fsRange = 6.144f;
            break;
        case GAIN_ONE:
            fsRange = 4.096f;
            break;
        case GAIN_TWO:
            fsRange = 2.048f;
            break;
        case GAIN_FOUR:
            fsRange = 1.024f;
            break;
        case GAIN_EIGHT:
            fsRange = 0.512f;
            break;
        case GAIN_SIXTEEN:
            fsRange = 0.256f;
            break;
        default:
            fsRange = 0.0f;
        }
        return counts * (fsRange / (32768));
    }

    adsGain_t gainToBits(uint8_t gain)
    {

        switch (gain)
        {
        case 0:
            return GAIN_TWOTHIRDS;
        case 1:
            return GAIN_ONE;
        case 2:
            return GAIN_TWO;
        case 3:
            return GAIN_FOUR;
        case 4:
            return GAIN_EIGHT;
        case 5:
            return GAIN_SIXTEEN;
        }
        return GAIN_ONE;
    }

    uint16_t dataRateToBits(uint8_t dataRate)
    {
        switch (dataRate)
        {
        case 0:
            return RATE_ADS1115_8SPS;
        case 1:
            return RATE_ADS1115_16SPS;
        case 2:
            return RATE_ADS1115_32SPS;
        case 3:
            return RATE_ADS1115_64SPS;
        case 4:
            return RATE_ADS1115_128SPS;
        case 5:
            return RATE_ADS1115_250SPS;
        case 6:
            return RATE_ADS1115_475SPS;
        case 7:
            return RATE_ADS1115_860SPS;
        }
        return RATE_ADS1115_128SPS;
    }

    static AnalogInputs *_inst;
    MUXConfig mux;
    ADS1115Config ads1115gnd;
    ADS1115Config ads1115vdd;
    ADS1115Config ads1115sda;
    ADS1115Config ads1115scl;
};

AnalogInputs *AnalogInputs::_inst = nullptr;

AnalogInputs *AnalogInputs::createInst()
{
    AnalogInputs::_inst = new AnalogInputs();
    return _inst;
}
AnalogInputs *AnalogInputs::Inst()
{
    return AnalogInputs::_inst;
}

#ifndef __cpp_inline_variables
constexpr int AnalogInputs::RawBits8266;

constexpr double AnalogInputs::RawMin8266;
constexpr double AnalogInputs::RawMax8266;
#endif
