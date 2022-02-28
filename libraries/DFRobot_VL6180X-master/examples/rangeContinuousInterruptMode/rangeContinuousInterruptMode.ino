/**
 * @file rangeContinuousInterruptMode.ino
 * @brief The sensor can operate in four interrupt modes: 1. Trigger interrupt when below the L-threshold(lower threshold)
 * @n                                                     2. Trigger interrupt above the U-threshold(upper threshold)
 * @n                                                     3. Trigger interrupt when below the L-threshold or above the U-threshold
 * @n                                                     4. Trigger interrupt after the new sample value acquisition
 * @n This example introduces four interrupts under continuous range measuring mode
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

uint8_t ret= 1;
uint8_t flag = 0;

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
  /** Enable the notification function of the INT pin
   * mode：
   * VL6180X_DIS_INTERRUPT          Not enable interrupt
   * VL6180X_LOW_INTERRUPT          Enable interrupt, by default the INT pin outputs low level
   * VL6180X_HIGH_INTERRUPT         Enable interrupt, by default the INT pin outputs high level
   * Note: When using the VL6180X_LOW_INTERRUPT mode to enable the interrupt, please use "RISING" to trigger it.
   *       When using the VL6180X_HIGH_INTERRUPT mode to enable the interrupt, please use "FALLING" to trigger it.
   */
  VL6180X.setInterrupt(/*mode*/VL6180X_HIGH_INTERRUPT); 

  /** Set the interrupt mode for collecting ambient light
   * mode 
   * interrupt disable  :                       VL6180X_INT_DISABLE             0
   * value < thresh_low :                       VL6180X_LEVEL_LOW               1 
   * value > thresh_high:                       VL6180X_LEVEL_HIGH              2
   * value < thresh_low OR value > thresh_high: VL6180X_OUT_OF_WINDOW           3
   * new sample ready   :                       VL6180X_NEW_SAMPLE_READY        4
   */
  VL6180X.rangeConfigInterrupt(VL6180X_OUT_OF_WINDOW);

  /*Set the range measurement period*/
  VL6180X.rangeSetInterMeasurementPeriod(/* periodMs 0-25500ms */1000);

  /*Set threshold value*/
  VL6180X.setRangeThresholdValue(/*thresholdL 0-255mm */40,/*thresholdH 0-255mm*/100);

  #if defined(ESP32) || defined(ESP8266)||defined(ARDUINO_SAM_ZERO)
  attachInterrupt(digitalPinToInterrupt(D9)/*Query the interrupt number of the D9 pin*/,interrupt,FALLING);
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
  attachInterrupt(/*Interrupt No*/0,interrupt,FALLING);//Enable the external interrupt 0, connect INT1/2 to the digital pin of the main control: 
    //UNO(2), Mega2560(2), Leonardo(3), microbit(P0).
  #endif

  /*Start continuous range measuring mode */
  VL6180X.rangeStartContinuousMode();

}
void loop() {
  if(flag == 1){
    flag = 0;
    /**Read and judge whether the generated interrupt is the same as the interrupt we set before
     * No threshold events reported  :                                            0
     * value < thresh_low :                       VL6180X_LEVEL_LOW               1 
     * value > thresh_high:                       VL6180X_LEVEL_HIGH              2
     * value < thresh_low OR value > thresh_high: VL6180X_OUT_OF_WINDOW           3
     * new sample ready   :                       VL6180X_NEW_SAMPLE_READY        4
     */
    if(VL6180X.rangeGetInterruptStatus() == VL6180X_OUT_OF_WINDOW){
      /*Get the measured distance data*/
      uint8_t range = VL6180X.rangeGetMeasurement();
      /*Get the judged result of the range value*/
      uint8_t status = VL6180X.getRangeResult();
      /*Clear interrupts generated by measuring range*/
      VL6180X.clearRangeInterrupt();
      String str1 = "Range: "+String(range) + " mm"; 
      switch(status){
        case VL6180X_NO_ERR:
          Serial.println(str1);
          break;
        case VL6180X_EARLY_CONV_ERR:
          Serial.println("RANGE ERR: ECE check failed !");
          break;
        case VL6180X_MAX_CONV_ERR:
          Serial.println("RANGE ERR: System did not converge before the specified max!");
          break;
        case VL6180X_IGNORE_ERR:
          Serial.println("RANGE ERR: Ignore threshold check failed !");
          break;
        case VL6180X_MAX_S_N_ERR:
          Serial.println("RANGE ERR: Measurement invalidated!");
          break;
        case VL6180X_RAW_Range_UNDERFLOW_ERR:
          Serial.println("RANGE ERR: RESULT_RANGE_RAW < 0!");
          break;
        case VL6180X_RAW_Range_OVERFLOW_ERR:
          Serial.println("RESULT_RANGE_RAW is out of range !");
          break;
        case VL6180X_Range_UNDERFLOW_ERR:
          Serial.println("RANGE ERR: RESULT__RANGE_VAL < 0 !");
          break;
        case VL6180X_Range_OVERFLOW_ERR:
          Serial.println("RANGE ERR: RESULT__RANGE_VAL is out of range !");
          break;
       default:
          Serial.println("RANGE ERR: Systerm err!");
          break;
     }
    }
  }
}
