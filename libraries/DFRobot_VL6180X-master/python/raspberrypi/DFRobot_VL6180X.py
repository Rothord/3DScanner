# -*- coding: utf-8 -*
""" file DFRobot_VL6180X.py
  # DFRobot_VL6180X Class infrastructure, implementation of underlying methods
  @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @licence     The MIT License (MIT)
  @author      [yangfeng]<feng.yang@dfrobot.com> 
  @version  V1.0
  @date  2021-02-09
  @get from https://www.dfrobot.com
  @url https://github.com/DFRobot/DFRobot_VL6180X
"""
import smbus
import time
import RPi.GPIO as GPIO
class DFRobot_VL6180X:
  # IIC ADDR
  VL6180X_IIC_ADDRESS                          = 0x29

  # The sensor register address
  VL6180X_IDENTIFICATION_MODEL_ID             = 0x000
  VL6180X_SYSTEM_MODE_GPIO0                   = 0X010
  VL6180X_SYSTEM_MODE_GPIO1                   = 0X011
  VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO        = 0x014
  VL6180X_SYSTEM_INTERRUPT_CLEAR              = 0x015
  VL6180X_SYSTEM_FRESH_OUT_OF_RESET           = 0x016
  VL6180X_SYSTEM_GROUPED_PARAMETER_HOLD       = 0x017
  VL6180X_SYSRANGE_START                      = 0x018
  VL6180X_SYSRANGE_THRESH_HIGH                = 0x019
  VL6180X_SYSRANGE_THRESH_LOW                 = 0x01A
  VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD    = 0x01B
  VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME       = 0x01C
  VL6180X_SYSRANGE_EARLY_CONVERGENCE_ESTIMATE = 0x022
  VL6180X_SYSRANGE_MAX_AMBIENT_LEVEL_MULT     = 0x02C
  VL6180X_SYSRANGE_RANGE_CHECK_ENABLES        = 0x02D
  VL6180X_SYSRANGE_VHV_RECALIBRATE            = 0x02E
  VL6180X_SYSRANGE_VHV_REPEAT_RATE            = 0x031
  VL6180X_SYSALS_START                        = 0x038
  VL6180X_SYSALS_THRESH_HIGH                  = 0x03A
  VL6180X_SYSALS_THRESH_LOW                   = 0x03C
  VL6180X_SYSALS_INTERMEASUREMENT_PERIOD      = 0x03E
  VL6180X_SYSALS_ANALOGUE_GAIN                = 0x03F
  VL6180X_SYSALS_INTEGRATION_PERIOD           = 0x040
  VL6180X_RESULT_RANGE_STATUS                 = 0x04D
  VL6180X_RESULT_ALS_STATUS                   = 0x04E
  VL6180X_RESULT_INTERRUPT_STATUS_GPIO        = 0x04F
  VL6180X_RESULT_ALS_VAL                      = 0x050
  VL6180X_RESULT_RANGE_VAL                    = 0x062
  VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD     = 0x10A
  VL6180X_FIRMWARE_RESULT_SCALER              = 0x120
  VL6180X_I2C_SLAVE_DEVICE_ADDRESS            = 0x212
  VL6180X_INTERLEAVED_MODE_ENABLE             = 0x2A3
  
  # The valid ID of the sensor
  VL6180X_ID                                  = 0xB4
  # 8 gain modes for ambient light
  VL6180X_ALS_GAIN_20                         = 0
  VL6180X_ALS_GAIN_10                         = 1
  VL6180X_ALS_GAIN_5                          = 2
  VL6180X_ALS_GAIN_2_5                        = 3
  VL6180X_ALS_GAIN_1_67                       = 4
  VL6180X_ALS_GAIN_1_25                       = 5
  VL6180X_ALS_GAIN_1                          = 6
  VL6180X_ALS_GAIN_40                         = 7
  
  # The result of the range measurenments
  VL6180X_NO_ERR                              = 0x00
  VL6180X_ALS_OVERFLOW_ERR                    = 0x01
  VL6180X_ALS_UNDERFLOW_ERR                   = 0x02
  VL6180X_NO_ERR                              = 0x00
  VL6180X_EARLY_CONV_ERR                      = 0x06
  VL6180X_MAX_CONV_ERR                        = 0x07
  VL6180X_IGNORE_ERR                          = 0x08
  VL6180X_MAX_S_N_ERR                         = 0x0B
  VL6180X_RAW_Range_UNDERFLOW_ERR             = 0x0C
  VL6180X_RAW_Range_OVERFLOW_ERR              = 0x0D
  VL6180X_Range_UNDERFLOW_ERR                 = 0x0E
  VL6180X_Range_OVERFLOW_ERR                  = 0x0F
  
  # GPIO1 mode selection
  VL6180X_DIS_INTERRUPT                       = 0
  VL6180X_HIGH_INTERRUPT                      = 1
  VL6180X_LOW_INTERRUPT                       = 2

  # als/range interrupt mode selection
  VL6180X_INT_DISABLE                         = 0
  VL6180X_LEVEL_LOW                           = 1
  VL6180X_LEVEL_HIGH                          = 2
  VL6180X_OUT_OF_WINDOW                       = 3
  VL6180X_NEW_SAMPLE_READY                    = 4

  ''' 
    @brief  Module init
    @param  bus  Set to IICBus
    @param  addr  Set to IIC addr

  '''
  def __init__(self,iic_addr =VL6180X_IIC_ADDRESS,bus = 1):
    self.__i2cbus = smbus.SMBus(bus)
    self.__i2c_addr = iic_addr
    self.__gain = 1.0
    self.__atime =100

  ''' 
    @brief  Initialize sensor
    @param  CE  The pin number attached to the CE
    @return   return True succeed ;return False failed.

  '''
  def begin(self):
    device_id = self.__get_device_id()
    if device_id != self.VL6180X_ID:
      return False
    self.__init()
    return True 

  ''' 
    @brief  Configure the default level of the INT pin and enable the GPIO1 interrupt function
    @param  mode  Enable interrupt mode
    @n            VL6180X_DIS_INTERRUPT  disabled interrupt
    @n            VL6180X_DIS_INTERRUPT  GPIO1 interrupt enabled, INT high by default
    @n            VL6180X_LOW_INTERRUPT  GPIO1 interrupt enabled, INT low by default

  '''
  def set_interrupt(self,mode):
    if(mode == self.VL6180X_DIS_INTERRUPT):
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_MODE_GPIO1>>8, [self.VL6180X_SYSTEM_MODE_GPIO1,0x20])
    elif(mode == self.VL6180X_HIGH_INTERRUPT):
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_MODE_GPIO1>>8, [self.VL6180X_SYSTEM_MODE_GPIO1,0x10])
    elif(mode == self.VL6180X_LOW_INTERRUPT):
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_MODE_GPIO1>>8, [self.VL6180X_SYSTEM_MODE_GPIO1,0x30])

  ''' 
    @brief  A single range
    @return   return ranging data ,uint mm

  '''
  def range_poll_measurement(self):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSRANGE_START>>8, [self.VL6180X_SYSRANGE_START,0x01])
    return self.range_get_measurement()

  ''' 
    @brief  Configuration ranging period
    @param  period_ms  Measurement period, in milliseconds

  '''
  def range_set_inter_measurement_period(self,period_ms):
    if(period_ms > 10):
      if(period_ms < 2550):
        period_ms = ( period_ms / 10 ) -1
      else:
        period_ms = 254
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD>>8, [self.VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD,period_ms])
  
  ''' 
    @brief  Configure the interrupt mode for ranging
    @param  mode  Enable interrupt mode
    @n              VL6180X_INT_DISABLE                           interrupt disable                   
    @n              VL6180X_LEVEL_LOW                             value < thresh_low                      
    @n              VL6180X_LEVEL_HIGH                            value > thresh_high                      
    @n              VL6180X_OUT_OF_WINDOW                         value < thresh_low OR value > thresh_high
    @n              VL6180X_NEW_SAMPLE_READY                      new sample ready                      

  '''
  def range_config_interrupt(self,mode):
    if(mode > self.VL6180X_NEW_SAMPLE_READY):
      return False
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO>>8, [self.VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO])
    value = self.__i2cbus.read_byte(self.__i2c_addr)
    value = value | mode 
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO>>8, [self.VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO,value])
  
  ''' 
    @brief  Configure the interrupt mode for the ambient light
    @param  mode  Enable interrupt mode
    @n              VL6180X_INT_DISABLE                           interrupt disable                   
    @n              VL6180X_LEVEL_LOW                             value < thresh_low                      
    @n              VL6180X_LEVEL_HIGH                            value > thresh_high                      
    @n              VL6180X_OUT_OF_WINDOW                         value < thresh_low OR value > thresh_high
    @n              VL6180X_NEW_SAMPLE_READY                      new sample ready                      

  '''
  def als_config_interrupt(self,mode):
    if(mode > self.VL6180X_NEW_SAMPLE_READY):
      return False
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO>>8, [self.VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO])
    value = self.__i2cbus.read_byte(self.__i2c_addr)
    value = value | ( mode << 3 ) 
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO>>8, [self.VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO,value])

  ''' 
    @brief Enable continuous ranging mode

  '''
  def range_start_continuous_mode(self):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSRANGE_START>>8, [self.VL6180X_SYSRANGE_START,0x01])
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSALS_START>>8, [self.VL6180X_SYSALS_START,0x01])
    time.sleep(0.3);
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_INTERRUPT_CLEAR>>8, [self.VL6180X_SYSTEM_INTERRUPT_CLEAR,7])
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSRANGE_START>>8, [self.VL6180X_SYSRANGE_START,0x03])
  
  ''' 
    @brief  Retrieve ranging data
    @return   return ranging data ,uint mm

  '''
  def range_get_measurement(self):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_RESULT_RANGE_VAL>>8, [self.VL6180X_RESULT_RANGE_VAL])
    value = self.__i2cbus.read_byte(self.__i2c_addr)
    return value

  ''' 
    @brief  Clear ranging interrupt

  '''
  def clear_range_interrupt(self):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_INTERRUPT_CLEAR>>8, [self.VL6180X_SYSTEM_INTERRUPT_CLEAR,1])

  ''' 
    @brief  Clear the ambient light interrupt

  '''
  def clear_als_interrupt(self):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_INTERRUPT_CLEAR>>8, [self.VL6180X_SYSTEM_INTERRUPT_CLEAR,2])

  ''' 
    @brief Single measurement of ambient light
    @return   return The light intensity,uint lux

  '''
  def als_poll_measurement(self):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSALS_START>>8, [self.VL6180X_SYSALS_START,0x01])
    return self.als_get_measurement()
  
  ''' 
    @brief  Obtain measured light data
    @return   return The light intensity,uint lux

  '''
  def als_get_measurement(self):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_RESULT_ALS_VAL>>8, [self.VL6180X_RESULT_ALS_VAL])
    a = self.__i2cbus.read_byte(self.__i2c_addr)
    b = self.__i2cbus.read_byte(self.__i2c_addr)
    value = (a<<8) | b
    result  = ((0.32*100*value)/(self.__gain*self.__atime))
    return result

  ''' 
    @brief  Enable continuous measurement of ambient light intensity mode

  '''
  def als_start_continuous_mode(self):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSRANGE_START>>8, [self.VL6180X_SYSRANGE_START,0x01])
    time.sleep(1)
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_INTERRUPT_CLEAR>>8, [self.VL6180X_SYSTEM_INTERRUPT_CLEAR,7])
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSALS_START>>8, [self.VL6180X_SYSALS_START,0x03])

  ''' 
    @brief  Configure the period for measuring light intensity
    @param  period_ms  Measurement period, in milliseconds

  '''
  def als_set_inter_measurement_period(self,period_ms):
    if(period_ms>10):
      if(period_ms<2550):
        period_ms = (period_ms/10) -1
      else:
        period_ms = 254
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSALS_INTERMEASUREMENT_PERIOD>>8, [self.VL6180X_SYSALS_INTERMEASUREMENT_PERIOD,period_ms])
  ''' 
    @brief  turn on interleaved mode

  '''
  def start_interleaved_mode(self):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSRANGE_START>>8, [self.VL6180X_SYSRANGE_START,0x01])
    time.sleep(1)
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_INTERRUPT_CLEAR>>8, [self.VL6180X_SYSTEM_INTERRUPT_CLEAR,7])
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSALS_START>>8, [self.VL6180X_SYSALS_START,0x03])
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_INTERLEAVED_MODE_ENABLE>>8, [self.VL6180X_INTERLEAVED_MODE_ENABLE,0x01])

  ''' 
    @brief  Gets the interrupt state of the ranging
    @return   return status
    @n             0                        �?No threshold events reported
    @n             VL6180X_LEVEL_LOW        ：value < thresh_low
    @n             VL6180X_LEVEL_HIGH       ：value > thresh_high
    @n             VL6180X_OUT_OF_WINDOW    ：value < thresh_low OR value > thresh_high
    @n             VL6180X_NEW_SAMPLE_READY ：new sample ready

  '''
  def range_get_interrupt_status(self):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_RESULT_INTERRUPT_STATUS_GPIO>>8, [self.VL6180X_RESULT_INTERRUPT_STATUS_GPIO])
    result = self.__i2cbus.read_byte(self.__i2c_addr)
    result = result & 0x07
    return result
  ''' 
    @brief  Gets the interrupt state of the measured light intensity
    @return   return status
    @n             0                        �?No threshold events reported
    @n             VL6180X_LEVEL_LOW        ：value < thresh_low
    @n             VL6180X_LEVEL_HIGH       ：value > thresh_high
    @n             VL6180X_OUT_OF_WINDOW    ：value < thresh_low OR value > thresh_high
    @n             VL6180X_NEW_SAMPLE_READY ：new sample ready
    
  '''
  def als_get_interrupt_status(self):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_RESULT_INTERRUPT_STATUS_GPIO>>8, [self.VL6180X_RESULT_INTERRUPT_STATUS_GPIO])
    result = self.__i2cbus.read_byte(self.__i2c_addr)
    result = (result>>3) & 0x07
    return result
  ''' 
    @brief  turn off interleaved mode

  '''
  def __stop_interleave_mode(self):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSALS_START>>8, [self.VL6180X_SYSALS_START,0x01])
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_INTERLEAVED_MODE_ENABLE>>8, [self.VL6180X_INTERLEAVED_MODE_ENABLE,0x00])

  ''' 
    @brief  Gets validation information for range data
    @return Authentication information
  '''
  def get_range_result(self):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_RESULT_RANGE_STATUS>>8, [self.VL6180X_RESULT_RANGE_STATUS])
    result = self.__i2cbus.read_byte(self.__i2c_addr)>>4
    return result

  ''' 
    @brief  set IIC addr
    @param  addr  The IIC address to be modified
  '''
  def set_iic_addr(self,addr):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_I2C_SLAVE_DEVICE_ADDRESS>>8, [self.VL6180X_I2C_SLAVE_DEVICE_ADDRESS,addr])
    self.__i2c_addr = addr

  ''' 
    @brief  Initialize the sensor configuration
  '''
  def __init(self):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_FRESH_OUT_OF_RESET>>8, [self.VL6180X_SYSTEM_FRESH_OUT_OF_RESET])
    reset = self.__i2cbus.read_byte(self.__i2c_addr)
    if(reset):
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x02, [0x07,0x01])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x02, [0x08,0x01])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0x96,0x00])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0x97,0xfd])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xe3,0x00])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xe4,0x04])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xe5,0x02])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xe6,0x01])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xe7,0x03])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xf5,0x02])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xd9,0x05])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xdb,0xce])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xdc,0x03])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xdd,0xf8])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0x9f,0x00])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xa3,0x3c])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xb7,0x00])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xbb,0x3c])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xb2,0x09])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xca,0x09])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x01, [0x98,0x01])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x01, [0xb0,0x17])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x01, [0xad,0x00])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0xff,0x05])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x01, [0x00,0x05])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x01, [0x99,0x05])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x01, [0xa6,0x1b])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x01, [0xac,0x3e])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x01, [0xa7,0x1f])
      self.__i2cbus.write_i2c_block_data(self.__i2c_addr,0x00, [0x30,0x00])
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD>>8, [self.VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD,0x09])
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSRANGE_VHV_REPEAT_RATE>>8, [self.VL6180X_SYSRANGE_VHV_REPEAT_RATE,0xFF])    
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSRANGE_VHV_RECALIBRATE>>8, [self.VL6180X_SYSRANGE_VHV_RECALIBRATE,0x01])    
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME>>8, [self.VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME,0x31])    
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSALS_INTERMEASUREMENT_PERIOD>>8, [self.VL6180X_SYSALS_INTERMEASUREMENT_PERIOD,0x31])    
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSALS_INTEGRATION_PERIOD>>8, [self.VL6180X_SYSALS_INTEGRATION_PERIOD,0x63])    
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD>>8, [self.VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD,0x30])    
    self.set_als_gain(self.VL6180X_ALS_GAIN_1)
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_MODE_GPIO1>>8, [self.VL6180X_SYSTEM_MODE_GPIO1,0x20])    
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO>>8, [self.VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO,0x00])  
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSRANGE_START>>8, [self.VL6180X_SYSRANGE_START,0x00])
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSALS_START>>8, [self.VL6180X_SYSALS_START,0x00])
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_INTERLEAVED_MODE_ENABLE>>8, [self.VL6180X_INTERLEAVED_MODE_ENABLE,0x00])
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSTEM_FRESH_OUT_OF_RESET>>8, [self.VL6180X_SYSTEM_FRESH_OUT_OF_RESET,0])
    
  ''' 
    @brief  Set Range Threshold Value
    @param  thresholdL :Lower Threshold
    @param  thresholdH :Upper threshold

  '''
  def set_range_threshold_value(self,threshold_l,threshold_h):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSRANGE_THRESH_LOW>>8, [self.VL6180X_SYSRANGE_THRESH_LOW,threshold_l])
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSRANGE_THRESH_HIGH>>8, [self.VL6180X_SYSRANGE_THRESH_HIGH,threshold_h])

  ''' 
    @brief  Set ALS Threshold Value
    @param  thresholdL :Lower Threshold
    @param  thresholdH :Upper threshold

  '''
  def set_als_threshold_value(self,threshold_l,threshold_h):
    value_l = int((threshold_l * self.__gain)/0.32)
    value_h = int((threshold_h* self.__gain)/0.32)
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSALS_THRESH_LOW>>8, [self.VL6180X_SYSALS_THRESH_LOW,threshold_l>>8,value_l])
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSALS_THRESH_HIGH>>8, [self.VL6180X_SYSALS_THRESH_HIGH,threshold_h>>8,value_h])

  ''' 
    @brief  Set the ALS gain 
    @param  gain  the value of gain(range 0-7)
    @n            20   times gain: VL6180X_ALS_GAIN_20       = 0
    @n            10   times gain: VL6180X_ALS_GAIN_10       = 1
    @n            5    times gain: VL6180X_ALS_GAIN_5        = 2
    @n            2.5  times gain: VL6180X_ALS_GAIN_2_5      = 3
    @n            1.57 times gain: VL6180X_ALS_GAIN_1_67     = 4
    @n            1.27 times gain: VL6180X_ALS_GAIN_1_25     = 5
    @n            1    times gain: VL6180X_ALS_GAIN_1        = 6
    @n            40   times gain: VL6180X_ALS_GAIN_40       = 7
    @return true :Set up the success, false :Setup failed

  '''
  def set_als_gain(self,gain):
    if(gain>7):
      return False
    if(gain == self.VL6180X_ALS_GAIN_20):
      self.__gain = 20
    elif(gain == self.VL6180X_ALS_GAIN_10):
      self.__gain = 10
    elif(gain == self.VL6180X_ALS_GAIN_5):
      self.__gain = 5.0
    elif(gain == self.VL6180X_ALS_GAIN_2_5):
      self.__gain = 2.5
    elif(gain == self.VL6180X_ALS_GAIN_1_67):
      self.__gain = 1.67
    elif(gain == self.VL6180X_ALS_GAIN_1_25):
      self.__gain = 1.25
    elif(gain == self.VL6180X_ALS_GAIN_1):
      self.__gain = 1.0
    elif(gain == self.VL6180X_ALS_GAIN_40):
      self.__gain = 40
    gain =gain | 0x40
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_SYSALS_ANALOGUE_GAIN>>8, [self.VL6180X_SYSALS_ANALOGUE_GAIN,gain])
    return True

  ''' 
    @brief  get the identifier of sensor
    @return Authentication information

  '''
  def __get_device_id(self):
    self.__i2cbus.write_i2c_block_data(self.__i2c_addr,self.VL6180X_IDENTIFICATION_MODEL_ID>>8, [self.VL6180X_IDENTIFICATION_MODEL_ID])
    id = self.__i2cbus.read_byte(self.__i2c_addr)
    return id




