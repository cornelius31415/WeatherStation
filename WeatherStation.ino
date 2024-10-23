
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
// ----------------------------------------------------------------------------------------
//                                  HARDWARE SETUP
// ----------------------------------------------------------------------------------------

const int dht_pin = 2;       // sensor will be attached to pin 2 on Arduino
const int dht_type = DHT11; // type of DHT sensor

const int SD_pin = 10;      // Arduino Pin for CS Pin of SD card module

float humidity;             // the shit I wanna measure
float temperatureDHT;
float temperatureBMP;
float pressure;
float altitude;
// ----------------------------------------------------------------------------------------
//                                  CREATING OBJECTS
// ----------------------------------------------------------------------------------------

DHT dht(dht_pin, dht_type); // creating a DHT object
DS3231 clock;
RTCDateTime dt;
Adafruit_BMP280 bmp;

void setup() {

dht.begin();
clock.begin();
bmp.begin(0x76);

clock.setDateTime(__DATE__, __TIME__);

SD.begin(SD_pin);
File weatherData = SD.open("weather.csv",FILE_WRITE);


if(weatherData){                                    // check if file has been opened successfully

weatherData.println("Time,Temperature DHT,Humidity DHT (%), Temperature BMP, Pressure BMP (hPa), Altitude BMP (m)");   // print the header (column names) of the dataset

weatherData.close();                                // close the data set


}



//configuring the BMP280 sensor
bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Normaler Modus
                Adafruit_BMP280::SAMPLING_X2,     // Temperatur-Oversampling: x2
                Adafruit_BMP280::SAMPLING_X16,    // Druck-Oversampling: x16
                Adafruit_BMP280::FILTER_X16,      // Filter: x16
                Adafruit_BMP280::STANDBY_MS_500); // Standby-Zeit: 500ms


}


void loop() {

dt = clock.getDateTime();

humidity = dht.readHumidity();
temperatureDHT = dht.readTemperature();
delay(100);
temperatureBMP = bmp.readTemperature();
pressure = bmp.readPressure()/100.0;
altitude = bmp.readAltitude();

File weatherData = SD.open("weather.csv",FILE_WRITE);

if (weatherData){
  
weatherData.print(dt.year);
weatherData.print("-");
weatherData.print(dt.month <10? "0": "");
weatherData.print(dt.month);
weatherData.print("-");
weatherData.print(dt.day <10? "0": "");
weatherData.print(dt.day);
weatherData.print(" ");
weatherData.print(dt.hour <10? "0": "");
weatherData.print(dt.hour);
weatherData.print(":");
weatherData.print(dt.minute <10? "0": "");
weatherData.print(dt.minute);
weatherData.print(":");
weatherData.print(dt.second <10? "0": "");
weatherData.print(dt.second);
weatherData.print(",");
weatherData.print(temperatureDHT);
weatherData.print(",");
weatherData.print(humidity);
weatherData.print(",");
weatherData.print(temperatureBMP);
weatherData.print(",");  
weatherData.print(pressure);
weatherData.print(",");  
weatherData.print(altitude);
weatherData.println();
 

weatherData.close();
}

delay(2000);                                        // DHT11 sensor is slow with measuring

}
