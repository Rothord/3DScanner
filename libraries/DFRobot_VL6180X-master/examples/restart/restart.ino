/**!
 * @file restart.ino
 * @brief The example will reset the sensor and restores the I2C address to the default
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [yangfeng]<feng.yang@dfrobot.com>
 * @version  V1.0
 * @date  2021-02-08
 * @get from https://www.dfrobot.com
 * @url  https://github.com/DFRobot/DFRobot_VL6180X
 */
 uint8_t Pin 12   //The GPIO port we set is connected to the CE pin in the sensor	
void setup() {
 pinMode(pin,OUTPUT);
 digitalWrite(Pin,LOW);
 delayMicroseconds(300);
 digitalWrite(Pin,HIGH);
}

void loop() {

}
