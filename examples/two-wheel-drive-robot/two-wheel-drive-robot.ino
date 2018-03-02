/*
* Demo code for a two wheel differential drive robot using two continuous rotation micro servos.
*/

#include <Servo.h>
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

// continuous microservos
#define leftServoPin 11
#define rightServoPin 10

// Bluetooth module HM 10 pins
#define BLUETOOTH_TX 8
#define BLUETOOTH_RX 7

// Continuous rotation micro servo parameters.
#define THROTTLE_ZERO_THRESHOLD 5
#define MIN_THROTTLE 10
#define STILL 90
#define LEFT_FORWARD_MAX 130
#define LEFT_BACKWARD_MAX 50
#define RIGHT_FORWARD_MAX LEFT_BACKWARD_MAX
#define RIGHT_BACKWARD_MAX LEFT_FORWARD_MAX
#define LEFT_FORWARD_MIN STILL + MIN_THROTTLE
#define LEFT_BACKWARD_MIN STILL - MIN_THROTTLE
#define RIGHT_FORWARD_MIN LEFT_BACKWARD_MIN
#define RIGHT_BACKWARD_MIN LEFT_FORWARD_MIN

Servo leftServo;
Servo rightServo;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth);

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void handleDriving(int throttle, int steering) {
  throttle -= 49;
  steering -= 49;

  int leftVal, rightVal;
  double throttleReduction;

  // still if close enough to zero
  if (abs(throttle) < THROTTLE_ZERO_THRESHOLD) {
    leftVal = STILL;
    rightVal = STILL;
  }
  // forward
  else if (throttle > 0) {
    // turn left forward
    if (steering <= 0) {
      // steering: max abs = 49; min abs = 0
      throttleReduction = mapFloat(steering, 0, -49, 1, 0); 
      leftVal = map(throttle*throttleReduction, 0, 50, LEFT_FORWARD_MIN, LEFT_FORWARD_MAX);
      rightVal = map(throttle, 0, 50, RIGHT_FORWARD_MIN, RIGHT_FORWARD_MAX);
    }
    // turn right forward
    else {
      throttleReduction = mapFloat(steering, 1, 50, 1, 0); 
      leftVal = map(throttle, 0, 50, LEFT_FORWARD_MIN, LEFT_FORWARD_MAX);
      rightVal = map(throttle*throttleReduction, 0, 50, RIGHT_FORWARD_MIN, RIGHT_FORWARD_MAX);
    }
  }
  // backward
  else {
    // TODO: implement this
    // turn left backward
    if (steering <= 0) {
      throttleReduction = mapFloat(steering, 0, -49, 1, 0); 
      leftVal = map(throttle*throttleReduction, 0, -49, LEFT_BACKWARD_MIN, LEFT_BACKWARD_MAX);
      rightVal = map(throttle, 0, -49, RIGHT_BACKWARD_MIN, RIGHT_BACKWARD_MAX);
    }
    // turn right backward
    else {
      throttleReduction = mapFloat(steering, 1, 50, 1, 0); 
      leftVal = map(throttle, 0, -49, LEFT_BACKWARD_MIN, LEFT_BACKWARD_MAX);
      rightVal = map(throttle*throttleReduction, 0, -49, RIGHT_BACKWARD_MIN, RIGHT_BACKWARD_MAX);
    }
  }
  
  leftServo.write(leftVal);
  rightServo.write(rightVal);
  
  /*Serial.print("left = "); Serial.print(leftVal);
  Serial.print(" | right = "); Serial.print(rightVal);
  Serial.print(" | r = "); Serial.print(throttleReduction);
  Serial.print(" | steering = "); Serial.print(steering);
  Serial.print(" | throttle = "); Serial.println(throttle); */
}

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  delay(100);
  Serial.println("Setup Complete");

  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);
}


void loop() {
  int throttle = phone.getThrottle();
  int steering = phone.getSteering();
  handleDriving(throttle, steering);
}