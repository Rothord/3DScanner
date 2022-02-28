/*!
 * @file DFRobot_VL6180X.h
 * @brief Define the infrastructure for the DFRobot_VL6180X class
 * @n This is a DFRobot_VL6180X sensor that supports IIC communication. The IIC address can be changed, default to 0x29. The functions are as follows:
 * @n Measures absolute range from 0 to above 10 cm 
 * @n Measurement of ambient light data
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @SKU SEN0427
 * @licence The MIT License (MIT)
 * @author [yangfeng]<feng.yang@dfrobot.com>
 * @version V1.0
 * @date 2021-02-08
 * @url  https://github.com/DFRobot/DFRobot_VL6180X
 */
#ifndef _DFROBOT_VL6180X_H_
#define _DFROBOT_VL6180X_H_

#include <Arduino.h>
#include <Wire.h>
#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

/*I2C ADDRESS*/
#define VL6180X_IIC_ADDRESS          0x29

/*---------------------Register Address-------------------*/
//Device model identification number：0XB4
#define VL6180X_IDENTIFICATION_MODEL_ID               0x000

#define VL6180X_SYSTEM_MODE_GPIO0                     0X010
#define VL6180X_SYSTEM_MODE_GPIO1                     0X011
/*
   * SYSTEM__MODE_GPIO1
   * -------------------------------------------------------------------------------------------------
   * |    b7    |    b6    |       b5        |    b4    |    b3    |    b2    |    b1     |    b0    |
   * -------------------------------------------------------------------------------------------------
   * |      reversed       |     polarity    |                   select                   | reversed |
   * -------------------------------------------------------------------------------------------------
   *
   *
*/
typedef struct {
  uint8_t   reversed: 1; /* Reserved. Write 0.*/
  uint8_t   select: 4; /* Functional configuration options [bit:4-1].
                          0000: OFF (Hi-Z)
                          1000: GPIO Interrupt output
                      */
  uint8_t   polarity: 1; /* Signal Polarity Selection.[bit:5]
                            0: Active-low
                            1: Active-high 
                        */
  uint8_t   reservedBit6_7: 2; /* Reserved. */
} __attribute__ ((packed)) sModeGpio1Reg_t;

#define VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO          0x014
/*
   * SYSTEM__INTERRUPT_CONFIG_GPIO
   * ---------------------------------------------------------------------------------------------
   * |    b7    |    b6    |       b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
   * ---------------------------------------------------------------------------------------------
   * |      reversed       |             als_int_mode          |         range_int_mode          |
   * ---------------------------------------------------------------------------------------------
*/
typedef struct {
  uint8_t   rangeIntMode: 3; /*  Interrupt mode source for ALS readings[bit:2-0]:
                                 0: Disabled
                                 1: Level Low (value < thresh_low)
                                 2: Level High (value > thresh_high)
                                 3: Out Of Window (value < thresh_low OR value > thresh_high)
                                 4: New sample ready
                            */
  uint8_t   alsIntMode: 3; /*  Interrupt mode source for Range readings[bit:5-3]:
                                 0: Disabled
                                 1: Level Low (value < thresh_low)
                                 2: Level High (value > thresh_high)
                                 3: Out Of Window (value < thresh_low OR value > thresh_high)
                                 4: New sample ready
                           */
  uint8_t   reservedBit6_7: 2; /* Reserved. */
} __attribute__ ((packed)) sConfigIntGPIOReg_t;

#define VL6180X_SYSTEM_INTERRUPT_CLEAR                0x015
/*
   * SYSTEM__INTERRUPT_CLEAR
   * ---------------------------------------------------------------------------------------------
   * |    b7    |    b6    |       b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
   * ---------------------------------------------------------------------------------------------
   * |                         reversed                        |          int_clear_sig          |
   * ---------------------------------------------------------------------------------------------
   *
   *
*/
typedef struct {
  uint8_t   intClearSig: 3; /*  Interrupt clear bits. Writing a 1 to each bit will clear the intended interrupt.[bit:2-0]:
                                Bit [0] - Clear Range Int 
                                Bit [1] - Clear ALS Int 
                                Bit [2] - Clear Error Int.
                            */
  uint8_t   reserved: 5; /* Reserved. */
} __attribute__ ((packed)) sClearIntReg_t;

