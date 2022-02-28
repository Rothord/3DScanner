#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>
//#include <VL6180X.h>
#include <DFRobot_VL6180X.h>
#include <EEPROM.h>

DFRobot_VL6180X VL6180X;
//VL6180X sensor;
LiquidCrystal_I2C lcd(0x27, 16, 2);
File myFile;

const int short RIGHT=LOW,LEFT=HIGH,DOWN=HIGH,UP=LOW;
const char *menu[]={"START MEASURE","SETTINGS"};
const char *settings[]={"VERTICAL STEP","CALIBRATION","MOVE SENSOR","BACK"};
const char *v_resolution[]={"0.25mm","0.5mm","0.75mm","1.0mm","1.25mm","1.5mm","1.75mm","2.0mm"};
const char *kursor="<-";
byte short Button_1_STATE=1,Button_2_STATE=1,Button_3_STATE=1;
byte short pos = 0,display_list = 0;
byte short menuNum = 0,step_mode[2]={1,0};
int short step_resolution[2]={400,8}, FileNumber=0, point0=EEPROM.read(0);
bool MEASURE=LOW;
unsigned long time_a, z_steps=0;
float measure=0;
String file_buffer;

#define VERTICAL_PWM 6
#define HORIZONTAL_PWM 5
#define BUTTON_3 A1 
#define BUTTON_2 A2 
#define BUTTON_1 A3
#define UPPER_LIMIT 9
#define LOWER_LIMIT 2
#define VERTICAL_DIRECTION 8
#define HORIZONTAL_DIRECTION 4
#define VERTICAL_STANDBY 7
#define HORIZONTAL_STANDBY 3

void setup() {
  //Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000);
  pinMode(HORIZONTAL_PWM, OUTPUT);
  pinMode(VERTICAL_PWM, OUTPUT);
  pinMode(BUTTON_1,INPUT_PULLUP);
  pinMode(BUTTON_2,INPUT_PULLUP);
  pinMode(BUTTON_3,INPUT_PULLUP);
  pinMode(UPPER_LIMIT,INPUT_PULLUP);
  pinMode(LOWER_LIMIT,INPUT_PULLUP);
  pinMode(HORIZONTAL_STANDBY,OUTPUT);
  pinMode(VERTICAL_STANDBY,OUTPUT);
  pinMode(HORIZONTAL_DIRECTION,OUTPUT);
  pinMode(VERTICAL_DIRECTION,OUTPUT);
  
  lcd.begin();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0,0);
  digitalWrite(VERTICAL_STANDBY,LOW);
  digitalWrite(HORIZONTAL_STANDBY,LOW);
  
//  sensor.init();
//  sensor.configureDefault();
//  sensor.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 0x40);
//  sensor.writeReg(0x10A, 0x30);
//  sensor.setTimeout(500);
VL6180X.begin();
}

