# DFRobot_VL6180X

- [English Version](./README.md)

VL6180X是意法半导体的一款ToF激光测距传感器，可测量5-100mm的距离，但在良好的环境下测量距离可达200mm。VL6180X搭载了意法半导体FlightSense ™技术，可以精确的测量距离，与目标反射率无关。同时VL6180X还搭载了环境光传感器，可以测量出环境光数据。

![产品实物图](../../resources/images/SEN0427.png)


## 产品链接(https://www.dfrobot.com.cn/goods-3122.html)
    SKU：SEN0427

## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性)
* [历史](#历史)
* [创作者](#创作者)


## 概述

* 测量距离范围5-100mm
* 环境光数据的测量

## 库安装

要使用这个库，首先将库下载到Raspberry Pi，然后打开例程文件夹。要执行一个例程demox.py，请在命令行中输入python demox.py。例如，要执行als_continuous_measurement_mode.py例程，你需要输入:
```
python als_continuous_measurement_mode.py
```

## 方法

```python
   ''' 
    @brief  Initialize sensor
    @param  CE  The pin number attached to the CE
    @return   return True succeed ;return False failed.

  '''
  def begin(self)

  ''' 
    @brief  Configure the default level of the INT pin and enable the GPIO1 interrupt function
    @param  mode  Enable interrupt mode
    @n            VL6180X_DIS_INTERRUPT  disabled interrupt
    @n            VL6180X_DIS_INTERRUPT  GPIO1 interrupt enabled, INT high by default
    @n            VL6180X_LOW_INTERRUPT  GPIO1 interrupt enabled, INT low by default

  '''
  def set_interrupt(self,mode)

  ''' 
    @brief  A single range
    @return   return ranging data ,uint mm

  '''
  def range_poll_measurement(self)

  ''' 
    @brief  Configuration ranging period
    @param  period_ms  Measurement period, in milliseconds

  '''
  def range_set_inter_measurement_period(self,period_ms)
  
  ''' 
    @brief  Configure the interrupt mode for ranging
    @param  mode  Enable interrupt mode
    @n              VL6180X_INT_DISABLE                           interrupt disable                   
    @n              VL6180X_LEVEL_LOW                             value < thresh_low                      
    @n              VL6180X_LEVEL_HIGH                            value > thresh_high                      
    @n              VL6180X_OUT_OF_WINDOW                         value < thresh_low OR value > thresh_high
    @n              VL6180X_NEW_SAMPLE_READY                      new sample ready                      

  '''
  def range_config_interrupt(self,mode)
  
  ''' 
    @brief  Configure the interrupt mode for the ambient light
    @param  mode  Enable interrupt mode
    @n              VL6180X_INT_DISABLE                           interrupt disable                   
    @n              VL6180X_LEVEL_LOW                             value < thresh_low                      
    @n              VL6180X_LEVEL_HIGH                            value > thresh_high                      
    @n              VL6180X_OUT_OF_WINDOW                         value < thresh_low OR value > thresh_high
    @n              VL6180X_NEW_SAMPLE_READY                      new sample ready                      

  '''
  def als_config_interrupt(self,mode)

  ''' 
    @brief Enable continuous ranging mode

  '''
  def range_start_continuous_mode(self)  

  ''' 
    @brief  Retrieve ranging data
    @return   return ranging data ,uint mm

  '''
  def range_get_measurement(self)

  ''' 
    @brief  Clear ranging interrupt

  '''
  def clear_range_interrupt(self)

  ''' 
    @brief  Clear the ambient light interrupt

  '''
  def clear_als_interrupt(self)

  ''' 
    @brief Single measurement of ambient light
    @return   return The light intensity,uint lux

  '''
  def als_poll_measurement(self)

  
  ''' 
    @brief  Obtain measured light data
    @return   return The light intensity,uint lux

  '''
  def als_get_measurement(self)


  ''' 
    @brief  Enable continuous measurement of ambient light intensity mode

  '''
  def als_start_continuous_mode(self)
    
  ''' 
    @brief  Configure the period for measuring light intensity
    @param  period_ms  Measurement period, in milliseconds

  '''
  def als_set_inter_measurement_period(self,period_ms)

  ''' 
    @brief  turn on interleaved mode

  '''
  def start_interleaved_mode(self)


  ''' 
    @brief  Gets the interrupt state of the ranging
    @return   return status
    @n             0                        ： No threshold events reported
    @n             VL6180X_LEVEL_LOW        ：value < thresh_low
    @n             VL6180X_LEVEL_HIGH       ：value > thresh_high
    @n             VL6180X_OUT_OF_WINDOW    ：value < thresh_low OR value > thresh_high
    @n             VL6180X_NEW_SAMPLE_READY ：new sample ready

  '''
  def range_get_interrupt_status(self)

  ''' 
    @brief  Gets the interrupt state of the measured light intensity
    @return   return status
    @n             0                        ： No threshold events reported
    @n             VL6180X_LEVEL_LOW        ：value < thresh_low
    @n             VL6180X_LEVEL_HIGH       ：value > thresh_high
    @n             VL6180X_OUT_OF_WINDOW    ：value < thresh_low OR value > thresh_high
    @n             VL6180X_NEW_SAMPLE_READY ：new sample ready
    
  '''
  def als_get_interrupt_status(self)



  ''' 
    @brief  Gets validation information for range data
    @return Authentication information
  '''
  def get_range_result(self)


  ''' 
    @brief  set IIC addr
    @param  addr  The IIC address to be modified
  '''
    
  def set_iic_addr(self,addr)

  ''' 
    @brief  Set Range Threshold Value
    @param  thresholdL :Lower Threshold
    @param  thresholdH :Upper threshold

  '''
  def set_range_threshold_value(self,threshold_l,threshold_h):


  ''' 
    @brief  Set ALS Threshold Value
    @param  thresholdL :Lower Threshold
    @param  thresholdH :Upper threshold

  '''
  def set_als_threshold_value(self,threshold_l,threshold_h):


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


```


## 兼容性

* RaspberryPi 版本

| Board        | Work Well | Work Wrong | Untested | Remarks |
| ------------ | :-------: | :--------: | :------: | ------- |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |           |            |    √     |         |
| RaspberryPi4 |     √     |            |          |         |

* Python 版本

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | ------- |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |


## 历史

- 日期 2021-09-28
- 版本 V1.0.0


## 创作者

Written by yangfeng(feng.yang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))

