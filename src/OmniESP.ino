#include <PS4Controller.h>

void setup() {

  Serial.begin(115200);
  PS4.begin();
  Serial.println("Ready.");
}

void loop() {
  if (PS4.isConnected()) {
      Serial.printf("%dX", PS4.LStickX());
      Serial.printf("%d\n", PS4.LStickY());
  }
}
