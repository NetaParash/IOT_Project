#define TILT_PIN 23

volatile bool tilted = false;
volatile unsigned long tiltCount = 0;

void IRAM_ATTR tiltISR() {
  tilted = true;
}

void setup() {
  Serial.begin(115200);

  pinMode(TILT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TILT_PIN), tiltISR, FALLING);

  Serial.println("Tilt sensor ready");
}

void loop() {
  if (tilted) {
    tilted = false;

    //delay(10); // debounce

    if (digitalRead(TILT_PIN) == LOW) {
      tiltCount++;
      Serial.print("Tilt detected! Total: ");
      Serial.println(tiltCount);
    }
  }
}
