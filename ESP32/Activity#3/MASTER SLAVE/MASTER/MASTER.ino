// Activity #3
// 3.2 BT Master-Slave: MASTER (Central)

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// Paste the slave's MAC address here (from the slave's Serial Monitor output)
uint8_t slaveAddress[6] = {0xB4, 0xBF, 0xE9, 0x05, 0xED, 0x7A};

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_MASTER", true);  // true = master mode

  Serial.println("Connecting to slave...");
  if (SerialBT.connect(slaveAddress)) {
    Serial.println("Connected! Type '1' or '0' and press Enter.");
  } else {
    Serial.println("Connection failed - check MAC address / slave is powered on.");
  }
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    SerialBT.write(c);
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
}