#define VL6180X_SYSTEM_FRESH_OUT_OF_RESET             0x016

#define VL6180X_SYSTEM_GROUPED_PARAMETER_HOLD         0x017

#define VL6180X_SYSRANGE_START                        0x018
/*
   * SYSRANGE__START
   * -----------------------------------------------------------------------------------------------
   * |    b7    |    b6    |       b5    |    b4    |    b3    |    b2    |    b1     |     b0     |
   * -----------------------------------------------------------------------------------------------
   * |                             reversed                               |   select  | startstop  |
   * -----------------------------------------------------------------------------------------------
   *
   *
*/
typedef struct {
  uint8_t   startstop: 1; /*  StartStop trigger based on current mode and system configuration of device_ready. FW clears register automatically[bit:0]:
                              Setting this bit to 1 in single-shot mode starts a single measurement 
                              Setting this bit to 1 in continuous mode will either start continuous operation (if stopped) or halt continuous operation (if started).
                          */
  uint8_t   select: 1; /*  Device Mode select
                           0: Ranging Mode Single-Shot
                           1: Ranging Mode Continuous 
                       */
  uint8_t   reserved: 6; /* Reserved. */
} __attribute__ ((packed)) sRangeStartReg_t;

// High Threshold value for ranging comparison. Range 0-255mm.
#define VL6180X_SYSRANGE_THRESH_HIGH                  0x019

//Low Threshold value for ranging comparison. Range 0-255mm.
#define VL6180X_SYSRANGE_THRESH_LOW                   0x01A

// Time delay between measurements in Ranging continuous mode. Range 0-254 (0 = 10ms). Step size = 10ms.
#define VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD      0x01B

//Maximum time to run measurement in Ranging modes.Range 1 - 63 ms
#define VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME         0x01C

#define VL6180X_SYSRANGE_EARLY_CONVERGENCE_ESTIMATE   0x022
#define VL6180X_SYSRANGE_MAX_AMBIENT_LEVEL_MULT       0x02C
#define VL6180X_SYSRANGE_RANGE_CHECK_ENABLES          0x02D
#define VL6180X_SYSRANGE_VHV_RECALIBRATE              0x02E
#define VL6180X_SYSRANGE_VHV_REPEAT_RATE              0x031

#define VL6180X_SYSALS_START                          0x038
/*
   * SYSALS__START
   * -----------------------------------------------------------------------------------------------
   * |    b7    |    b6    |       b5    |    b4    |    b3    |    b2    |    b1     |     b0     |
   * -----------------------------------------------------------------------------------------------
   * |                             reversed                               |   select  | startstop  |
   * -----------------------------------------------------------------------------------------------
   *
   *
*/
typedef struct {
  uint8_t   startstop: 1; /*  Start/Stop trigger based on current mode and system configuration of device_ready. FW clears register automatically.[bit:0]:
                              Setting this bit to 1 in single-shot mode starts a single measurement 
                              Setting this bit to 1 in continuous mode will either start continuous operation (if stopped) or halt continuous operation (if started).
                          */
  uint8_t   select: 1; /*  Device Mode select
                           0: ALS Mode Single-Shot
                           1: ALS Mode Continuous
                       */
  uint8_t   reserved: 6; /* Reserved. */
} __attribute__ ((packed)) sALSStartReg_t;
//High Threshold value for ALS comparison. Range 0-65535 codes.
#define VL6180X_SYSALS_THRESH_HIGH                    0x03A
// Low Threshold value for ALS comparison. Range 0-65535 codes.
#define VL6180X_SYSALS_THRESH_LOW                     0x03C
//Time delay between measurements in ALS continuous mode. Range 0-254 (0 = 10ms). Step size = 10ms.
#define VL6180X_SYSALS_INTERMEASUREMENT_PERIOD        0x03E

