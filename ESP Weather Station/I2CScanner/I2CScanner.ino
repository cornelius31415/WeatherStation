#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
}

void loop() {

  Serial.println("Suche I2C-Geraete...");

  for (int address = 1; address < 127; address++) {

    Wire.beginTransmission(address);

    if (Wire.endTransmission() == 0) {
      Serial.print("Gefunden: 0x");
      Serial.println(address, HEX);
    }
  }

  delay(3000);
}
