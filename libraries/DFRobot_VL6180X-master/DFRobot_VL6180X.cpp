/*!
 * @file DFRobot_VL6180X.cpp
 * @brief Implementation of DFRobot_VL6180X class
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @SKU SEN0427
 * @licence The MIT License (MIT)
 * @author [yangfeng]<feng.yang@dfrobot.com>
 * @version V1.0
 * @date 2021-02-08
 * @url  https://github.com/DFRobot/DFRobot_VL6180X
 */
#include <DFRobot_VL6180X.h>

DFRobot_VL6180X::DFRobot_VL6180X(uint8_t iicAddr,TwoWire *pWire):
_pWire(pWire)
{
  _deviceAddr = iicAddr;
  _modeGpio1Reg.reversed=0;
  _modeGpio1Reg.select = 0;
  _modeGpio1Reg.polarity = 0;
  _modeGpio1Reg.reservedBit6_7 = 0;
  
  _configIntGPIOReg.rangeIntMode=0;
  _configIntGPIOReg.alsIntMode=0;
  _configIntGPIOReg.reservedBit6_7=0;

  _clearIntReg.intClearSig = 0;
  _clearIntReg.reserved = 0;

  _rangeStartReg.startstop = 0;
  _rangeStartReg.select = 0;
  _rangeStartReg.reserved = 0;

  _ALSStartReg.startstop = 0;
  _ALSStartReg.select = 0;
  _ALSStartReg.reserved = 0;

  _analogueGainReg.gain = 6;
  _analogueGainReg.gain = 8;
  
  _gain = 1.0;
  _atime =100;

}

bool DFRobot_VL6180X::begin()
{
  _pWire->begin();
  if((getDeviceID()!=VL6180X_ID)){
    return false;
  }
  init();
  return true;
}

void DFRobot_VL6180X::init()
{
  if(read(VL6180X_SYSTEM_FRESH_OUT_OF_RESET,1)){
    write8bit(0x0207, 0x01);
    write8bit(0x0208, 0x01);
    write8bit(0x0096, 0x00);
    write8bit(0x0097, 0xfd);
    write8bit(0x00e3, 0x00);
    write8bit(0x00e4, 0x04);
    write8bit(0x00e5, 0x02);
    write8bit(0x00e6, 0x01);
    write8bit(0x00e7, 0x03);
    write8bit(0x00f5, 0x02);
    write8bit(0x00d9, 0x05);
    write8bit(0x00db, 0xce);
    write8bit(0x00dc, 0x03);
    write8bit(0x00dd, 0xf8);
    write8bit(0x009f, 0x00);
    write8bit(0x00a3, 0x3c);
    write8bit(0x00b7, 0x00);
    write8bit(0x00bb, 0x3c);
    write8bit(0x00b2, 0x09);
    write8bit(0x00ca, 0x09);
    write8bit(0x0198, 0x01);
    write8bit(0x01b0, 0x17);
    write8bit(0x01ad, 0x00);
    write8bit(0x00ff, 0x05);
    write8bit(0x0100, 0x05);
    write8bit(0x0199, 0x05);
    write8bit(0x01a6, 0x1b);
    write8bit(0x01ac, 0x3e);
    write8bit(0x01a7, 0x1f);
    write8bit(0x0030, 0x00);
  }
  write8bit(VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD, 0x30);
  write8bit(VL6180X_SYSALS_ANALOGUE_GAIN, 0x46);
  write8bit(VL6180X_SYSRANGE_VHV_REPEAT_RATE, 0xFF);
  write8bit(VL6180X_SYSALS_INTEGRATION_PERIOD, 0x63);
  write8bit(VL6180X_SYSRANGE_VHV_RECALIBRATE, 0x01);
  write8bit(VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD, 0x09);
  write8bit(VL6180X_SYSALS_INTERMEASUREMENT_PERIOD, 0x31);
  write8bit(VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x00);
  write8bit(VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME, 0x31);
  write8bit(VL6180X_INTERLEAVED_MODE_ENABLE, 0);
  write8bit(VL6180X_SYSTEM_MODE_GPIO1,0x20);
  write8bit(VL6180X_SYSTEM_FRESH_OUT_OF_RESET,0);
}

void DFRobot_VL6180X::setInterrupt(uint8_t mode)
{
  if(mode == VL6180X_DIS_INTERRUPT){
    write8bit(VL6180X_SYSTEM_MODE_GPIO1,0x20);
  }else if(mode == VL6180X_HIGH_INTERRUPT){
    write8bit(VL6180X_SYSTEM_MODE_GPIO1,0x10);
  }else if(mode == VL6180X_LOW_INTERRUPT){
    write8bit(VL6180X_SYSTEM_MODE_GPIO1,0x30);
  }
}

