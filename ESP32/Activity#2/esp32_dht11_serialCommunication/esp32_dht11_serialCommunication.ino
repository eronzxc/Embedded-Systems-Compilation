// EXP 2.2 - Activity #2: Serial Communication
// DHT11 Monitoring via Serial Monitor

#include <DHT.h>
#define WOKWI_SIM

#define DHTPIN 32       // DHT11 data pin connected to pin 32

#ifdef WOKWI_SIM
  #define DHTTYPE DHT22
#else
  #define DHTTYPE DHT11
#endif

DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
const long interval = 2000;  // read DHT11 every 2 seconds

void setup() {
  Serial.begin(9600);

  dht.begin();

  Serial.println("=== Activity 2: Serial Communication ===");
  Serial.println("DHT11 readings will print automatically...");
  Serial.println();
}

void loop() {
  // ----- Part B: DHT11 Monitoring -----
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();  // Celsius

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C   Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }
}