
const int SENSOR_PIN = 27; // GPIO 27 (Touch 7)

const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
long sum = 0;
int average = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

void loop() {
  sum = sum - readings[readIndex];	// Subtract the last reading from the sum
  
  readings[readIndex] = touchRead(SENSOR_PIN);	// get value of Touch 7 pin = GPIO 27
  
  sum = sum + readings[readIndex];	// Add the new reading to the sum
  
  readIndex = (readIndex+1) % numReadings;	// Increment the index, and wrap to 0 if it exceeds the array size
  
  average = sum / numReadings;

  Serial.print("Raw:");
  Serial.print(readings[readIndex]);
  Serial.print(",");
  Serial.print("Smoothed:");
  Serial.println(average);

  delay(50);
}