uint8_t DFRobot_VL6180X::rangePollMeasurement()
{
  _rangeStartReg.startstop = 1;
  _rangeStartReg.select = 0;
  write8bit(VL6180X_SYSRANGE_START,*((uint8_t*)(&_rangeStartReg)));
  return rangeGetMeasurement();
}

void DFRobot_VL6180X::rangeSetInterMeasurementPeriod(uint16_t periodMs)
{
  if(periodMs>10){
    if(periodMs<2550){
      periodMs = (periodMs/10) - 1;
    }else{
      periodMs = 254;
    }
  }
  write8bit(VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD, periodMs);
}
bool DFRobot_VL6180X::rangeConfigInterrupt(uint8_t mode)
{
  if(mode>VL6180X_NEW_SAMPLE_READY){
    return false;
  }
  _configIntGPIOReg.rangeIntMode=mode;
  write8bit(VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO,*((uint8_t*)(&_configIntGPIOReg)));
  return true;
}
bool DFRobot_VL6180X::alsConfigInterrupt(uint8_t mode)
{
  if(mode>VL6180X_NEW_SAMPLE_READY){
    return false;
  }
  _configIntGPIOReg.alsIntMode=mode;
  write8bit(VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO,*((uint8_t*)(&_configIntGPIOReg)));
  return true;
}
void DFRobot_VL6180X::DFRobot_VL6180X::setRangeThresholdValue(uint8_t thresholdL,uint8_t thresholdH)
{
  write8bit(VL6180X_SYSRANGE_THRESH_LOW,thresholdL);
  write8bit(VL6180X_SYSRANGE_THRESH_HIGH,thresholdH);
}
void DFRobot_VL6180X::rangeStartContinuousMode()
{
  stopMeasurement();
  _rangeStartReg.startstop = 1;
  _rangeStartReg.select = 1;
  write8bit(VL6180X_SYSRANGE_START,*((uint8_t*)(&_rangeStartReg)));
}
void DFRobot_VL6180X::rangeStopContinuousMode()
{
  _rangeStartReg.select = 0;
  write8bit(VL6180X_SYSRANGE_START,*((uint8_t*)(&_rangeStartReg)));
}
uint8_t DFRobot_VL6180X::rangeGetMeasurement()
{
  uint8_t value = read(VL6180X_RESULT_RANGE_VAL,1);
  return value;
}
void DFRobot_VL6180X::clearAlsInterrupt(){
  _clearIntReg.intClearSig = 2;
  write8bit(VL6180X_SYSTEM_INTERRUPT_CLEAR,*((uint8_t*)(&_clearIntReg)));
}
void DFRobot_VL6180X::clearRangeInterrupt(){
  _clearIntReg.intClearSig = 1;
  write8bit(VL6180X_SYSTEM_INTERRUPT_CLEAR,*((uint8_t*)(&_clearIntReg)));
}

float DFRobot_VL6180X::alsPoLLMeasurement()
{
  _ALSStartReg.startstop = 1;
  _ALSStartReg.select = 0;
  write8bit(VL6180X_SYSALS_START,*((uint8_t*)(&_ALSStartReg)));
  return alsGetMeasurement();
}
void DFRobot_VL6180X::setALSThresholdValue(uint16_t thresholdL,uint16_t thresholdH)
{
  float valueL = (thresholdL * _gain)/0.32;
  float valueH = (thresholdH * _gain)/0.32;
  write16bit(VL6180X_SYSALS_THRESH_LOW,(uint16_t)valueL);
  write16bit(VL6180X_SYSALS_THRESH_HIGH,(uint16_t)valueH);
}

float DFRobot_VL6180X::alsGetMeasurement()
{
  float value = read(VL6180X_RESULT_ALS_VAL,2);
  value  = (0.32*100*value)/(_gain*_atime);
  return value;
}

void DFRobot_VL6180X::alsStartContinuousMode()
{
  stopMeasurement();
  _ALSStartReg.startstop = 1;
  _ALSStartReg.select = 1;
  write8bit(VL6180X_SYSALS_START,*((uint8_t*)(&_ALSStartReg)));
}

