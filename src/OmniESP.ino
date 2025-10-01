#include <PS4Controller.h>

void setup() {

  Serial.begin(115200);
  PS4.begin();
  Serial.println("Ready.");
}

void loop() {
  if (PS4.isConnected()) {
  Serial.printf("%dX%dY%dR%dL", PS4.LStickX(),PS4.LStickY(),PS4.R2Value(),PS4.L2Value());
  }
}
