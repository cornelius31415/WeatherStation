#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ================= LCD =================

LiquidCrystal_I2C lcd(0x27, 20, 4);

// ================= ESP NOW =================

typedef struct struct_message {

  float bmpTemperature;
  float bmpPressure;
  float dhtHumidity;
  float dhtTemperature;

} struct_message;

struct_message myData;

QueueHandle_t dataQueue;

// ================= CALLBACK =================

void OnDataRecv(const esp_now_recv_info_t *info,
                const uint8_t *incomingData, int len) {

  if (len == sizeof(struct_message)) {

    struct_message receivedData;

    memcpy(&receivedData, incomingData, sizeof(receivedData));

    xQueueOverwrite(dataQueue, &receivedData);
  }
}

// ================= SETUP =================

void setup() {

  Serial.begin(115200);

  dataQueue = xQueueCreate(1, sizeof(struct_message));

  // ESP-NOW starten

  WiFi.mode(WIFI_STA);

  delay(1000);

  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  Serial.print("Empfaenger MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Fehler");
    while (true) {
      delay(1000);
    }
  }

  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Empfaenger bereit");

  // LCD starten

  Wire.begin(21, 22);

  lcd.begin();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Wetterstation");

  lcd.setCursor(0, 1);
  lcd.print("Warte auf Daten...");
}

// ================= LOOP =================

void loop() {

  if (xQueueReceive(dataQueue, &myData, 0) == pdTRUE) {

    // LCD

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("BMP Temp: ");
    lcd.print(myData.bmpTemperature, 1);
    lcd.write(223);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("DHT Temp: ");
    lcd.print(myData.dhtTemperature, 1);
    lcd.write(223);
    lcd.print("C");

    lcd.setCursor(0, 2);
    lcd.print("Humidity: ");
    lcd.print(myData.dhtHumidity, 1);
    lcd.print(" %");

    lcd.setCursor(0, 3);
    lcd.print("Pressure: ");
    lcd.print(myData.bmpPressure, 1);
    lcd.print(" hPa");

    // Serial Monitor

    Serial.println("--------------------------");

    Serial.print("BMP Temperatur: ");
    Serial.println(myData.bmpTemperature);

    Serial.print("DHT Temperatur: ");
    Serial.println(myData.dhtTemperature);

    Serial.print("Luftfeuchtigkeit: ");
    Serial.println(myData.dhtHumidity);

    Serial.print("Luftdruck: ");
    Serial.println(myData.bmpPressure);
  }

  delay(100);
} 
