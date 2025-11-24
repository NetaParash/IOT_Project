#define TILT_PIN 23

volatile bool tiltDetected = false;
unsigned long lastTiltTime = 0;
const unsigned long tiltHoldTime = 80; // ms to keep reading as "1"

void IRAM_ATTR tiltISR() {
  tiltDetected = true;
  lastTiltTime = millis();
}

void setup() {
  Serial.begin(115200);

  pinMode(TILT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TILT_PIN), tiltISR, FALLING);

  Serial.println("Tilt sensor ready");
}

void loop() {
  unsigned long now = millis();

  if (tiltDetected) {
    tiltDetected = false;
  }

  // If a tilt happened recently, show "1"
  if (now - lastTiltTime < tiltHoldTime) {
    Serial.println(1);
  } else {
    Serial.println(0);
  }

  delay(20); // controls print rate
}
