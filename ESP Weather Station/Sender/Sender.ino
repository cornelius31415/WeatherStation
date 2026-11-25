// OUTSIDE MEASUREMENT DEVICE
// BMP280 + DHT22 + ESP-NOW
// Low Power without Deep Sleep

#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include "DHT.h"


// ================================ BMP280 ==================================

#define BMP280_I2C_ADDRESS 0x76

Adafruit_BMP280 bmp;


// ================================ DHT22 ==================================

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);


// ================================ ESP NOW ================================

uint8_t receiverAddress[] = {
  0xB4, 0xBF, 0xE9, 0xDD, 0x3B, 0xE8
};


typedef struct struct_message {

  float bmpTemperature;
  float bmpPressure;

  float dhtHumidity;
  float dhtTemperature;

} struct_message;


struct_message myData;

esp_now_peer_info_t peerInfo;


// ================================ CALLBACK ================================

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {

  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Daten erfolgreich gesendet");
  }
  else {
    Serial.println("Senden fehlgeschlagen");
  }

}


// ================================ SETUP ==================================

void setup() {

  Serial.begin(115200);

  // CPU langsamer für Strom sparen
  setCpuFrequencyMhz(80);

  // Bluetooth aus
  btStop();


  Wire.begin();


  // BMP280 starten
  if (!bmp.begin(BMP280_I2C_ADDRESS)) {

    Serial.println("BMP280 nicht gefunden!");

    while(true);
  }


  dht.begin();



  // ================= ESP NOW =================

  WiFi.mode(WIFI_STA);

  delay(100);


  Serial.print("Sender MAC: ");
  Serial.println(WiFi.macAddress());


  if (esp_now_init() != ESP_OK) {

    Serial.println("ESP-NOW Fehler");

    while(true);
  }


  esp_now_register_send_cb(OnDataSent);



  memcpy(peerInfo.peer_addr, receiverAddress, 6);

  peerInfo.channel = 0;
  peerInfo.encrypt = false;



  if (esp_now_add_peer(&peerInfo) != ESP_OK) {

    Serial.println("Peer Fehler");

    while(true);
  }


}



// ================================ LOOP ==================================

void loop() {


  Serial.println("----------------");
  Serial.println("Neue Messung");


  // BMP280 lesen

  myData.bmpTemperature = bmp.readTemperature();

  myData.bmpPressure =
      bmp.readPressure() / 100.0F;



  // DHT22 lesen

  myData.dhtHumidity =
      dht.readHumidity();

  myData.dhtTemperature =
      dht.readTemperature();



  if (isnan(myData.dhtHumidity) ||
      isnan(myData.dhtTemperature)) {

    Serial.println("DHT Fehler");

  }
  else {


    Serial.print("BMP Temp: ");
    Serial.println(myData.bmpTemperature);

    Serial.print("Pressure: ");
    Serial.println(myData.bmpPressure);

    Serial.print("DHT Temp: ");
    Serial.println(myData.dhtTemperature);

    Serial.print("Humidity: ");
    Serial.println(myData.dhtHumidity);



    esp_err_t result = esp_now_send(
      receiverAddress,
      (uint8_t *)&myData,
      sizeof(myData)
    );


    if(result == ESP_OK) {

      Serial.println("Senden gestartet");

    }
    else {

      Serial.println("ESP-NOW Fehler");

    }

  }


  // 10 Sekunden warten
  delay(10000);

}