#define VL6180X_SYSALS_ANALOGUE_GAIN                  0x03F
/*
   * SYSALS_ANALOGUE_GAIN
   * ---------------------------------------------------------------------------------------------
   * |    b7    |    b6    |       b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
   * ---------------------------------------------------------------------------------------------
   * |                         reversed                        |   sysals__analogue_gain_light   |
   * ---------------------------------------------------------------------------------------------
   *
   *
*/
typedef struct {
  uint8_t   gain: 3; /*  ALS analogue gain[bit:2-0]:
                         0: ALS Gain = 20
                         1: ALS Gain = 10
                         2: ALS Gain = 5.0
                         3: ALS Gain = 2.5
                         4: ALS Gain = 1.67
                         5: ALS Gain = 1.25
                         6: ALS Gain = 1.0
                         7: ALS Gain = 40
                     */
  uint8_t   reserved: 5; /* Reserved. */
} __attribute__ ((packed)) sAnalogueGainReg_t;

// Integration period for ALS mode. 1 code = 1 ms (0 = 1 ms). Recommended setting is 100 ms (0x63).
#define VL6180X_SYSALS_INTEGRATION_PERIOD             0x040

#define VL6180X_RESULT_RANGE_STATUS                   0x04D
#define VL6180X_RESULT_ALS_STATUS                     0x04E
#define VL6180X_RESULT_INTERRUPT_STATUS_GPIO          0x04F
#define VL6180X_RESULT_ALS_VAL                        0x050
#define VL6180X_RESULT_RANGE_VAL                      0x062



#define VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD       0x10A


#define VL6180X_FIRMWARE_RESULT_SCALER                0x120
#define VL6180X_I2C_SLAVE_DEVICE_ADDRESS              0x212
#define VL6180X_INTERLEAVED_MODE_ENABLE               0x2A3

#define VL6180X_ID                                    0xB4
#define VL6180X_ALS_GAIN_20                           0x00
#define VL6180X_ALS_GAIN_10                           0x01
#define VL6180X_ALS_GAIN_5                            0x02
#define VL6180X_ALS_GAIN_2_5                          0x03
#define VL6180X_ALS_GAIN_1_67                         0x04
#define VL6180X_ALS_GAIN_1_25                         0x05
#define VL6180X_ALS_GAIN_1                            0x06
#define VL6180X_ALS_GAIN_40                           0x07

#define VL6180X_NO_ERR                                0x00
#define VL6180X_EARLY_CONV_ERR                        0x06
#define VL6180X_MAX_CONV_ERR                          0x07
#define VL6180X_IGNORE_ERR                            0x08
#define VL6180X_MAX_S_N_ERR                           0x0B
#define VL6180X_RAW_Range_UNDERFLOW_ERR               0x0C
#define VL6180X_RAW_Range_OVERFLOW_ERR                0x0D
#define VL6180X_Range_UNDERFLOW_ERR                   0x0E
#define VL6180X_Range_OVERFLOW_ERR                    0x0F

#define VL6180X_DIS_INTERRUPT        0
#define VL6180X_HIGH_INTERRUPT       1
#define VL6180X_LOW_INTERRUPT        2

#define VL6180X_INT_DISABLE          0
#define VL6180X_LEVEL_LOW            1
#define VL6180X_LEVEL_HIGH           2
#define VL6180X_OUT_OF_WINDOW        3
#define VL6180X_NEW_SAMPLE_READY     4
class DFRobot_VL6180X{
  
public:
  /**
   * @brief  constructed function
   * @param  pWire  When instantiate this class, you can specify its twowire
   */
  DFRobot_VL6180X(uint8_t iiicAddr = VL6180X_IIC_ADDRESS,TwoWire *pWire=&Wire);

  /**
   * @brief  Destructor
   */
  ~DFRobot_VL6180X(){
  };

