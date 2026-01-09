#include "AppClient.h"

AppClient client(
  "OrZ iPhone",
  "g0iibm9ik7ry",
  "https://iot-project-6i3k.onrender.com"
);

void setup() {
  Serial.begin(115200);
  delay(1000);

  client.sendSettings("custom", 2000, 90);

  auto settings = client.getSettings();
  Serial.println(settings[0]); // mode
  Serial.println(settings[1]); // goal
  Serial.println(settings[2]); // alerts_every

  client.sendEvent(1767530023, 130, 370);
}

void loop() {}
