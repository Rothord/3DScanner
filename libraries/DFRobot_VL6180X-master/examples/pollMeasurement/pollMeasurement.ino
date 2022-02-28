/**!
 * @file pollMeasurement.ino
 * @brief Measures absolute range from 0 to above 10 cm 
 * @n Measurement of ambient light data
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [yangfeng]<feng.yang@dfrobot.com>
 * @version  V1.0
 * @date  2021-02-08
 * @get from https://www.dfrobot.com
 * @url  https://github.com/DFRobot/DFRobot_VL6180X
 */
#include <DFRobot_VL6180X.h>

//When the IIC address is changed, the address should be passed in when the class is instantiated. And it will be saved after its changing. 
//But if the sensor is restarted by using the CE pin, the IIC address will change back to the default address 0x29
//DFRobot_VL6180X VL6180X(/* iicAddr */0x29,/* TwoWire * */&Wire);
DFRobot_VL6180X VL6180X;

void setup() {
  Serial.begin(9600);
  while(!(VL6180X.begin())){
    Serial.println("Please check that the IIC device is properly connected!");
    delay(1000);
  }
  /*Change IIC address*/
  //VL6180X.setIICAddr(0x29);
}

void loop() {
  /*Poll measurement of ambient light data*/
  float lux = VL6180X.alsPoLLMeasurement();
  String str ="ALS: "+String(lux)+" lux";
  Serial.println(str);
  delay(1000);
  /*Poll measurement of distance*/
  uint8_t range = VL6180X.rangePollMeasurement();
  /*Get the judgment of the range value*/
  uint8_t status = VL6180X.getRangeResult();
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
  delay(1000);
}
