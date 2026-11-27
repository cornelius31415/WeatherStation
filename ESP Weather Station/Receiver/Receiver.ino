#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <TFT_eSPI.h>
#include <DHT.h>

// ============================
// Einstellungen
// ============================

#define WIFI_CHANNEL 1
#define DHT_PIN 27
#define DHT_TYPE DHT22

TFT_eSPI tft = TFT_eSPI();
DHT dht(DHT_PIN, DHT_TYPE);

// ============================
// Wetterdaten
// ============================

typedef struct {
  float bmpTemperature;
  float bmpPressure;
  float dhtHumidity;
  float dhtTemperature;
} WeatherData;

WeatherData weather;

float indoorTemperature = NAN;
float indoorHumidity = NAN;

bool received = false;

unsigned long lastReceived = 0;
unsigned long lastDHT = 0;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

// ============================
// ESP-NOW Empfang
// ============================

void OnDataRecv(const esp_now_recv_info_t *info,
                const uint8_t *data, int len) {

  if (len != sizeof(WeatherData)) return;

  portENTER_CRITICAL(&mux);

  memcpy(&weather, data, sizeof(weather));

  received = true;
  lastReceived = millis();

  portEXIT_CRITICAL(&mux);
}

// ============================
// Display Layout
// ============================

void drawLayout() {

  tft.fillScreen(TFT_BLACK);

  tft.fillRect(0, 0, 240, 34, TFT_NAVY);

  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(36, 9);
  tft.print("WETTERSTATION");

  tft.setTextSize(1);
  tft.setTextColor(TFT_LIGHTGREY);

  tft.setCursor(12, 45);
  tft.print("AUSSEN");

  tft.setCursor(12, 141);
  tft.print("INNEN");

  tft.setCursor(12, 237);
  tft.print("LUFTDRUCK");

  tft.drawFastHLine(10, 130, 220, TFT_DARKGREY);
  tft.drawFastHLine(10, 226, 220, TFT_DARKGREY);
  tft.drawFastHLine(10, 289, 220, TFT_DARKGREY);
}

// ============================
// Setup
// ============================

void setup() {

  Serial.begin(115200);

  dht.begin();

  tft.init();
  tft.setRotation(2);

  drawLayout();

  WiFi.mode(WIFI_STA);

  esp_wifi_set_channel(
    WIFI_CHANNEL,
    WIFI_SECOND_CHAN_NONE
  );

  Serial.print("Receiver MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW ERROR");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

// ============================
// Loop
// ============================

void loop() {

  // Innensensor alle 2,5 Sekunden auslesen

  if (millis() - lastDHT >= 2500) {

    indoorTemperature = dht.readTemperature();
    indoorHumidity = dht.readHumidity();

    lastDHT = millis();
  }

  // Empfangene Daten kopieren

  WeatherData data;
  bool hasData;
  unsigned long last;

  portENTER_CRITICAL(&mux);

  data = weather;
  hasData = received;
  last = lastReceived;

  portEXIT_CRITICAL(&mux);

  // ==========================
  // Aussentemperatur
  // ==========================

  tft.setTextSize(4);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setCursor(12, 65);

  if (hasData)
    tft.printf("%5.1f C  ", data.dhtTemperature);
  else
    tft.print(" --.- C  ");

  // Aussenluftfeuchtigkeit

  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(12, 105);

  if (hasData)
    tft.printf("Feuchte: %.1f %%  ", data.dhtHumidity);
  else
    tft.print("Feuchte: --.- %  ");

  // ==========================
  // Innentemperatur
  // ==========================

  tft.setTextSize(4);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setCursor(12, 161);

  if (!isnan(indoorTemperature))
    tft.printf("%5.1f C  ", indoorTemperature);
  else
    tft.print(" --.- C  ");

  // Innenluftfeuchtigkeit

  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(12, 201);

  if (!isnan(indoorHumidity))
    tft.printf("Feuchte: %.1f %%  ", indoorHumidity);
  else
    tft.print("Feuchte: --.- %  ");

  // ==========================
  // Luftdruck
  // ==========================

  float pressure = data.bmpPressure;

  if (pressure > 2000)
    pressure /= 100.0;

  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(12, 256);

  if (hasData)
    tft.printf("%.1f hPa  ", pressure);
  else
    tft.print("----.- hPa");

  // ==========================
  // Empfangsstatus
  // ==========================

  tft.setTextSize(1);
  tft.setCursor(12, 303);

  if (hasData) {

    unsigned long seconds = (millis() - last) / 1000;

    tft.setTextColor(
      seconds < 120 ? TFT_GREEN : TFT_RED,
      TFT_BLACK
    );

    tft.printf("LETZTER EMPFANG: %lu s    ", seconds);

  } else {

    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("WARTE AUF WETTERDATEN    ");
  }

  delay(500);
}