  /**
   * @brief  Initialization function
   * @return Whether the device is on or not. return true succeed ;return false failed.
   */
  bool begin();

  /**
   * @brief  Configure the default level of the INT pin and enable the GPIO1 interrupt function
   * @param  mode  Enable interrupt mode
   * @n            VL6180X_DIS_INTERRUPT  disabled interrupt
   * @n            VL6180X_DIS_INTERRUPT  GPIO1 interrupt enabled, INT high by default
   * @n            VL6180X_LOW_INTERRUPT  GPIO1 interrupt enabled, INT low by default
   */
  void setInterrupt(uint8_t mode);

  /**
   * @brief  A single range
   * @return   return ranging data ,uint mm
   */
  uint8_t rangePollMeasurement();

  /**
   * @brief  Configuration ranging period
   * @param  period_ms  Measurement period, in milliseconds
   */
  void rangeSetInterMeasurementPeriod(uint16_t periodMs);

  /**
   * @brief  Configure the interrupt mode for ranging
   * @param  mode  Enable interrupt mode
   * @n              VL6180X_INT_DISABLE                           interrupt disable                   
   * @n              VL6180X_LEVEL_LOW                             value < thresh_low                      
   * @n              VL6180X_LEVEL_HIGH                            value > thresh_high                      
   * @n              VL6180X_OUT_OF_WINDOW                         value < thresh_low OR value > thresh_high
   * @n              VL6180X_NEW_SAMPLE_READY                      new sample ready                      
   */
  bool rangeConfigInterrupt(uint8_t mode);

  /**
   * @brief  Configure the interrupt mode for the ambient light
   * @param  mode  Enable interrupt mode
   * @n              VL6180X_INT_DISABLE                           interrupt disable                   
   * @n              VL6180X_LEVEL_LOW                             value < thresh_low                      
   * @n              VL6180X_LEVEL_HIGH                            value > thresh_high                      
   * @n              VL6180X_OUT_OF_WINDOW                         value < thresh_low OR value > thresh_high
   * @n              VL6180X_NEW_SAMPLE_READY                      new sample ready                      
   */
  bool alsConfigInterrupt(uint8_t mode);

  /**
   * @brief Enable continuous ranging mode
   */
  void rangeStartContinuousMode();
  
  /**
   * @brief  Retrieve ranging data
   * @return   return ranging data ,uint mm
   */
  uint8_t rangeGetMeasurement();

  /**
   * @brief  Clear the ambient light interrupt
   */
  void clearAlsInterrupt();

  /**
   * @brief  Clear ranging interrupt
   */
  void clearRangeInterrupt();

  /**
   * @brief Single measurement of ambient light
   * @return   return The light intensity,uint lux
   */
  float alsPoLLMeasurement();

  /**
   * @brief  Obtain measured light data
   * @return   return The light intensity,uint lux
   */
  float alsGetMeasurement();

  /**
   * @brief  Enable continuous measurement of ambient light intensity mode
   */
  void alsStartContinuousMode();

  /**
   * @brief  Configure the period for measuring light intensity
   * @param  period_ms  Measurement period, in milliseconds
   */
  void alsSetInterMeasurementPeriod(uint16_t periodMs);

  /**
   * @brief  turn on interleaved mode
   */
  void startInterleavedMode();

  /**
   * @brief  Get the interrupt state of the ranging
   * @return   return status
   * @n             0                          No threshold events reported
   * @n             VL6180X_LEVEL_LOW         value < thresh_low
   * @n             VL6180X_LEVEL_HIGH        value > thresh_high
   * @n             VL6180X_OUT_OF_WINDOW     value < thresh_low OR value > thresh_high
   * @n             VL6180X_NEW_SAMPLE_READY  new sample ready
   */
  uint8_t rangeGetInterruptStatus();

