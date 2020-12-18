
//DHT22 Setup - The Humidity Sensor
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//SD Card Setup
#include <SPI.h>
#include <SD.h>
File sdcard_file;
int CS_pin = 10;

//DS18B20 Temperature sensor
#include <OneWire.h>
#include<DallasTemperature.h>
#define ONE_WIRE_BUS 3 //change 3 to whatever digital pin you are using
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
int scale = 3; // 3 (±3g) for ADXL337, 200 (±200g) for ADXL377
boolean micro_is_5V = true; // Set to true if using a 5V microcontroller such as the Arduino Uno, false if using a 3.3V microcontroller, this affects the interpretation of the sensor data

// Altimeter Setup - MPRLS is bad unless you are doing altitude readings >30,000 ft
#include "Adafruit_MPRLS.h"
#define RESET_PIN  -1  // set to any GPIO pin # to hard-reset on begin()
#define EOC_PIN    -1  // set to any GPIO pin to read end-of-conversion by pin
Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);

//LED
int Blue = 7;
int Red = 6;
int Green = 5;
int Status = 2;
int save = 37;


void setup() {
pinMode(2, OUTPUT);
digitalWrite(2, HIGH);

if (! mpr.begin()) {
    digitalWrite(Red, HIGH);
  digitalWrite(Green, HIGH);
  digitalWrite(Blue, HIGH);
    while (1) {
      delay(10000);
    }
  }
  digitalWrite(Green, HIGH);
  digitalWrite(Blue, HIGH);
  delay(1000);
  


  
pinMode(save, INPUT); //detects if a switch is turned on to stop the sdcard writing and to save the data
sensors.begin();
pinMode(Blue, OUTPUT);
pinMode(Red, OUTPUT);
pinMode(Green, OUTPUT);
  
digitalWrite(Green, HIGH);


  dht.begin();

  pinMode(CS_pin, OUTPUT);//declaring CS pin as output pin
  if (SD.begin()){
  
    
    digitalWrite(Green, LOW);
    digitalWrite(Green, HIGH);
    digitalWrite(Green, LOW);
    digitalWrite(Green, HIGH);
    digitalWrite(Green, LOW);
    digitalWrite(Green, HIGH);
  } else
  {
    digitalWrite(Green, LOW);
    digitalWrite(Red, HIGH);
    delay(5000);
    return;
  }
  sdcard_file = SD.open("data.csv", FILE_WRITE);
  if (sdcard_file) { //If the file is found
    
    digitalWrite(Red, LOW);
    digitalWrite(Blue, LOW);
    digitalWrite(Green, HIGH);
    
  }
  else {
    
     digitalWrite(Green, LOW);
    digitalWrite(Red, HIGH);
    
    delay(1000);
    digitalWrite(Red, LOW);
    digitalWrite(Blue, HIGH);
    delay(1000);
     digitalWrite(Green, LOW);
    digitalWrite(Red, HIGH);
    
    delay(1000);
    digitalWrite(Red, LOW);
    digitalWrite(Blue, HIGH);
    delay(1000);
    digitalWrite(Red, HIGH);
    digitalWrite(Blue, LOW);
    
  }
  

sdcard_file.print("DS18B20");
sdcard_file.print(",");
sdcard_file.print("DHT22 Temp");
sdcard_file.print(",");
sdcard_file.print("Hum");
sdcard_file.print(",");
sdcard_file.print("H.InX");
sdcard_file.print(",");
sdcard_file.print("rawX");
sdcard_file.print(",");
sdcard_file.print("rawY");
sdcard_file.print(",");
sdcard_file.print("rawZ");
sdcard_file.print(",");
sdcard_file.print("scaledX");
sdcard_file.print(",");
sdcard_file.print("scaledY");
sdcard_file.print(",");
sdcard_file.print("scaledZ");
sdcard_file.print(",");
sdcard_file.print(" ");
sdcard_file.print("MPRLS");
sdcard_file.print(",");
sdcard_file.println(" ");


}


void loop() {

 delay(222);
 digitalWrite(Green, LOW);
 digitalWrite(Blue, HIGH);
 
 //MPRLS
 float hPa = mpr.readPressure();
 
  
  


 //Accelerometer
  int rawX = analogRead(A0);
  int rawY = analogRead(A1);
  int rawZ = analogRead(A2);
 
  float scaledX, scaledY, scaledZ; 
  if (micro_is_5V) //scaling data if voltage is 5V (it is)
  {
    scaledX = mapf(rawX, 0, 675, -scale, scale); 
    scaledY = mapf(rawY, 0, 675, -scale, scale);
    scaledZ = mapf(rawZ, 0, 675, -scale, scale);
  }
  else //scaling data if voltage is 3.3V
  {
    scaledX = mapf(rawX, 0, 1023, -scale, scale);
    scaledY = mapf(rawY, 0, 1023, -scale, scale);
    scaledZ = mapf(rawZ, 0, 1023, -scale, scale);
  }
  


//DS18B20 Temp
sensors.requestTemperatures();
int etemp=sensors.getTempCByIndex(0);



 //DS18B20 Humidity Sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early for DHT
  if (isnan(h) || isnan(t) || isnan(f)) {
    
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);


  sdcard_file.print(etemp);
  sdcard_file.print(",");
  sdcard_file.print(t);
   sdcard_file.print(",");
  sdcard_file.print(h);
   sdcard_file.print(",");
  sdcard_file.print(hic);
   sdcard_file.print(",");
  sdcard_file.print(rawX);
   sdcard_file.print(",");
  sdcard_file.print(rawY);
   sdcard_file.print(",");
  sdcard_file.print(rawZ);
  sdcard_file.print(",");
  sdcard_file.print(scaledX);
   sdcard_file.print(",");
  sdcard_file.print(scaledY);
  sdcard_file.print(",");
  sdcard_file.print(scaledZ);
  sdcard_file.print(",");
  sdcard_file.print(" ");
  sdcard_file.print(hPa);
  sdcard_file.print(",");
  sdcard_file.println(" ");
  
 
 
  
delay(223);

 
 if(digitalRead(save) == LOW){ 
  sdcard_file.close();

 }
}
float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

}
