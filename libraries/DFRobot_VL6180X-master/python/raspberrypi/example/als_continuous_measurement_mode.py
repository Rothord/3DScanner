  # -*- coding: utf-8 -*
""" file als_continuous_measurement_mode.py
  # @brief The sensor can operate in four interrupt modes: 1. Trigger interrupt when below the L-threshold(lower threshold)
  # @n                                                     2. Trigger interrupt when above the U-threshold(upper threshold)
  # @n                                                     3. Trigger interrupt when below the L-threshold or above the U-threshold
  # @n                                                     4. Trigger interrupt after the new sample value acquisition
  # @n This example introduces four interrupts under continuous measurement ambient light mode
  # @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  # @licence     The MIT License (MIT)
  # @author      [yangfeng]<feng.yang@dfrobot.com> 
  # @version  V1.0
  # @date  2021-02-09
  # @get from https://www.dfrobot.com
  # @url https://github.com/DFRobot/DFRobot_VL6180X
"""
import sys
sys.path.append('../')
from DFRobot_VL6180X import DFRobot_VL6180X
import time
import RPi.GPIO as GPIO

# When the IIC address is changed, the revised address should be passed in when instantiating class. And the changed I2C address will be saved after power-down.
# But if the sensor is restarted by using the CE pin, the IIC address will be back to the default address 0x29
# bus :iic bus
VL6180X = DFRobot_VL6180X(iic_addr= 0x29,bus = 1)
global flag
flag = 0
def int_callback(channel):
  global flag
  if flag == 0:
    flag = 1
    
GPIO.setwarnings(False)
# Use GPIO port to monitor sensor interrupt
gpio_int = 22
GPIO.setmode(GPIO.BCM)
GPIO.setup(gpio_int, GPIO.IN)
GPIO.add_event_detect(gpio_int, GPIO.FALLING, callback=int_callback) 

while(VL6180X.begin() == False ):
  print ('Please check that the IIC device is properly connected')

''' mode
   * VL6180X_DIS_INTERRUPT          Not enable interrupt notification function
   * VL6180X_LOW_INTERRUPT          Enable interrupt notification function, by default the INT pin outputs low level
   * VL6180X_HIGH_INTERRUPT         Enable interrupt notification function, by default the INT pin outputs high level
   * Note: When using the VL6180X_LOW_INTERRUPT mode to enable the interrupt notification function, please use "RISING" to trigger it.
   *       When using the VL6180X_HIGH_INTERRUPT mode to enable the interrupt notification function, please use "FALLING" to trigger it.
'''
VL6180X.set_interrupt(mode = VL6180X.VL6180X_HIGH_INTERRUPT)
''' mode 
   * interrupt disable  :                       VL6180X_INT_DISABLE             0
   * value < thresh_low :                       VL6180X_LEVEL_LOW               1 
   * value > thresh_high:                       VL6180X_LEVEL_HIGH              2
   * value < thresh_low OR value > thresh_high: VL6180X_OUT_OF_WINDOW           3
   * new sample ready   :                       VL6180X_NEW_SAMPLE_READY        4
'''
VL6180X.als_config_interrupt(mode = VL6180X.VL6180X_NEW_SAMPLE_READY)
# Set ambient light collecting period
VL6180X.als_set_inter_measurement_period(period_ms = 1000)
'''gain
  * 20   times gain: VL6180X_ALS_GAIN_20                       
  * 10   times gain: VL6180X_ALS_GAIN_10                       
  * 5    times gain: VL6180X_ALS_GAIN_5                        
  * 2.5  times gain: VL6180X_ALS_GAIN_2_5                      
  * 1.57 times gain: VL6180X_ALS_GAIN_1_67                     
  * 1.27 times gain: VL6180X_ALS_GAIN_1_25                     
  * 1    times gain: VL6180X_ALS_GAIN_1                        
  * 40   times gain: VL6180X_ALS_GAIN_40                       
'''
VL6180X.set_als_gain(gain = VL6180X.VL6180X_ALS_GAIN_1)
#The interface for setting the threshold here is related to the interface for setting the gain.
#If you want to specify both the gain and the threshold at the same time, please set the gain first and then set the threshold.
VL6180X.set_als_threshold_value(threshold_l = 30,threshold_h = 100)
# Start measurement
VL6180X.als_start_continuous_mode()

try:
  while True:
    if(flag == 1):
      flag = 0
      '''
         * No threshold events reported  :                                            0
         * value < thresh_low :                       VL6180X_LEVEL_LOW               1 
         * value > thresh_high:                       VL6180X_LEVEL_HIGH              2
         * value < thresh_low OR value > thresh_high: VL6180X_OUT_OF_WINDOW           3
         * new sample ready   :                       VL6180X_NEW_SAMPLE_READY        4
      '''
      if( VL6180X.als_get_interrupt_status()== VL6180X.VL6180X_NEW_SAMPLE_READY):
        # Get ambient light data
        lux = VL6180X.als_get_measurement()
        # Clear interrupt
        VL6180X.clear_als_interrupt()
        print('ALS vlaue : %f lux'%lux)

  
except KeyboardInterrupt:
  GPIO.cleanup()    
