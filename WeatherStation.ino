
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

const byte dht11_pin = 2;       // sensor will be attached to pin 2 on Arduino
const unsigned int dht11_type = DHT11; // type of DHT sensor
const byte dht22_pin = 4;
const unsigned int dht22_type = DHT22;

const byte SD_pin = 10;      // Arduino Pin for CS Pin of SD card module

byte humidityDHT11;             // the shit I wanna measure
float temperatureDHT11;
byte humidityDHT22;             
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

// defining the degree symbol, because it does not exist in the LCD screen
const byte degreeSymbol[8] PROGMEM  ={
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

// DHT HUMIDITY AND TEMPERATURE SENSORS
DHT dht11(dht11_pin, dht11_type); // creating a DHT11 object
DHT dht22(dht22_pin, dht22_type); // creating a DHT22 object

// DS1307 TIME MODULE
DS3231 clock;
RTCDateTime dt;

// BMP280 PRESSURE AND TEMPERATURE SENSORS
Adafruit_BMP280 bmp1;
Adafruit_BMP280 bmp2;

// LCD SCREEN
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ----------------------------------------------------------------------------------------
//                               SETTING EVERYTHING UP
// ----------------------------------------------------------------------------------------

void setup() {

    // setting up the LCD screen
    lcd.begin(16,2);
    lcd.backlight();  
    // getting the degree symbol from flash memory and putting into RAM to work with it
    byte degreeSymbolBuffer[8];
    for (int i=0; i<8; i++){
        degreeSymbolBuffer[i] = pgm_read_byte(&degreeSymbol[i]);
    }
    
    lcd.createChar(1,degreeSymbolBuffer);

    // setting up the DHT sensors
    dht11.begin();
    dht22.begin();
    
    // setting up the time module
    clock.begin();
    clock.setDateTime(__DATE__, __TIME__);
    
    //setting up and configuring the BMP280 sensors
    bmp1.begin(0x76);
    bmp2.begin(0x77);
    bmp1.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Normal Mode
                     Adafruit_BMP280::SAMPLING_X16,     // Temperature-Oversampling: x16
                     Adafruit_BMP280::SAMPLING_X16,    // Pressure-Oversampling: x16
                     Adafruit_BMP280::FILTER_X16,      // Filter: x16
                     Adafruit_BMP280::STANDBY_MS_500); // Standby-Time: 500ms
                    
    bmp2.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Normal Mode
                     Adafruit_BMP280::SAMPLING_X16,     // Temperature-Oversampling: x16
                     Adafruit_BMP280::SAMPLING_X16,    // Pressure-Oversampling: x16
                     Adafruit_BMP280::FILTER_X16,      // Filter: x16
                     Adafruit_BMP280::STANDBY_MS_500); // Standby-Time: 500ms
    
    
    // setting up the SD card module and open a csv-file
    SD.begin(SD_pin);
    File weatherData = SD.open("weather.csv",FILE_WRITE);
    
    
    
    // only if file has been opened successfully we write the header (the column names)
    if(weatherData){                                    
    
         // print the header (column names) of the dataset
        weatherData.println(F("Time,Temperature DHT11,Humidity DHT11 (%),Temperature DHT22,Humidity DHT22 (%),"
                            "Temperature BMP1, Pressure BMP1 (hPa),Altitude BMP1 (m),Temperature BMP2,"
                            "Pressure BMP2 (hPa), Altitude BMP2 (m)"));  
                            
        // and close the data set
        weatherData.close();                               
    
    }
    

}

// ----------------------------------------------------------------------------------------
//                                    MAIN PROGRAMM
// ----------------------------------------------------------------------------------------

void loop() {

    dt = clock.getDateTime();
    
    readSensors();                    // read the input values from the sensors
    
    writeDataToSD();                  // write the data onto SD card
    
    printLCD();                       // also print part of the data on the LCD screen
    
    delay(2000);                      // waiting time till next measurement

}

// ----------------------------------------------------------------------------------------
//                                 READING SENSOR DATA
// ----------------------------------------------------------------------------------------

void readSensors(){

    // get the data from DHT11
    humidityDHT11 = dht11.readHumidity();
    temperatureDHT11 = dht11.readTemperature();

    // get the data from DHT22
    humidityDHT22 = dht22.readHumidity();
    temperatureDHT22 = dht22.readTemperature();

    // get the data from first BMP280
    temperatureBMP1 = bmp1.readTemperature();
    pressure1 = bmp1.readPressure()/100.0;
    altitude1 = bmp1.readAltitude();

    // get the data from second BMP280
    temperatureBMP2 = bmp2.readTemperature();
    pressure2 = bmp2.readPressure()/100.0;
    altitude2 = bmp2.readAltitude();
}

// ----------------------------------------------------------------------------------------
//                                    DISPLAY ON LCD
// ----------------------------------------------------------------------------------------

void printLCD(){
  
    // printing the temperature and the pressure from the BMP280 sensor
    // and the humidity from the DHT22 sensor

    // temperature
    lcd.setCursor(0,0);
    lcd.print(F("T:"));
    lcd.setCursor(2,0);
    lcd.print(temperatureBMP2);
    lcd.setCursor(7,0);
    lcd.write(1);
    lcd.setCursor(8,0);
    lcd.print(F("C"));

    // humidity
    lcd.setCursor(11,0);
    lcd.print(F("H:"));
    lcd.setCursor(13,0);
    lcd.print(round(humidityDHT22));
    lcd.setCursor(15,0);
    lcd.print(F("%"));

    // pressure
    lcd.setCursor(0,1);
    lcd.print(F("P:"));
    lcd.setCursor(2,1);
    lcd.print(pressure2);
    lcd.setCursor(9,1);
    lcd.print(F("hPa"));

}

// ----------------------------------------------------------------------------------------
//                                   PUT IT ON SD CARD
// ----------------------------------------------------------------------------------------

void writeDataToSD(){

    // open the csv-file
    File weatherData = SD.open("weather.csv",FILE_WRITE);
    
    if (weatherData){
    
        printDataInCSV(weatherData);  // print gathered data into csv-file
        weatherData.close();          // and close the file
    
    }
}


void printDataInCSV(File &weatherData){
  
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
    
}
