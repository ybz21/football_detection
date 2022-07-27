// https://robojax.com/learn/arduino/?vid=robojax_PCA9685-V4
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver cloudPlatform = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  150 
#define SERVOMAX  600 
#define USMIN  500  
#define USMAX  2500 
#define SERVO_FREQ 50
#define BOTTOMSEVROID 0
#define TOPSEVROID 1


int angleToPulse(int ang){
   int pulse = map(ang,0, 270, SERVOMIN,SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max
   Serial.print("Angle: ");Serial.print(ang);
   Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}

void setBottomServoAngle(int angle){
  Serial.print("Bottom: ");
  cloudPlatform.setPWM(BOTTOMSEVROID, 0, angleToPulse(angle));
}

void setTopServoAngle(int angle){
  Serial.print("Top: ");
  cloudPlatform.setPWM(TOPSEVROID, 0, angleToPulse(angle) );
}

void initCloudPlatform() {
  // Serial.begin(9600);
  // Serial.println("16 channel Servo test!");
  cloudPlatform.begin();
  cloudPlatform.setOscillatorFrequency(27000000);
  cloudPlatform.setPWMFreq(SERVO_FREQ); 
}


// void loop() {
//     for(int angle =0; angle<181; angle +=1){
//           //  setBottomServoAngle(angle);
//            setTopServoAngle(angle);
//            delay(1000);
//     }
// }