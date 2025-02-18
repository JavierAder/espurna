#pragma once

#include "../analog_inputs.h"
#include "../settings.h"
#include "../settings_convert.h"
#include "AnalogSensor.h"
#include "NTCSensor.h"

class NTCMuxSensor : public NTCSensor
{
public:
  unsigned char id() const override
  {
    return SENSOR_NTC_MUX_ID;
  }

  // Descriptive name of the sensor
  String description() const override
  {
    char buffer[31];
    snprintf_P(buffer, sizeof(buffer),
      PSTR("NTC Mux Sensor Dev: %2d Pin: %2d"), _in_device, _pin);
    return String(buffer);
  }
};

class NTCMuxSensorConfig
{
public:
  NTCMuxSensorConfig()
  {
  }

  std::vector<String> splitConfig(String str)
  {
    std::vector<String> strings;
    char separator = ',';
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

  std::vector<NTCMuxSensor *> getSensors()
  {
    std::vector<NTCMuxSensor *> ntcSensors;
    // FORMAT ntcMuxNumber: int, number of ntc sensors
    // FORMAT ntcMuxX: X=1.. to number of ntcs; 10 items separated by commas
    // InDevice:int,AnalogPin:int,samples:int,delay:int,upResistor:long,downResistor:long,inputVoltage:double,beta:long,r0:long,t0:long
    // Where:
    // InDevice:see IN_DEVICE_xxx in analog_input.h (0=standard analog read, 1= generic mux, 2= ADS1115 with address 1001000 etc)
    // AnalogPin:pin/channel in device used for read

    // Examples: 10 k upResistor, voltage 1.0, beta 3799,R0 10000,T0 298.15 (25 C in Kelvin)
    //  String s1="2,0,1,0,10000,0,1.0,3799,10000,298.15"; //ADS1115 GND channel 0
    //  String s2="2,1,1,0,10000,0,1.0,3799,10000,298.15"; //ADS1115 GND channel 1
    //  String s3="2,2,1,0,10000,0,1.0,3799,10000,298.15"; //ADS1115 GND channel 2

    using namespace espurna::settings::internal;
    std::vector<String> allConfigs = getAllConfig();

    for (String config : allConfigs)
    {
      DEBUG_MSG_P( PSTR("ntcMuxSensor Config: %s\n"), config.c_str());

      std::vector<String> dataSensor = AnalogInputs::Inst()->splitConfig(config);
      if (dataSensor.size() != 10)
      {
        DEBUG_MSG_P( PSTR("Incorrect number of parameters in configuration\n"));
        continue;
      }
      NTCMuxSensor *sensor = new NTCMuxSensor();
      // TODO:check values
      int in_device = convert<int>(dataSensor[0]);
      int pin = convert<int>(dataSensor[1]);
      int samples = convert<int>(dataSensor[2]);
      int delay = convert<int>(dataSensor[3]);
      long rUp = convert<long>(dataSensor[4]);
      long rDown = convert<long>(dataSensor[5]);
      double voltage = convert<double>(dataSensor[6]);
      long beta = convert<long>(dataSensor[7]);
      long r0 = convert<long>(dataSensor[8]);
      double t0 = convert<double>(dataSensor[9]);

      sensor->setInDevice(in_device);
      sensor->setPin(pin);
      sensor->setSamples(samples);
      sensor->setDelay(delay);
      sensor->setUpstreamResistor(rUp);
      sensor->setDownstreamResistor(rDown);
      sensor->setInputVoltage(voltage);
      sensor->setBeta(beta);
      sensor->setR0(r0);
      sensor->setT0(t0);
      ntcSensors.push_back(sensor);
    }
    return ntcSensors;
  }

  std::vector<String> getAllConfig()
  {
    std::vector<String> allConfigs;

    using namespace espurna::settings::internal;
    String ns = getSetting("ntcMuxNumber"); // number of ntcMux in system
    DEBUG_MSG_P(PSTR("ntcMuxNumber: %s\n"), ns.c_str());
    if (ns == nullptr)
      return allConfigs;
    if (ns.equals(""))
      return allConfigs;

    using namespace espurna::settings::internal;
    int n = convert<int>(ns);
    if (n <= 0)
      return allConfigs;

    for (int i = 1; i <= n; i++)
    {
      String ntcSettingKey = "ntcMux" + String(i);
      String ntcSettingValue = getSetting(ntcSettingKey);
       DEBUG_MSG_P(PSTR("Config of %s -> %s\n"),
                    ntcSettingKey.c_str(),
                    ntcSettingValue.c_str());
      allConfigs.push_back(ntcSettingValue);
    }
    return allConfigs;
  }
};
