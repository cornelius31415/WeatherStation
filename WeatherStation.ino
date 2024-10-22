
// ----------------------------------------------------------------------------------------
//                                  IMPORTING LIBRARIES
// ----------------------------------------------------------------------------------------

# include <DHT.h> // library to get data from DHT sensors
#include <SPI.h>
#include <SD.h>

// ----------------------------------------------------------------------------------------
//                                  HARDWARE SETUP
// ----------------------------------------------------------------------------------------

const int dht_pin = 2;       // sensor will be attached to pin 2 on Arduino
const int dht_type = DHT11; // type of DHT sensor

const int SD_pin = 10;      // Arduino Pin for CS Pin of SD card module

float humidity;             
float temperature;

DHT dht(dht_pin, dht_type); // creating a DHT object


void setup() {

Serial.begin(9600);
dht.begin();

SD.begin(SD_pin);
File weatherData = SD.open("weather.csv",FILE_WRITE);

// check if file has been opened successfully
if(weatherData){
// print the header (column names) of the dataset
weatherData.println("Time,Temperature,Humidity");
// close the data set
weatherData.close();
// and let everyone know on the serial monitor that it worked wuhu
Serial.println("Data has been stored succesfully");

}

else{
  // if file has not been opened successfully please tell the people
  Serial.println("Could not open");
}


}




void loop() {
delay(5000);        // the sensor is a bit slow in measuring

humidity = dht.readHumidity();
temperature = dht.readTemperature();

Serial.print("Humidity:");
Serial.print(humidity);
Serial.println(" %");
Serial.print("Temperature:");
Serial.print(temperature);
Serial.println(" degrees");

}
