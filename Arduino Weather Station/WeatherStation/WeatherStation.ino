// ----------------------------------------------------------------------------------------
//                                  IMPORTING LIBRARIES
// ----------------------------------------------------------------------------------------

#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_I2C.h>

// ----------------------------------------------------------------------------------------
//                                  HARDWARE SETUP
// ----------------------------------------------------------------------------------------

const byte dht11_pin = 2;
const byte dht11_type = DHT11;
const byte dht22_pin = 4;
const byte dht22_type = DHT22;
const byte SD_pin = 10;

byte humidityDHT11;
float temperatureDHT11;
byte humidityDHT22;
float temperatureDHT22;

float temperatureBMP1;
float pressure1;
float altitude1;
float temperatureBMP2;
float pressure2;
float altitude2;

int pirPin = 7; // Input for HC-S501 movement sensor
unsigned long lastMotionTime = 0;
const unsigned long lcdOnTime = 10000;
unsigned long lastMeasurement = 0;
const unsigned long measurementInterval = 10000;
// ----------------------------------------------------------------------------------------
//                                          LCD
// ----------------------------------------------------------------------------------------

const byte degreeSymbol[8] PROGMEM = {
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

// ----------------------------------------------------------------------------------------
//                                  CREATING OBJECTS
// ----------------------------------------------------------------------------------------

DHT dht11(dht11_pin, dht11_type);
DHT dht22(dht22_pin, dht22_type);

RTC_DS3231 rtc;
DateTime dt;

Adafruit_BMP280 bmp1;
Adafruit_BMP280 bmp2;

LiquidCrystal_I2C lcd1(0x27, 20, 4);
LiquidCrystal_I2C lcd2(0x23, 16, 2);


// ----------------------------------------------------------------------------------------
//                               SETTING EVERYTHING UP
// ----------------------------------------------------------------------------------------

void setup() {

  pinMode(pirPin,INPUT);
  
  lcd1.begin();
  lcd1.noBacklight();

  lcd2.begin();
  lcd2.noBacklight();

  byte degreeSymbolBuffer[8];
  for (int i = 0; i < 8; i++) {
    degreeSymbolBuffer[i] = pgm_read_byte(&degreeSymbol[i]);
  }
  lcd1.createChar(1, degreeSymbolBuffer);
  lcd2.createChar(1, degreeSymbolBuffer);

  dht11.begin();
  dht22.begin();
  
  rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  rtc.writeSqwPinMode(DS3231_OFF);  // disables square wave and enables clock


  bmp1.begin(0x76);
  bmp2.begin(0x77);

  bmp1.setSampling(Adafruit_BMP280::MODE_NORMAL,
                   Adafruit_BMP280::SAMPLING_X16,
                   Adafruit_BMP280::SAMPLING_X16,
                   Adafruit_BMP280::FILTER_X16,
                   Adafruit_BMP280::STANDBY_MS_500);

  bmp2.setSampling(Adafruit_BMP280::MODE_NORMAL,
                   Adafruit_BMP280::SAMPLING_X16,
                   Adafruit_BMP280::SAMPLING_X16,
                   Adafruit_BMP280::FILTER_X16,
                   Adafruit_BMP280::STANDBY_MS_500);

  SD.begin(SD_pin);
  File weatherData = SD.open("weather.csv", FILE_WRITE);

  if (weatherData) {
    weatherData.println(F("Time,Temperature DHT11,Humidity DHT11 (%),Temperature DHT22,Humidity DHT22 (%),"
                          "Temperature BMP1, Pressure BMP1 (hPa),Altitude BMP1 (m),Temperature BMP2,"
                          "Pressure BMP2 (hPa), Altitude BMP2 (m)"));
    weatherData.close();
  }
}

// ----------------------------------------------------------------------------------------
//                                    MAIN PROGRAM
// ----------------------------------------------------------------------------------------


void loop() {

  if (digitalRead(pirPin) == HIGH) {
    lastMotionTime = millis();
    lcd1.backlight();
    lcd2.backlight();
  }

  if (millis() - lastMotionTime > lcdOnTime){
    lcd1.noBacklight();
    lcd2.noBacklight();
  }

  if (millis() - lastMeasurement >= measurementInterval){
    lastMeasurement = millis();
      dt = rtc.now();

    readSensors();
    printLCD2();



    writeDataToSD();
    printLCD1();
  }
  
  

}

// ----------------------------------------------------------------------------------------
//                                 READING SENSOR DATA
// ----------------------------------------------------------------------------------------

void readSensors() {
  humidityDHT11 = dht11.readHumidity();
  temperatureDHT11 = dht11.readTemperature();

  humidityDHT22 = dht22.readHumidity();
  temperatureDHT22 = dht22.readTemperature();

  temperatureBMP1 = bmp1.readTemperature();
  pressure1 = bmp1.readPressure() / 100.0;
  altitude1 = bmp1.readAltitude();

  temperatureBMP2 = bmp2.readTemperature();
  pressure2 = bmp2.readPressure() / 100.0;
  altitude2 = bmp2.readAltitude();
}

// ----------------------------------------------------------------------------------------
//                                    DISPLAY ON LCD
// ----------------------------------------------------------------------------------------

void printLCD1() {
  // ----------------------------
  // SMALL LCD (Sensor readings)
  // ----------------------------
  lcd1.clear(); // Optional: Clear screen if flickering isn't an issue
  lcd1.setCursor(0, 0);
  lcd1.print(F("Temperature: "));
  lcd1.print(temperatureBMP1);
  lcd1.write(1); // Degree symbol
  lcd1.print(F("C"));

  lcd1.setCursor(0, 1);
  lcd1.print(F("Humidity:    "));
  lcd1.print(round(humidityDHT22));
  lcd1.print(F("%"));

  lcd1.setCursor(0, 2);
  lcd1.print(F("Pressure:    "));
  lcd1.print(round(pressure2));
  lcd1.setCursor(17, 2);
  lcd1.print(F("hPa"));

  lcd1.setCursor(0, 3);
  lcd1.print(F("Time:    "));
  lcd1.setCursor(13, 3);
  if (dt.hour() < 10) lcd1.print("0");
  lcd1.print(dt.hour()); 
  lcd1.print(":");
  if (dt.minute() < 10) lcd1.print("0");
  lcd1.print(dt.minute());
  



}


void printLCD2() {


  lcd2.clear(); 
lcd2.setCursor(0, 0);
lcd2.print(F("Time: "));
if (dt.hour() < 10) lcd2.print("0");
lcd2.print(dt.hour());
  lcd2.print(":");
if (dt.minute() < 10) lcd2.print("0");
lcd2.print(dt.minute());



  lcd2.setCursor(0, 1);
  lcd2.print(F("Temp: "));
  lcd2.print(temperatureBMP1);
  lcd2.write(1); // Degree symbol
  lcd2.print(F("C"));


}


// ----------------------------------------------------------------------------------------
//                                   PUT IT ON SD CARD
// ----------------------------------------------------------------------------------------

void writeDataToSD() {
  File weatherData = SD.open("weather.csv", FILE_WRITE);

  if (weatherData) {
    printDataInCSV(weatherData);
    weatherData.close();
  }
}

void printDataInCSV(File &weatherData) {
  weatherData.print(dt.year());
  weatherData.print(F("-"));
  weatherData.print(dt.month() < 10 ? "0" : ""); weatherData.print(dt.month());
  weatherData.print(F("-"));
  weatherData.print(dt.day() < 10 ? "0" : ""); weatherData.print(dt.day());
  weatherData.print(F(" "));
  weatherData.print(dt.hour() < 10 ? "0" : ""); weatherData.print(dt.hour());
  weatherData.print(F(":"));
  weatherData.print(dt.minute() < 10 ? "0" : ""); weatherData.print(dt.minute());
  weatherData.print(F(":"));
  weatherData.print(dt.second() < 10 ? "0" : ""); weatherData.print(dt.second());
  weatherData.print(F(","));

  weatherData.print(temperatureDHT11); weatherData.print(F(","));
  weatherData.print(humidityDHT11); weatherData.print(F(","));
  weatherData.print(temperatureDHT22); weatherData.print(F(","));
  weatherData.print(humidityDHT22); weatherData.print(F(","));

  weatherData.print(temperatureBMP1); weatherData.print(F(","));
  weatherData.print(pressure1); weatherData.print(F(","));
  weatherData.print(altitude1); weatherData.print(F(","));

  weatherData.print(temperatureBMP2); weatherData.print(F(","));
  weatherData.print(pressure2); weatherData.print(F(","));
  weatherData.print(altitude2);

  weatherData.println();
}