  /**
   * @brief  Get the interrupt state of the measured light intensity
   * @return   return status
   * @n             0                          No threshold events reported
   * @n             VL6180X_LEVEL_LOW         value < thresh_low
   * @n             VL6180X_LEVEL_HIGH        value > thresh_high
   * @n             VL6180X_OUT_OF_WINDOW     value < thresh_low OR value > thresh_high
   * @n             VL6180X_NEW_SAMPLE_READY  new sample ready
   */
  uint8_t alsGetInterruptStatus();

  /**
   * @brief  Get validation information for range data
   * @return Authentication information
   */
  uint8_t getRangeResult();

  /**
   * @brief  set IIC addr
   * @param  addr  The IIC address to be modified
   */
  void setIICAddr(uint8_t addr);

  /**
   * @brief  Set the ALS gain 
   * @param  gain  the value of gain(range 0-7)
   * @n            20   times gain   VL6180X_ALS_GAIN_20        0
   * @n            10   times gain   VL6180X_ALS_GAIN_10        1
   * @n            5    times gain   VL6180X_ALS_GAIN_5         2
   * @n            2.5  times gain   VL6180X_ALS_GAIN_2_5       3
   * @n            1.57 times gain   VL6180X_ALS_GAIN_1_67      4
   * @n            1.27 times gain   VL6180X_ALS_GAIN_1_25      5
   * @n            1    times gain   VL6180X_ALS_GAIN_1         6
   * @n            40   times gain   VL6180X_ALS_GAIN_40        7
   * @return true :Set up the success, false :Setup faile
   */
  bool setALSGain(uint8_t gain = VL6180X_ALS_GAIN_1);

  /**
   * @brief  Set ALS Threshold Value
   * @param  thresholdL Lower Threshold
   * @param  thresholdH Upper threshold
   */
  void setALSThresholdValue(uint16_t thresholdL=0x0000,uint16_t thresholdH=0xFFFF);

  /**
   * @brief  Set Range Threshold Value
   * @param  thresholdL Lower Threshold
   * @param  thresholdH Upper threshold
   */
  void setRangeThresholdValue(uint8_t thresholdL=0x00,uint8_t thresholdH=0xFF);

private:

  /**
   * @brief  Initialize the sensor configuration
   */
  void init();

  /**
   * @brief  Gets validation information for ALS data
   * @return Authentication information
   */
  uint8_t getALSResult();

  /**
   * @brief  get the identifier of sensor
   * @return Authentication information
   */
  uint8_t getDeviceID();

  /**
   * @brief disable continuous ranging mode
   */
  void rangeStopContinuousMode();

  /**
   * @brief  disable continuous measurement of ambient light intensity mode
   */
  void alsStopContinuousMode();
  
  /**
   * @brief  @brief  turn off interleaved mode
   */
  void stopInterleavedMode();

  /**
   * @brief  @brief  stop measuremwnt
   */
  void stopMeasurement();
  /**
   * @brief  config register(8 byte)
   * @param  regAddr  register address
   * @param  value  Writes the value of the register
   */
  void write8bit(uint16_t regAddr,uint8_t value);

  /**
   * @brief  config register(16 byte)
   * @param  regAddr  register address
   * @param  value  Writes the value of the register
   */
  void write16bit(uint16_t regAddr,uint16_t value);

  /**
   * @brief  read register
   * @param  regAddr  register address
   * @param  readNum  Number of bytes read
   * @return Read data from a register
   */
  uint16_t read(uint16_t regAddr,uint8_t readNum);

private:
  TwoWire *_pWire;
  sModeGpio1Reg_t      _modeGpio1Reg;
  sConfigIntGPIOReg_t  _configIntGPIOReg;
  sClearIntReg_t       _clearIntReg;
  sRangeStartReg_t     _rangeStartReg;
  sALSStartReg_t       _ALSStartReg;
  sAnalogueGainReg_t   _analogueGainReg;
  float _gain;
  uint8_t _atime;
  uint8_t _deviceAddr;
  bool _continuousRangeMode;
  bool _continuousALSMode;
};

#endif