void DFRobot_VL6180X::alsStopContinuousMode()
{
  _ALSStartReg.select = 0;
  write8bit(VL6180X_SYSALS_START,*((uint8_t*)(&_ALSStartReg)));
}
void DFRobot_VL6180X :: stopMeasurement()
{
  _ALSStartReg.startstop = 1;
  _ALSStartReg.select = 0;
  write8bit(VL6180X_SYSALS_START,*((uint8_t*)(&_ALSStartReg)));
  _rangeStartReg.startstop = 1;
  _rangeStartReg.select = 0;
  write8bit(VL6180X_SYSRANGE_START,*((uint8_t*)(&_rangeStartReg)));
  write8bit(VL6180X_INTERLEAVED_MODE_ENABLE,0x00);
  delay(1000);
  write8bit(VL6180X_SYSTEM_INTERRUPT_CLEAR,7);
}
void DFRobot_VL6180X::alsSetInterMeasurementPeriod(uint16_t periodMs)
{
  if(periodMs>10){
    if(periodMs<2550){
      periodMs = (periodMs/10) - 1;
    }else{
      periodMs = 254;
    }
  }
  write8bit(VL6180X_SYSALS_INTERMEASUREMENT_PERIOD, periodMs);
}
void DFRobot_VL6180X::startInterleavedMode()
{
  stopMeasurement();
  write8bit(VL6180X_INTERLEAVED_MODE_ENABLE,0x01);
  _ALSStartReg.startstop = 1;
  _ALSStartReg.select = 1;
  write8bit(VL6180X_SYSALS_START,*((uint8_t*)(&_ALSStartReg)));
}

uint8_t DFRobot_VL6180X::rangeGetInterruptStatus()
{
  return (read(VL6180X_RESULT_INTERRUPT_STATUS_GPIO,1)&0x07);
}

uint8_t DFRobot_VL6180X::alsGetInterruptStatus()
{
  return ((read(VL6180X_RESULT_INTERRUPT_STATUS_GPIO,1)>>3)&0x07);
}

void DFRobot_VL6180X::stopInterleavedMode()
{
  _ALSStartReg.startstop = 1;
  _ALSStartReg.select = 0;
  write8bit(VL6180X_SYSALS_START,*((uint8_t*)(&_ALSStartReg)));
  write8bit(VL6180X_INTERLEAVED_MODE_ENABLE,0x00);
}
bool DFRobot_VL6180X::setALSGain(uint8_t gain)
{
  if(gain>7){
    return false;
  }else{
    _analogueGainReg.gain = gain;
    switch(gain){
    case VL6180X_ALS_GAIN_20:
       _gain = 20;
       break;
    case VL6180X_ALS_GAIN_10:
       _gain = 10;
       break;
    case VL6180X_ALS_GAIN_5:
       _gain = 5.0;
       break;
    case VL6180X_ALS_GAIN_2_5:
       _gain = 2.5;
       break;
    case VL6180X_ALS_GAIN_1_67:
       _gain = 1.67;
       break;
    case VL6180X_ALS_GAIN_1_25:
       _gain = 1.25;
       break;
    case VL6180X_ALS_GAIN_1:
       _gain = 1.0;
       break;
    case VL6180X_ALS_GAIN_40:
       _gain = 40;
       break;
  }
    write8bit(VL6180X_SYSALS_ANALOGUE_GAIN,*((uint8_t*)(&_analogueGainReg)));
  }
  return true;
}
uint8_t DFRobot_VL6180X::getDeviceID()
{
  return (uint8_t)read(VL6180X_IDENTIFICATION_MODEL_ID,1);
}
uint8_t DFRobot_VL6180X::getRangeResult()
{
  return read(VL6180X_RESULT_RANGE_STATUS,1)>>4;
}
void DFRobot_VL6180X::setIICAddr(uint8_t addr)
{
  write8bit(VL6180X_I2C_SLAVE_DEVICE_ADDRESS,addr);
  _deviceAddr = addr;
}

void DFRobot_VL6180X:: write8bit(uint16_t regAddr,uint8_t value)
{
  _pWire->beginTransmission(_deviceAddr);
  _pWire->write(regAddr>>8);
  _pWire->write(regAddr);
  _pWire->write(value);
  _pWire->endTransmission();
}

void DFRobot_VL6180X:: write16bit(uint16_t regAddr,uint16_t value)
{
  _pWire->beginTransmission(_deviceAddr);
  _pWire->write(regAddr>>8);
  _pWire->write(regAddr);
  _pWire->write(value>>8);
  _pWire->write(value);
  _pWire->endTransmission();
}

uint16_t DFRobot_VL6180X:: read(uint16_t regAddr,uint8_t readNum)
{
  uint16_t value=0;
  uint8_t  a ,b;
  _pWire->beginTransmission(_deviceAddr);
  _pWire->write(regAddr>>8);
  _pWire->write(regAddr&0xFF);
  _pWire->endTransmission();
  _pWire->requestFrom(_deviceAddr, readNum);
  if(readNum==1){
    value = _pWire->read();
  }else if(readNum == 2){
    b = _pWire->read();
    a = _pWire->read();
    value = (b<<8)|a;
  }
  return value;
}












