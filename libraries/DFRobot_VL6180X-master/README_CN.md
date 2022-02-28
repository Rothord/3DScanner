# DFRobot_VL6180X

- [English Version](./README.md)

VL6180X是意法半导体的一款ToF激光测距传感器，可测量5-100mm的距离，但在良好的环境下测量距离可达200mm。VL6180X搭载了意法半导体FlightSense ™技术，可以精确的测量距离，与目标反射率无关。同时VL6180X还搭载了环境光传感器，可以测量出环境光数据。

![产品实物图](./resources/images/SEN0427.png)


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

要使用这个库，首先下载库文件，将其粘贴到\Arduino\libraries目录中，然后打开示例文件夹并在文件夹中运行演示。


## 方法

```C++
   /**
   * @brief 初始化函数
   * @return 返回0表示初始化成功，返回其他值表示初始化失败，返回错误码
   */
  int begin(void);

  /**
   * @brief 获取测量温度值
   * @return 温度值 单位：℃
   */
  float getTemperature(void);

  /**
   * @brief 获取X轴加速计值
   * @return X轴加速计值 单位：mg
   */
  float getAccelDataX(void);

  /**
   * @brief 获取Y轴加速计值
   * @return Y轴加速计值 单位：mg
   */
  float getAccelDataY(void);

  /**
   * @brief 获取Z轴加速计值
   * @return Z轴加速计值 单位：mg
   */
  float getAccelDataZ(void);

  /**
   * @brief 获取X轴陀螺仪值
   * @return X轴陀螺仪值 单位：dps
   */
  float getGyroDataX(void);

  /**
   * @brief 获取Y轴陀螺仪值
   * @return Y轴陀螺仪值 单位：dps
   */
  float getGyroDataY(void);

  /**
   * @brief 获取Z轴陀螺仪值
   * @return Z轴陀螺仪值 单位：dps
   */
  float getGyroDataZ(void);

  /**
   * @brief 敲击事件初始化
   * @param accelMode 加速计工作模式 0 代表工作在低功耗模式 ，1代表工作在低噪声模式
   */
  void tapDetectionInit(uint8_t accelMode);

  /**
   * @brief 获取敲击信息
   */
  void getTapInformation();

  /**
   * @brief 获取敲击次数，分别是：单击、双击
   * @return 分别是：单击 TAP_SINGLE、双击TAP_DOUBLE
   */
  uint8_t numberOfTap();

  /**
   * @brief 获取敲击轴，分别是：X\Y\Z轴
   * @return 分别是：X_AXIS、Y_AXIS、Z_AXIS
   */
  uint8_t axisOfTap();

  /**
   * @brief 初始化移动唤醒
   */
  void wakeOnMotionInit();

  /**
   * @brief 设置某轴加速度计的运动中断唤醒的阈值
   * @param axis  x/y/z轴
   * @n           X_AXIS_WOM
   * @n           Y_AXIS_WOM
   * @n           Z_AXIS_WOM
   * @n           ALL
   * @param threshold  Range(0-255) [WoM thresholds are expressed in fixed “mg” independent of the selected Range [0g : 1g]; Resolution 1g/256=~3.9mg]
   */
  void setWOMTh(uint8_t axis,uint8_t threshold);

  /**
   * @brief 使能运动唤醒中断
   * @param axis  x/y/z轴
   * @n           X_AXIS_WOM
   * @n           Y_AXIS_WOM
   * @n           Z_AXIS_WOM
   */
  void setWOMInterrupt(uint8_t axis);

  /**
   * @brief 设置重要运动检测模式并且开启SMD中断
   * @param mode  0: disable SMD
   * @n           2 : SMD short (1 sec wait) An SMD event is detected when two WOM are detected 1 sec apart
   * @n           3 : SMD long (3 sec wait) An SMD event is detected when two WOM are detected 3 sec apart
   */
  void enableSMDInterrupt(uint8_t mode);

  /**
   * @brief 读取中断信息，并清除中断
   * @param reg 中断信息寄存器
   * @n         ICM42688_INT_STATUS2    可以获取SMD_INT、WOM_X_INT、WOM_Y_INT、WOM_Z_INT中断信息并且清除
   * @n         ICM42688_INT_STATUS3    可以获取STEP_DET_INT、STEP_CNT_OVF_INT、TILT_DET_INT、WAKE_INT、TAP_DET_INT中断信息并且清除
   * @return 中断信息，无中断时返回0。
   */
  uint8_t readInterruptStatus(uint8_t reg);

  /**
   * @brief 设置陀螺仪或者加速计的ODR和 Full-scale range
   * @param who  GYRO/ACCEL/ALL
   * @n          GYRO:代表只设置陀螺仪
   * @n          ACCEL:代表只设置加速计
   * @param ODR 输出数据速率
   * @n         ODR_32KHZ         支持：Gyro/Accel(LN mode)
   * @n         ODR_16KHZ         支持：Gyro/Accel(LN mode)
   * @n         ODR_8KHZ          支持：Gyro/Accel(LN mode)
   * @n         ODR_4KHZ          支持：Gyro/Accel(LN mode)
   * @n         ODR_2KHZ          支持：Gyro/Accel(LN mode)
   * @n         ODR_1KHZ          支持：Gyro/Accel(LN mode)
   * @n         ODR_200HZ         支持：Gyro/Accel(LP or LN mode)
   * @n         ODR_100HZ         支持：Gyro/Accel(LP or LN mode)
   * @n         ODR_50HZ          支持：Gyro/Accel(LP or LN mode)
   * @n         ODR_25KHZ         支持：Gyro/Accel(LP or LN mode)
   * @n         ODR_12_5KHZ       支持：Gyro/Accel(LP or LN mode)
   * @n         ODR_6_25KHZ       支持：Accel(LP mode)
   * @n         ODR_3_125HZ       支持：Accel(LP mode)
   * @n         ODR_1_5625HZ      支持：Accel(LP mode)
   * @n         ODR_500HZ         支持：Accel(LP or LN mode)
   * @param FSR Full-scale range
   * @n         FSR_0      Gyro:±2000dps   /   Accel: ±16g
   * @n         FSR_1      Gyro:±1000dps   /   Accel: ±8g
   * @n         FSR_2      Gyro:±500dps    /   Accel: ±4g
   * @n         FSR_3      Gyro:±250dps    /   Accel: ±2g
   * @n         FSR_4      Gyro:±125dps    /   Accel: 不可选
   * @n         FSR_5      Gyro:±62.5dps   /   Accel: 不可选
   * @n         FSR_6      Gyro:±31.25dps  /   Accel: 不可选
   * @n         FSR_7      Gyro:±15.625dps /   Accel: 不可选
   * @return true代表设置设置成功；flase代表选择的参数有误
   */
  bool setODRAndFSR(uint8_t who,uint8_t ODR,uint8_t FSR);
  /**
   * @brief 设置FIFO的数据包格式
   */
  void setFIFODataMode();
  /**
   * @brief 启用FIFO
   */
  void startFIFOMode();

  /**
   * @brief 关闭停用FIFO
   */
  void sotpFIFOMode();

  /**
   * @brief 读取FIFO数据，分别读出温度数据、陀螺仪数据、加速计数据并保存等待解析
   */
  void getFIFOData();

  /**
   * @brief 设置中断模式
   * @param INTPin  中断引脚 ：1 代表使用INT1中断引脚；2代表使用INT2中断引脚
   * @param INTmode 设置中断模式，1 代表中断锁定模式（即中断触发后会保持极性，清除中断后恢复）；0 代表脉冲模式
   * @param INTPolarity 中断输出的电平极性，0 代表产生中断时中断引脚极性为LOW, 1 代表产生中断时中断引脚极性为HIGH
   * @param INTDriveCircuit  0 代表Open drain  1 代表 Push pull
   */
  void setINTMode(uint8_t INTPin,uint8_t INTmode,uint8_t INTPolarity,uint8_t INTDriveCircuit);

  /**
   * @brief 启动陀螺仪
   * @param mode 设置陀螺仪的工作模式
   * @n          STANDBY_MODE_ONLY_GYRO 1  设置为备用模式，仅支持陀螺仪
   * @n          LN_MODE  3                设置为低噪声模式
   */
  void startGyroMeasure(uint8_t mode);

  /**
   * @brief 启动加速计
   * @param mode 设置加速计的工作模式
   * @n          LP_MODE_ONLY_ACCEL  2     设置为低功耗模式，仅支持加速计
   * @n          LN_MODE  3                设置为低噪声模式
   */
  void startAccelMeasure(uint8_t mode);

  /**
   * @brief 启动温度计
   */
  void startTempMeasure();
```


## 兼容性

主控               |  正常运行    |   运行失败    |   未测试    | 备注
| ------------------ | :-------: | :--------: | :------: | ------- |
| Arduino Uno        |     √     |            |          |         |
| FireBeetle-ESP8266 |     √     |            |          |         |
| FireBeetle-ESP32   |     √     |            |          |         |
| Arduino MEGA2560   |     √     |            |          |         |
| Arduino Leonardo   |     √     |            |          |         |
| Micro:bit          |     √     |            |          |         |
| FireBeetle-M0      |     √     |            |          |         |
| Raspberry Pi       |     √     |            |          |         |


## 历史

- 日期 2021-09-28
- 版本 V1.0.0


## 创作者

Written by yangfeng(feng.yang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))

