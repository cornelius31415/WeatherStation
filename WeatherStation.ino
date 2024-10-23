
// ----------------------------------------------------------------------------------------
//                                  IMPORTING LIBRARIES
// ----------------------------------------------------------------------------------------

# include <DHT.h> // library to get data from DHT sensors
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <DS3231.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_I2C.h>
// ----------------------------------------------------------------------------------------
//                                  HARDWARE SETUP
// ----------------------------------------------------------------------------------------

const unsigned int dht11_pin = 2;       // sensor will be attached to pin 2 on Arduino
const unsigned int dht11_type = DHT11; // type of DHT sensor
const unsigned int dht22_pin = 4;
const unsigned int dht22_type = DHT22;

const int SD_pin = 10;      // Arduino Pin for CS Pin of SD card module

float humidityDHT11;             // the shit I wanna measure
float temperatureDHT11;
float humidityDHT22;             
float temperatureDHT22;

float temperatureBMP1;
float pressure1;
float altitude1;
float temperatureBMP2;
float pressure2;
float altitude2;

// ----------------------------------------------------------------------------------------
//                                          LCD
// ----------------------------------------------------------------------------------------

byte degreeSymbol[8]={
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

DHT dht11(dht11_pin, dht11_type); // creating a DHT object
DHT dht22(dht22_pin, dht22_type);
DS3231 clock;
RTCDateTime dt;
Adafruit_BMP280 bmp1;
Adafruit_BMP280 bmp2;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

lcd.begin(16,2);
lcd.backlight();  
lcd.createChar(1,degreeSymbol);

dht11.begin();
dht22.begin();
clock.begin();
bmp1.begin(0x76);
bmp2.begin(0x77);

clock.setDateTime(__DATE__, __TIME__);

SD.begin(SD_pin);
File weatherData = SD.open("weather.csv",FILE_WRITE);


if(weatherData){                                    // check if file has been opened successfully

weatherData.println(F("Time,Temperature DHT11,Humidity DHT11 (%),Temperature DHT22,Humidity DHT22 (%),"
                    "Temperature BMP1, Pressure BMP1 (hPa),"
                    "Altitude BMP1 (m),Temperature BMP2, Pressure BMP2 (hPa), Altitude BMP2 (m)"));   // print the header (column names) of the dataset

weatherData.close();                                // close the data set


}



//configuring the BMP280 sensor
bmp1.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Normaler Modus
                Adafruit_BMP280::SAMPLING_X2,     // Temperatur-Oversampling: x2
                Adafruit_BMP280::SAMPLING_X16,    // Druck-Oversampling: x16
                Adafruit_BMP280::FILTER_X16,      // Filter: x16
                Adafruit_BMP280::STANDBY_MS_500); // Standby-Zeit: 500ms
                
bmp2.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Normaler Modus
                Adafruit_BMP280::SAMPLING_X2,     // Temperatur-Oversampling: x2
                Adafruit_BMP280::SAMPLING_X16,    // Druck-Oversampling: x16
                Adafruit_BMP280::FILTER_X16,      // Filter: x16
                Adafruit_BMP280::STANDBY_MS_500); // Standby-Zeit: 500ms


}


void loop() {

dt = clock.getDateTime();

humidityDHT11 = dht11.readHumidity();
temperatureDHT11 = dht11.readTemperature();
humidityDHT22 = dht22.readHumidity();
temperatureDHT22 = dht22.readTemperature();
delay(100);

temperatureBMP1 = bmp1.readTemperature();
pressure1 = bmp1.readPressure()/100.0;
altitude1 = bmp1.readAltitude();
delay(10);
temperatureBMP2 = bmp2.readTemperature();
pressure2 = bmp2.readPressure()/100.0;
altitude2 = bmp2.readAltitude();

File weatherData = SD.open("weather.csv",FILE_WRITE);

if (weatherData){

// data from the DS1307 time module
weatherData.print(dt.year);
weatherData.print(F("-"));
weatherData.print(dt.month <10? "0": "");
weatherData.print(dt.month);
weatherData.print(F("-"));
weatherData.print(dt.day <10? "0": "");
weatherData.print(dt.day);
weatherData.print(F(" "));
weatherData.print(dt.hour <10? "0": "");
weatherData.print(dt.hour);
weatherData.print(F(":"));
weatherData.print(dt.minute <10? "0": "");
weatherData.print(dt.minute);
weatherData.print(F(":"));
weatherData.print(dt.second <10? "0": "");
weatherData.print(dt.second);
weatherData.print(F(","));

// data from DHT11 sensor
weatherData.print(temperatureDHT11);
weatherData.print(F(","));
weatherData.print(humidityDHT11);
weatherData.print(F(","));

// data from DHT22 sensor
weatherData.print(temperatureDHT22);
weatherData.print(F(","));
weatherData.print(humidityDHT22);
weatherData.print(F(","));

// data from first BMP280 sensor
weatherData.print(temperatureBMP1);
weatherData.print(F(","));  
weatherData.print(pressure1);
weatherData.print(F(","));  
weatherData.print(altitude1);
weatherData.print(F(","));

// data from second BMP280 sensor
weatherData.print(temperatureBMP2);
weatherData.print(F(","));  
weatherData.print(pressure2);
weatherData.print(F(","));  
weatherData.print(altitude2);

// finish the line
weatherData.println();
 
// and close the file
weatherData.close();
}

// printing the temperature and the pressure from the BMP280 sensor
// and the humidity from the DHT22 sensor
lcd.setCursor(0,0);
lcd.print(F("T:"));
lcd.setCursor(2,0);
lcd.print(temperatureBMP2);
lcd.setCursor(7,0);
lcd.write(1);
lcd.setCursor(8,0);
lcd.print(F("C"));

lcd. setCursor(11,0);
lcd.print(F("H:"));
lcd.setCursor(13,0);
lcd.print(round(humidityDHT22));
lcd.setCursor(15,0);
lcd.print(F("%"));

lcd.setCursor(0,1);
lcd.print(F("P:"));
lcd.setCursor(2,1);
lcd.print(pressure2);
lcd.setCursor(9,1);
lcd.print(F("hPa"));

delay(2000);                                        // DHT11 sensor is slow with measuring

}
