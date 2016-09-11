// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

int BPM1 = 0;
int BPM2 = 0;

int DIR_PIN = 8;
int STEP_PIN = 9;

int SWITCH1_PIN = 2;
int SWITCH2_PIN = 3;

int BPM_request_counter = 0;
int servo_step_counter = 0;
int servo_delay = 40;
boolean switch_1 = true;
boolean switch_2 = true;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  
  pinMode(DIR_PIN, OUTPUT);     
  pinMode(STEP_PIN, OUTPUT);
  pinMode(SWITCH1_PIN, INPUT);
  pinMode(SWITCH2_PIN, INPUT);
  
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(STEP_PIN, LOW);
}

void loop() {

  switch_1 = digitalRead(SWITCH1_PIN);
  switch_2 = digitalRead(SWITCH2_PIN);

  if (!switch_1) {
    Serial.println("Player 1 wins");
    gameOver(1);
    while(1){};
  }
  if (!switch_2) {
    Serial.println("Player 2 wins");
    gameOver(2);
    while(1){};
  }

  if (BPM_request_counter >= 5000) {
    Wire.requestFrom(8, 1);    // request 6 bytes from slave device #8
  
    if (Wire.available()) { // slave may send less than requested
      BPM1 = Wire.read(); // receive a byte as character
      Serial.println("BPM1: ");
      Serial.println(BPM1);         // print the character
    }
  
    Wire.requestFrom(9, 1);
  
    if (Wire.available()) {
      BPM2 = Wire.read();
      Serial.println("BPM2: ");
      Serial.println(BPM2);
      Serial.println();
    }

    digitalWrite(DIR_PIN, getDirection());
    servo_delay = getDelay();
    BPM_request_counter = 0;
  }

  if (servo_step_counter >= servo_delay && servo_step_counter < servo_delay * 2) {
    digitalWrite(STEP_PIN, LOW);
  } else if (servo_step_counter > servo_delay * 2) {
    servo_step_counter = 0;
    digitalWrite(STEP_PIN, HIGH);
  }
  
  BPM_request_counter++;
  servo_step_counter++;
  delayMicroseconds(100);
}

boolean getDirection(){
  if (BPM1 > BPM2) {
    return HIGH;
  } else {
    return LOW;
  }
}

int getDelay() {
  int baseSpeed = abs(BPM1 - BPM2);
  if (baseSpeed == 0) {
    return 100;
  } else if (baseSpeed > 30) {
    return 20;    
  } else {
    return 50 - baseSpeed;
  }
}

void gameOver(int winner) {
  if (winner == 1) {
    digitalWrite(DIR_PIN, HIGH);
  } else if (winner == 2) {
    digitalWrite(DIR_PIN, LOW);
  }

  for (int i = 0; i < 5000; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(500);
  }
}

