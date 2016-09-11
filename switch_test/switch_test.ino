
int SWITCH1_PIN = 2;
int SWITCH2_PIN = 3;

boolean switch_1 = true;
boolean switch_2 = true;

void setup() {
  Serial.begin(9600);
  pinMode(SWITCH1_PIN, INPUT);
  pinMode(SWITCH2_PIN, INPUT);
}

void loop() {
  switch_1 = digitalRead(SWITCH1_PIN);
  switch_2 = digitalRead(SWITCH2_PIN);

  if (!switch_1) {
    // move HIGH
    
    Serial.println("Player 1 wins");
  }
  if (!switch_2) {
    // move LOW
    Serial.println("Player 2 wins");
  }
}
