#include "DHT.h"       // Include the DHT library for interfacing with the sensor
#include <ESP8266WiFi.h> //Include ESP8266
#include <ESP8266Firebase.h>

// Wi-Fi credentials
const char* ssid = "Your-WiFi-SSID"; //Replace with your WiFi SSID
const char* password = "Your-WiFi-Passwor"; //Replace with your WiFi password


//Red LED: turns on when not connected to internet
int RedLED = D1;

 // Firebase project reference URL
#define REFERENCE_URL "Your-Firebase-Database-URL"  //Replace with your Firebase Database URL
// Create a Firebase object with the specified reference URL
Firebase firebase(REFERENCE_URL);

#define Type DHT11     // Define the sensor type as DHT11
int sensePin = D2;      // Define the sensor's data pin as digital pin 2
DHT HT(sensePin, Type); // Create a DHT object named HT for the sensor
// Variables to store humidity and temperature readings
float humidity;        
float tempC;
float tempF;
int setTime = 500;     // Initial delay time in milliseconds
int dt = 1000;         // Delay between sensor readings in milliseconds

//Pin A0 of the NodeMCU reads the voltage which depends on the light.
//Dimmer light to photoresistor will result in lessor voltage (high resistance) and vice versa 
int ReadPin = A0;
//Variable to store the voltage reading by analog pin
int Volts = 0;

void setup() {
  // Setup code that runs once when the Arduino starts
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
  HT.begin();          // Initialize the DHT sensor
  delay(setTime);      // Initial delay to let the sensor stabilize
  //Analog Pin (defined as ReadPin in our case) is the the input pin 
  pinMode(ReadPin, INPUT);

  //Red LED pin as OUTPUT pin
  pinMode(RedLED, OUTPUT);
  //turn on red LED until connected to internet
  digitalWrite(RedLED, HIGH);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  //turn off Red LED
   digitalWrite(RedLED, LOW);
}

void loop() {
  // Main loop that runs repeatedly
  humidity = HT.readHumidity();    // Read the humidity from the sensor
  tempC = HT.readTemperature();    // Read temperature in Celsius
  // Getting voltage reading using analogRead method.
  Volts = analogRead(ReadPin);

    // Check if Wi-Fi is connected
  if (WiFi.status() == WL_CONNECTED) {
    // Upload data to firebae
    firebase.setFloat("humidity", humidity);
    Serial.println("Humidity data uploaded");
    firebase.setFloat("temperature", tempC);
    Serial.println("Temperature data uploaded");
    firebase.setFloat("light", Volts);
    Serial.println("Light data uploaded");
    delay(dt);
    //keep the Red LED off when connected to internet  
   digitalWrite(RedLED, LOW);
  
  } else {
    // If Wi-Fi is not connected, attempt to reconnect
    //turn on Red LED until connected back to internet
    digitalWrite(RedLED, HIGH);
    Serial.println("WiFi not connected. Reconnecting...");
    WiFi.reconnect();
    delay(5000); // 5 seconds delay
  }
}