// Activity #4
// 4.1 DHT11 Monitoring Using OLED Display

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ---------- Pin / Device Definitions ----------
#define DHT_PIN   32
#define WOKWI_SIM

#ifdef WOKWI_SIM
  #define DHT_TYPE  DHT22
#else
  #define DHT_TYPE  DHT11
#endif

#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  64
#define OLED_RESET     -1     // No dedicated reset pin (shares ESP32 reset)
#define SCREEN_ADDRESS 0x3C  // Change to 0x3D if display stays blank

// ---------- Timing ----------
const unsigned long READ_INTERVAL_MS = 2000; // DHT11 min. sampling interval
unsigned long lastReadTime = 0;

// ---------- Objects ----------
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  delay(200);

  dht.begin();

  // Initialize I2C on the default ESP32 pins (SDA=21, SCL=22)
  Wire.begin(33, 25);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("OLED init failed - check wiring/address."));
    while (true) { delay(1000); } // halt: nothing useful to do without display
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("DHT11 + OLED Monitor"));
  display.println(F("Initializing..."));
  display.display();
  delay(1000);
}

void loop() {
  unsigned long now = millis();

  if (now - lastReadTime >= READ_INTERVAL_MS) {
    lastReadTime = now;

    float humidity    = dht.readHumidity();
    float temperatureC = dht.readTemperature();        // Celsius
    float temperatureF = dht.readTemperature(true);    // Fahrenheit

    if (isnan(humidity) || isnan(temperatureC)) {
      Serial.println(F("Failed to read from DHT11 sensor."));
      showError();
      return;
    }

    // Log to Serial Monitor
    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.print(F(" %  Temp: "));
    Serial.print(temperatureC);
    Serial.print(F(" C / "));
    Serial.print(temperatureF);
    Serial.println(F(" F"));

    showReadings(temperatureC, temperatureF, humidity);
  }
}

void showReadings(float tempC, float tempF, float hum) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("DHT11 Monitor (I2C OLED)"));
  display.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print(tempC, 1);
  display.print(F(" C"));

  display.setCursor(0, 44);
  display.print(hum, 0);
  display.print(F(" %"));

  display.setTextSize(1);
  display.setCursor(75, 27);
  display.print(tempF, 1);
  display.print(F(" F"));

  display.display();
}

void showError() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("DHT11 Monitor"));
  display.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);
  display.setCursor(0, 25);
  display.println(F("Sensor read error!"));
  display.println(F("Check wiring."));
  display.display();
}
