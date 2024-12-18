#include <Servo.h>

Servo myservo;
int servoPos = 0;           // Current position of the servo
int servoIncrement = 1;     // Step size (1 degree at a time)
unsigned long lastServoMove = 0; // Time of last servo movement
const int servoInterval = 15;    // Interval for servo movement in ms

// clock stepper
#define IN1 12
#define IN2 11
#define IN3 10
#define IN4 9

// hand stepper
#define IN5 7
#define IN6 6
#define IN7 5
#define IN8 4

int stepper1[4] = {IN1, IN2, IN3, IN4};
int stepper2[4] = {IN5, IN6, IN7, IN8};

int Steps = 0;
boolean Direction = false;
unsigned long last_time_stepper_1;
unsigned long last_time_stepper_2;
unsigned long currentMillis;
int steps_left_1 = 4095;
int steps_left_2 = 4095;
long time;
boolean servo_active = false;
boolean stepper1_active = false;
boolean stepper2_active = false;

void setup() {
  Serial.begin(115200);

  // Stepper motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  
  // Servo motor setup
  myservo.attach(8);
  myservo.write(servoPos);
}

void loop() {
  
  // Wait for osc message
  if (Serial.available() > 0) {
    char input =  Serial.read();
    Serial.println(input);
    // run se servo
    if (input == '1') {
      Direction = false;
      servo_active = true;
    } else if (input == '2') {
      stepper1_active = true;
    } else if (input == '3'){
      stepper2_active = true;
    } else if (input == '4'){
      Direction = true;
      stepper1_active = true;
      stepper2_active = true;
    } else if(input == '5') {
      servo_active = false;
      stepper1_active = false;
      stepper2_active = false;
    }
  }

  if (stepper1_active){
      if (steps_left_1 > 0) {
        currentMillis = micros();
        if (currentMillis - last_time_stepper_1 >= 1000) {
          stepper(1, stepper1);
          time = time + micros() - last_time_stepper_1;
          last_time_stepper_1 = micros();
          steps_left_1--;
        }
      } else {
      // Serial.println(time);
      //  Serial.println("Wait...!");
        steps_left_1 = 4095;
      }
    }

  if (stepper2_active){
      if (steps_left_2 > 0) {
        currentMillis = micros();
        if (currentMillis - last_time_stepper_2 >= 1000) {
          stepper(1, stepper2);
          time = time + micros() - last_time_stepper_2;
          last_time_stepper_2 = micros();
          steps_left_2--;
        }
      } else {
      // Serial.println(time);
      //  Serial.println("Wait...!");
        steps_left_2 = 4095;
      }
    }

  if (servo_active) {
    // Non-blocking servo movement
    unsigned long currentMillis = millis();
    if (currentMillis - lastServoMove >= servoInterval) {
      lastServoMove = currentMillis;
      servoPos += servoIncrement;

      if (servoPos >= 180 || servoPos <= 0) {
        servoIncrement = -servoIncrement;  // Reverse direction
      }
      myservo.write(servoPos);  // Move the servo to the new position
    }
  }
}

void stepper(int xw, int (&stepper)[4]) {
  for (int i = 0; i < 4; i++) {
      Serial.println(stepper[i]);
  }
  for (int x = 0; x < xw; x++) {
    switch (Steps) {
      case 0:
        digitalWrite(stepper[0], LOW); 
        digitalWrite(stepper[1], LOW);
        digitalWrite(stepper[2], LOW);
        digitalWrite(stepper[3], HIGH);
        break; 
      case 1:
        digitalWrite(stepper[0], LOW); 
        digitalWrite(stepper[1], LOW);
        digitalWrite(stepper[2], HIGH);
        digitalWrite(stepper[3], HIGH);
        break; 
      case 2:
        digitalWrite(stepper[0], LOW); 
        digitalWrite(stepper[1], LOW);
        digitalWrite(stepper[2], HIGH);
        digitalWrite(stepper[3], LOW);
        break; 
      case 3:
        digitalWrite(stepper[0], LOW); 
        digitalWrite(stepper[1], HIGH);
        digitalWrite(stepper[2], HIGH);
        digitalWrite(stepper[3], LOW);
        break; 
      case 4:
        digitalWrite(stepper[0], LOW); 
        digitalWrite(stepper[1], HIGH);
        digitalWrite(stepper[2], LOW);
        digitalWrite(stepper[3], LOW);
        break; 
      case 5:
        digitalWrite(stepper[0], HIGH); 
        digitalWrite(stepper[1], HIGH);
        digitalWrite(stepper[2], LOW);
        digitalWrite(stepper[3], LOW);
        break; 
      case 6:
        digitalWrite(stepper[0], HIGH); 
        digitalWrite(stepper[1], LOW);
        digitalWrite(stepper[2], LOW);
        digitalWrite(stepper[3], LOW);
        break; 
      case 7:
        digitalWrite(stepper[0], HIGH); 
        digitalWrite(stepper[1], LOW);
        digitalWrite(stepper[2], LOW);
        digitalWrite(stepper[3], HIGH);
        break; 
      default:
        digitalWrite(stepper[0], LOW); 
        digitalWrite(stepper[1], LOW);
        digitalWrite(stepper[2], LOW);
        digitalWrite(stepper[3], LOW);
        break; 
    }
    SetDirection();
  }
}

void SetDirection() {
  if (Direction) { 
    Steps++;
  } else {
    Steps--; 
  }
  if (Steps > 7) { Steps = 0; }
  if (Steps < 0) { Steps = 7; }
}
