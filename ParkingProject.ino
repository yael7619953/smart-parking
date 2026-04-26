#define RED 11
#define GREEN 6
#define LED 12
#define BUZZER 7
#define SENSOR_LIGHT A0
#define SERVO 3

#include <Servo.h>
Servo servo;

int currentServoAngle = 0;
bool isCarIn = false;

void setup() {
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(SENSOR_LIGHT,INPUT);
  
  servo.attach(SERVO);
  servo.write(currentServoAngle);

  isCarIn = analogRead(SENSOR_LIGHT) <= 600;

  if (isCarIn) setRGBLed(100, 0);
  else setRGBLed(0, 100);
}

void loop() {
  Serial.println(analogRead(SENSOR_LIGHT));
  mainParking();
}

void setRGBLed(int r,int g)
{
  analogWrite(RED, r);
  analogWrite(GREEN, g);
}

void setLed(int level){
  if (level == 0){
    digitalWrite(LED, LOW);
  }
  else {
    digitalWrite(LED, HIGH);
  }
}

void setBuzzer(int time, int wait,int power){
   for (int i = 0; i < time; i++){
      tone(BUZZER,power);
      delay(wait);
      noTone(BUZZER);
      delay(wait);
   }
}

void moveServo(int targetAngle, int duration) {
  int diff = targetAngle - currentServoAngle;
  int steps = abs(diff);
  int step = diff / steps;
  int delayTime = duration / steps;

  for (int i = 0; i < steps; i++) {
    currentServoAngle += step;
    servo.write(currentServoAngle);
    delay(delayTime);
  }
}


void mainParking() {
  if (analogRead(SENSOR_LIGHT) > 600) {
    if (isCarIn) {
      setRGBLed(0,100);
      setBuzzer(1,1000,1500);
      moveServo(35,1000);
      setLed(0);
      isCarIn = false;
    }
  } else {
    if(!isCarIn){
      setRGBLed(100,0);

      moveServo(0,1000);
      setLed(1);
      isCarIn = true;
    }
  }
}

