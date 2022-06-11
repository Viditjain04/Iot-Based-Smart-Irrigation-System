#define BLYNK_PRINT Serial // Serial to define and print 

#include <OneWire.h> //for serial communication via the Dallas protocol 

#include <SPI.h> // allows you to communicate with serial peripheral interface devices(io device like soil moisture sensor, relay..)
#include <BlynkSimpleEsp8266.h> // for node mcu
// support wifi, ble, bluetooth, ethernet with many boards  

#include <DHT.h> // arduino library for dht1, dht2 etc temp and humidity sensors
#include <DallasTemperature.h>
// DallasTemperature.h library is provides by Dallas. It is used for dallas temperature sensors. 
//included this library in our code to use its pre defined functions provided by dallas to work with its temperature sensors

// Data wire is connect to the Arduino digital pin 2
#define ONE_WIRE_BUS D2 
OneWire oneWire(ONE_WIRE_BUS);

//Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire); 

char auth[] =BLYNK_AUTH_TOKEN; // used with wifi module
char ssid[] = "username"; 
char pass[] = "password";
 
#define DHTPIN 2 
#define DHTTYPE DHT11
//DHTPIN: the Arduino pin the DHT sensor is connected to
//DHTTYPE: DHT type 
DHT dht(DHTPIN, DHTTYPE); //To create an  object

SimpleTimer timer; //Arduino library for work with time. 
// function to sense the temp and humidity
void sendSensor()
{
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
    }

    Blynk.virtualWrite(V5, h); //V5 is for Humidity
    Blynk.virtualWrite(V6, t); //V6 is for Temperature
}
// connection setup
void setup()
{
    Serial.begin(9600); // a command you give to the Arduino to begin serial communication for debugging purposes

    dht.begin(); //Start and initialize the dht object
  
    timer.setInterval(1000L, sendSensor); 
    Blynk.begin(auth, ssid, pass); 
      // Start up the library
    sensors.begin(); //?
}
int sensor=0;
int output=0;
void sendTemps()
{
    sensor=analogRead(A0);
    output=(145-map(sensor, 0, 1023, 0, 100)); // concurrent values, then map from 0-100,then decide threshold value, then check whether the moisture level is than the threshold value or not
    delay(1000);
     // Call sensors.requestTemperatures()-Requests to all devices  
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0); // if we use multiple temp sensor
    Serial.println(temp);
    Serial.print("moisture =" );
    Serial.print(output); // serial.println(sensorValue) is a command that the program gives to the Arduino to print the variable sensorValue to the serial monitor.
    Serial.println("%");
    Blynk.virtualWrite(V1, temp);
    Blynk.virtualWrite(V2,output);
    delay(1000); //temperature readings are requested every second.
}
void loop()
{
    Blynk.run();
    timer.run();
    sendTemps();
}
