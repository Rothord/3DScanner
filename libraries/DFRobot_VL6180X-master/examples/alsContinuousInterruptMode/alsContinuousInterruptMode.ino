/**
 * @file alsContinuousInterruptMode.ino
 * @brief The sensor can operate in four interrupt modes: 1. Trigger interrupt when below the L-threshold(lower threshold)
 * @n                                                     2. Trigger interrupt when above the U-threshold(upper threshold)
 * @n                                                     3. Trigger interrupt when below the L-threshold or above the U-threshold
 * @n                                                     4. Trigger interrupt after the new sample value acquisition
 * @n This example introduces four interrupts under continuous measurement ambient light mode
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [yangfeng]<feng.yang@dfrobot.com>
 * @version  V1.0
 * @date  2021-03-08
 * @get from https://www.dfrobot.com
 * @url  https://github.com/DFRobot/DFRobot_VL6180X
 */
#include <DFRobot_VL6180X.h>

DFRobot_VL6180X VL6180X;

uint8_t flag = 0;
uint8_t ret= 1;

void interrupt()
{
  if(flag ==0){
    flag = 1;
  }
}

void setup() {
  Serial.begin(9600);
  while(!(VL6180X.begin())){
    Serial.println("Please check that the IIC device is properly connected!");
    delay(1000);
  }
  /*Set the ambient light acquisition period*/  
  VL6180X.alsSetInterMeasurementPeriod(/* periodMs 0-25500ms */1000);

  /** Enable the notification function of the INT pin
   * mode：
   * VL6180X_DIS_INTERRUPT          Not enable interrupt
   * VL6180X_LOW_INTERRUPT          Enable interrupt, by default the INT pin outputs low level
   * VL6180X_HIGH_INTERRUPT         Enable interrupt, by default the INT pin outputs high level
   * Note: When using the VL6180X_LOW_INTERRUPT mode to enable the interrupt, please use "RISING" to trigger it. When using the VL6180X_HIGH_INTERRUPT mode 
   *       to enable the interrupt, please use "FALLING" to trigger it.
   */
  VL6180X.setInterrupt(/*mode*/VL6180X_LOW_INTERRUPT); 

  /** Set the interrupt mode for collecting ambient light
   * mode 
   * interrupt disable  :                       VL6180X_INT_DISABLE             0
   * value < thresh_low :                       VL6180X_LEVEL_LOW               1 
   * value > thresh_high:                       VL6180X_LEVEL_HIGH              2
   * value < thresh_low OR value > thresh_high: VL6180X_OUT_OF_WINDOW           3
   * new sample ready   :                       VL6180X_NEW_SAMPLE_READY        4
   */
  VL6180X.alsConfigInterrupt(/*mode*/VL6180X_OUT_OF_WINDOW); 
  /**Set acquisition gain
   * gain:
   * 20   times gain: VL6180X_ALS_GAIN_20                       
   * 10   times gain: VL6180X_ALS_GAIN_10                       
   * 5    times gain: VL6180X_ALS_GAIN_5                        
   * 2.5  times gain: VL6180X_ALS_GAIN_2_5                      
   * 1.57 times gain: VL6180X_ALS_GAIN_1_67                     
   * 1.27 times gain: VL6180X_ALS_GAIN_1_25                     
   * 1    times gain: VL6180X_ALS_GAIN_1                        
   * 40   times gain: VL6180X_ALS_GAIN_40                       
  */
  //VL6180X.setALSGain(VL6180X_ALS_GAIN_1);

  // The interface for setting the threshold here is related to the interface for setting the gain. If you want to specify both the gain and the threshold at the same time, 
  /*  please set the gain first and then set the threshold */
  VL6180X.setALSThresholdValue(/*thresholdL 0-65535lux */40,/*thresholdH 0-65535lux*/100);
  
  #if defined(ESP32) || defined(ESP8266)||defined(ARDUINO_SAM_ZERO)
  attachInterrupt(digitalPinToInterrupt(D9)/*Query the interrupt number of the D9 pin*/,interrupt,RISING);
  #else
  /*    The Correspondence Table of AVR Series Arduino Interrupt Pins And Terminal Numbers
   * ---------------------------------------------------------------------------------------
   * |                                        |  DigitalPin  | 2  | 3  |                   |
   * |    Uno, Nano, Mini, other 328-based    |--------------------------------------------|
   * |                                        | Interrupt No | 0  | 1  |                   |
   * |-------------------------------------------------------------------------------------|
   * |                                        |    Pin       | 2  | 3  | 21 | 20 | 19 | 18 |
   * |               Mega2560                 |--------------------------------------------|
   * |                                        | Interrupt No | 0  | 1  | 2  | 3  | 4  | 5  |
   * |-------------------------------------------------------------------------------------|
   * |                                        |    Pin       | 3  | 2  | 0  | 1  | 7  |    |
   * |    Leonardo, other 32u4-based          |--------------------------------------------|
   * |                                        | Interrupt No | 0  | 1  | 2  | 3  | 4  |    |
   * |--------------------------------------------------------------------------------------
   */
  /*                      The Correspondence Table of micro:bit Interrupt Pins And Terminal Numbers
   * ---------------------------------------------------------------------------------------------------------------------------------------------
   * |             micro:bit                       | DigitalPin |P0-P20 can be used as an external interrupt                                     |
   * |  (When using as an external interrupt,      |---------------------------------------------------------------------------------------------|
   * |no need to set it to input mode with pinMode)|Interrupt No|Interrupt number is a pin digital value, such as P0 interrupt number 0, P1 is 1 |
   * |-------------------------------------------------------------------------------------------------------------------------------------------|
   */
  attachInterrupt(/*Interrupt No*/0,interrupt,RISING);//Enable the external interrupt 0, connect INT1/2 to the digital pin of the main control: 
    //UNO(2), Mega2560(2), Leonardo(3), microbit(P0).
  #endif
  
  /*Start the continuous acquisition mode*/
  VL6180X.alsStartContinuousMode();
}

void loop() {
  if(flag == 1){
    flag = 0;
    /**Read and judge whether the generated interrupt is the same as the interrupt we set before
     * interrupt disable  :                       VL6180X_INT_DISABLE             0
     * value < thresh_low :                       VL6180X_LEVEL_LOW               1 
     * value > thresh_high:                       VL6180X_LEVEL_HIGH              2
     * value < thresh_low OR value > thresh_high: VL6180X_OUT_OF_WINDOW           3
     * new sample ready   :                       VL6180X_NEW_SAMPLE_READY        4
     */
    if(VL6180X.alsGetInterruptStatus() == VL6180X_OUT_OF_WINDOW){
      /*Get measurement data*/
      float lux = VL6180X.alsGetMeasurement();
      /*Clear interrupt*/
      VL6180X.clearAlsInterrupt();
      String str ="ALS: "+String(lux)+" lux";
      Serial.println(str);
    }
  }
}