void loop() {
if(MEASURE==LOW)
{
  if (Button_3_STATE==HIGH && digitalRead(BUTTON_3)==LOW && menuNum<3)
  {
///////////////////STARTING MEASUREMENT///////////////////
    if (menuNum==0 && pos==0)
    {
      lcd.clear();
      lcd.setCursor(0,0);lcd.print("Initializing");lcd.setCursor(0,1);lcd.print("SD card ...");
      delay(2000);
      lcd.clear();
      ///////////////////CARD INITIALIZATION///////////////////
      if (!SD.begin(10)) 
      {
        lcd.setCursor(0,0);lcd.print("Initialization");lcd.setCursor(0,1);lcd.print("failed :(");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);lcd.print("Check SD card");lcd.setCursor(0,1);lcd.print("and try again");
        delay(2000);
      } 
      else 
      {
        lcd.setCursor(0,0);lcd.print("Initialization");lcd.setCursor(0,1);lcd.print("successful");
        delay(2000);
        while (SD.exists("p"+String(FileNumber,DEC)+".txt"))
        {
          //Serial.println("p"+String(FileNumber,DEC)+".txt EXIST");
          FileNumber++;
        }
       myFile = SD.open("p"+String(FileNumber,DEC)+".txt", FILE_WRITE);
       myFile.close();
       //Serial.println("Creating p"+String(FileNumber,DEC)+".txt");
       lcd.clear();lcd.setCursor(0,0);lcd.print("File name:");lcd.setCursor(0,1);lcd.print("P"+String(FileNumber,DEC)+".txt");
       FileNumber++;
       delay(3000);
       ///////////////////INITIALIZATION OF STEPPER CONTROLLERS///////////////////
       motorresolution(VERTICAL_STANDBY,VERTICAL_PWM,VERTICAL_DIRECTION,step_mode[0]);
       motorresolution(HORIZONTAL_STANDBY,HORIZONTAL_PWM,HORIZONTAL_DIRECTION,step_mode[1]);
       delay(250);
       ///////////////////DRIVING SCANNER SLIDE DOWN///////////////////
       digitalWrite(VERTICAL_DIRECTION,DOWN);
       while(digitalRead(LOWER_LIMIT)==HIGH)
       {
         rotateMotor(VERTICAL_PWM,1);
       }
       z_steps=0;
       measure=0;
       digitalWrite(VERTICAL_DIRECTION,UP);
       rotateMotor(VERTICAL_PWM,2000);
       MEASURE=HIGH;
       //sensor.startRangeContinuous(30);
    }
}
///////////////////GO TO SETTINGS MENU///////////////////
    else if (menuNum==0 && pos==1)
    {
      menuNum=1;
      pos=0;
      lcd.clear();
    }
///////////////////SETTINGS MENU///////////////////
    else if (menuNum==1)
    {
      switch(pos)
      {
        case 0:
        menuNum=2; //SCANNER STEP MOTOR RESOLUTION SETTING MENU
        break;
        case 1:
        menuNum=3; //CALIBRATION MENU
        break;
        case 2:
        menuNum=4;
        break;
        case 3:
        menuNum=0; //RETURN TO MAIN MENU
        break;
      }
      pos=0;
      lcd.clear();
   }
///////////////////SCANNER STEP MOTOR RESOLUTION SETTING MENU///////////////////
//Z-AXIS step resolution: 0.25mm | 0.5mm | 0.75mm | 1.0mm | 1.25mm | 1.5mm | 1.75mm | 2.0mm 
   else if (menuNum==2)
   {
     switch(pos)
      {
        case 0:
        step_resolution[0]=100;
        break;
        case 1:
        step_resolution[0]=200;
        break;
        case 2:
        step_resolution[0]=300;
        break;
        case 3:
        step_resolution[0]=400;
        break;
        case 4:
        step_resolution[0]=500;
        break;
        case 5:
        step_resolution[0]=600;
        break;
        case 6:
        step_resolution[0]=700;
        break;
        case 7:
        step_resolution[0]=800;
        break;
      }
      step_mode[0]=1;
      menuNum=1;
      pos=0;
      lcd.clear();
   }
  }
///////////////////SENSOR CALIBRATION WITH RECORDING VALUES TO EEPROM///////////////////
   else if (menuNum==3)
   {
      if (Button_1_STATE==HIGH && digitalRead(BUTTON_1)==LOW)
      {
        lcd.setCursor(11,0);lcd.print("      ");
        //measure=sensor.readRangeSingleMillimeters();
        measure=VL6180X.rangePollMeasurement();
      }
      else if (Button_2_STATE==HIGH && digitalRead(BUTTON_2)==LOW)
      {
        point0=measure;
        EEPROM.write(0,point0);
        lcd.setCursor(0,1);lcd.print("                ");
        lcd.setCursor(0,1);lcd.print("DISTANCE SAVED");
        delay(1000);
      }
      else if (Button_3_STATE==HIGH && digitalRead(BUTTON_3)==LOW)
      {
        menuNum=1;
        pos=1;
        lcd.clear();
      }
   }
///////////////////MANUAL CONTROL OF THE Z-AXIS///////////////////
   else if (menuNum==4)
   {
      if (Button_1_STATE==HIGH && digitalRead(BUTTON_1)==LOW)
      {
        motorresolution(VERTICAL_STANDBY,VERTICAL_PWM,VERTICAL_DIRECTION,step_mode[0]);
        delay(100);
        digitalWrite(VERTICAL_DIRECTION,UP);
        while(digitalRead(BUTTON_3)==HIGH && digitalRead(UPPER_LIMIT)==HIGH&&digitalRead(VERTICAL_DIRECTION)==UP)
        {
          rotateMotor(VERTICAL_PWM,1);
        }
        digitalWrite(VERTICAL_STANDBY,LOW);
      }
      else if (Button_2_STATE==HIGH && digitalRead(BUTTON_2)==LOW)
      {
        motorresolution(VERTICAL_STANDBY,VERTICAL_PWM,VERTICAL_DIRECTION,step_mode[0]);
        delay(100);
        digitalWrite(VERTICAL_DIRECTION,DOWN);
        while(digitalRead(BUTTON_3)==HIGH && digitalRead(LOWER_LIMIT)==HIGH&&digitalRead(VERTICAL_DIRECTION)==DOWN)
        {
          rotateMotor(VERTICAL_PWM,1);
        }
        digitalWrite(VERTICAL_STANDBY,LOW);
      }
      else if (Button_3_STATE==HIGH && digitalRead(BUTTON_3)==LOW)
      {
        menuNum=1;
        pos=2;
        lcd.clear();
      }
   }
///////////////////DISPLAY MENU COMPONENTS ON LCD///////////////////
if (MEASURE==HIGH){lcd.clear();lcd.setCursor(0,0);lcd.print("MEASUREMENT");lcd.setCursor(0,1);lcd.print("IN PROGESS...");}
else
  {
    switch (menuNum)
    {
      ///////////////////MAIN MENU///////////////////
      case 0:
      lcd.setCursor(0,0);
      lcd.print(menu[0]);
      lcd.setCursor(14,pos%2);
      lcd.print(kursor);
      lcd.setCursor(0,1);
      lcd.print(menu[1]);
      menuwidth(0,1);  
      break;
      ///////////////////SETTINGS MENU///////////////////
      case 1:
      lcd.setCursor(0,0);
      lcd.print(settings[display_list]);
      lcd.setCursor(14,pos%2);
      lcd.print(kursor);
      lcd.setCursor(0,1);
      lcd.print(settings[display_list+1]);
      menuwidth(0,3);
      break;
      ///////////////////SCANNER STEP MOTOR RESOLUTION SETTING MENU///////////////////
      case 2:
      lcd.setCursor(0,0);
      lcd.print(v_resolution[display_list]);
      lcd.setCursor(14,pos%2);
      lcd.print(kursor);
      lcd.setCursor(0,1);
      lcd.print(v_resolution[display_list+1]);
      menuwidth(0,7);
      break;
      ///////////////////SENSOR CALIBRATION///////////////////
      case 3:
      lcd.setCursor(0,0);
      lcd.print("Distance: "+String(measure,0)+"mm");
      lcd.setCursor(0,1);
      lcd.print("B2:SAVE B3:BACK");
      break;
      ///////////////////MANUAL CONTROL OF THE Z-AXIS///////////////////
      case 4:
      lcd.setCursor(0,0);
      lcd.print("B1:UP B2:DOWN");
      lcd.setCursor(0,1);
      lcd.print("B3:STOP/BACK");
      break;
    }
  }
///////////////////BUTTON STATE CHANGE DETECTION///////////////////
  if(digitalRead(BUTTON_1)!=Button_1_STATE)Button_1_STATE = digitalRead(BUTTON_1);
  if(digitalRead(BUTTON_2)!=Button_2_STATE)Button_2_STATE = digitalRead(BUTTON_2);
  if(digitalRead(BUTTON_3)!=Button_3_STATE)Button_3_STATE = digitalRead(BUTTON_3);
  delay(25);
}
if (MEASURE==HIGH)
{
  ///////////////////MEASUREMENT LOOP///////////////////
  myFile = SD.open("p"+String(FileNumber,DEC)+".txt", FILE_WRITE);
  for(int i=0;i<400;i++)
  { 
      measure=VL6180X.rangePollMeasurement();
      rotateMotorH(HORIZONTAL_PWM,128); 
      point_calculation(point0,measure,(float)i*360/400,((float)z_steps/800)); 
      if(measure<200){myFile.println(file_buffer);} 
  }
  myFile.close();
  if(digitalRead(UPPER_LIMIT)==LOW || measure>200)
  {
      MEASURE=0;
      digitalWrite(VERTICAL_STANDBY,LOW);
      digitalWrite(HORIZONTAL_STANDBY,LOW);
      lcd.clear(); 
  }
  else
  {
      rotateMotor(VERTICAL_PWM,step_resolution[0]);
      z_steps+=step_resolution[0]; 
  }  
}
}

