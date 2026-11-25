#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define WIFI_CHANNEL 1

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

bool received = false;
unsigned long lastReceived = 0;

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
// Display
// ============================

void drawLayout() {

  tft.fillScreen(TFT_BLACK);

  tft.fillRect(0, 0, 240, 40, TFT_NAVY);

  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(36, 12);
  tft.print("WETTERSTATION");

  tft.setTextSize(1);
  tft.setTextColor(TFT_LIGHTGREY);

  tft.setCursor(15, 53);
  tft.print("AUSSENTEMPERATUR");

  tft.setCursor(15, 127);
  tft.print("LUFTFEUCHTIGKEIT");

  tft.setCursor(15, 197);
  tft.print("LUFTDRUCK");

  tft.drawFastHLine(15, 115, 210, TFT_DARKGREY);
  tft.drawFastHLine(15, 185, 210, TFT_DARKGREY);
  tft.drawFastHLine(15, 285, 210, TFT_DARKGREY);
}

// ============================
// Setup
// ============================

void setup() {

  Serial.begin(115200);

  tft.init();
  tft.setRotation(0);

  drawLayout();

  WiFi.mode(WIFI_STA);

  esp_wifi_set_channel(
    WIFI_CHANNEL,
    WIFI_SECOND_CHAN_NONE
  );

  Serial.print("Receiver MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    tft.setCursor(15, 300);
    tft.print("ESP-NOW ERROR");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

// ============================
// Loop
// ============================

void loop() {

  WeatherData data;
  bool hasData;
  unsigned long last;

  portENTER_CRITICAL(&mux);

  data = weather;
  hasData = received;
  last = lastReceived;

  portEXIT_CRITICAL(&mux);

  if (!hasData) {

    tft.setTextSize(1);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setCursor(15, 300);
    tft.print("WARTE AUF WETTERDATEN");

    delay(1000);
    return;
  }

  // Temperatur

  tft.setTextSize(4);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setCursor(15, 75);
  tft.printf("%5.1f C ", data.dhtTemperature);

  // Luftfeuchtigkeit

  tft.setTextSize(4);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setCursor(15, 148);
  tft.printf("%5.1f %% ", data.dhtHumidity);

  // Luftdruck

  float pressure = data.bmpPressure;

  // Falls Druck in Pascal gesendet wird
  if (pressure > 2000) pressure /= 100.0;

  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(15, 220);
  tft.printf("%.1f hPa  ", pressure);

  // BMP280 Temperatur

  tft.setTextSize(2);
  tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  tft.setCursor(15, 258);
  tft.printf("BMP280: %.1f C  ", data.bmpTemperature);

  // Zeit seit letztem Empfang

  unsigned long seconds = (millis() - last) / 1000;

  tft.setTextSize(1);
  tft.setTextColor(
    seconds < 120 ? TFT_GREEN : TFT_RED,
    TFT_BLACK
  );

  tft.setCursor(15, 300);
  tft.printf("LETZTER EMPFANG: %lu s     ", seconds);

  delay(1000);
}
