
// ----------------------------------------------------------------------------------------
//                                  IMPORTING LIBRARIES
// ----------------------------------------------------------------------------------------

# include <DHT.h> // library to get data from DHT sensors
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <DS3231.h>

// ----------------------------------------------------------------------------------------
//                                  HARDWARE SETUP
// ----------------------------------------------------------------------------------------

const int dht_pin = 2;       // sensor will be attached to pin 2 on Arduino
const int dht_type = DHT11; // type of DHT sensor

const int SD_pin = 10;      // Arduino Pin for CS Pin of SD card module

float humidity;             // the shit I wanna measure
float temperature;

// ----------------------------------------------------------------------------------------
//                                  CREATING OBJECTS
// ----------------------------------------------------------------------------------------

DHT dht(dht_pin, dht_type); // creating a DHT object
DS3231 clock;
RTCDateTime dt;


void setup() {

Serial.begin(9600);
dht.begin();
clock.begin();

clock.setDateTime(__DATE__, __TIME__);

SD.begin(SD_pin);
File weatherData = SD.open("weather.csv",FILE_WRITE);


if(weatherData){                                    // check if file has been opened successfully

weatherData.println("Time,Temperature,Humidity");   // print the header (column names) of the dataset

weatherData.close();                                // close the data set


}



}


void loop() {

dt = clock.getDateTime();

humidity = dht.readHumidity();
temperature = dht.readTemperature();

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
weatherData.print(temperature);
weatherData.print(",");
weatherData.println(humidity);
weatherData.close();
  
}

delay(5000);                                        // sensor is slow with measuring

}