///////////////////STEP MODES: 1/2 - 1 | 1/8 - 3 | 1/128 - 0 | 1/256 - 2
void motorresolution(int STANDBY,int PWM,int DIRECTION,byte MODE)
{
  digitalWrite(STANDBY,LOW);
  delay(100);
  digitalWrite(PWM,bitRead(MODE, 0));
  digitalWrite(DIRECTION,bitRead(MODE, 1));
  delay(100);
  digitalWrite(STANDBY,HIGH);
  delay(100);
  digitalWrite(PWM,LOW);
  digitalWrite(DIRECTION,LOW);
}
///////////////////FUNCTION TO ROTATE A Z-AXIS STEP MOTOR WITH A SPECIFIED NUMBER OF STEPS (DEFINED FOR 1/2 STEP RESOLUTION)///////////////////
void rotateMotor(int pinNo, int steps)
{     
  for (int j=0; j<steps; j++)
  {
    digitalWrite(pinNo, HIGH);                       
    time_a = micros();    
    while(micros() - time_a < 500){}  
    digitalWrite(pinNo, LOW);
    time_a = micros();    
    while(micros() - time_a < 500){} 
  }  
}
///////////////////FUNCTION TO ROTATE A HORIZONTAL STEP MOTOR WITH A SPECIFIED NUMBER OF STEPS (DEFINED FOR 1/128 STEP RESOLUTION)///////////////////
void rotateMotorH(int pinNo, int steps)
{
  for (int k=0; k<steps; k++)
  {
    digitalWrite(pinNo, HIGH);                       
    time_a = micros();    
    while(micros() - time_a < 50){}  
    digitalWrite(pinNo, LOW);
    time_a = micros();    
    while(micros() - time_a < 50){} 
  }
}
///////////////////FUNCTION FOR DETERMINING THE COORDINATES OF THE MEASURED POINT///////////////////
void point_calculation(int default_point,float measurement,float degree,float z_step)
{
  float x=cos(degree*PI/180)*(float)(default_point-measurement);
  float y=sin(degree*PI/180)*(float)(default_point-measurement);
  float z=2*z_step;
  file_buffer=String(x,6)+" "+String(y,6)+" "+String(z,6);
}
///////////////////FUNCTION THAT DEFINES BORDER OF THE MENU///////////////////
void menuwidth(byte lowerBorder,byte upperBorder)
{
  if (Button_2_STATE==HIGH && digitalRead(BUTTON_2)==LOW)
  {
    pos++;
    if (pos > upperBorder)
    {
      pos = lowerBorder;  
    }
     lcd.clear();
  }
  else if (Button_1_STATE==HIGH && digitalRead(BUTTON_1)==LOW)
  {
    pos--;
    if (pos < lowerBorder)
    {
      pos = upperBorder;  
    }
      lcd.clear();
  }
  if (pos>=0 && pos<=1)
  {
      display_list=0;
  }
  else if(pos>=2 && pos<=3)
  {
      display_list=2;
  }
  else if (pos>=4 && pos<=5)
  {
      display_list=4;
  }
  else if(pos>=6 && pos<=7)
  {
      display_list=6;
  }
}
