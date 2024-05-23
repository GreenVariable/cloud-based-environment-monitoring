#include <ESP8266WiFi.h> //Include ESP8266
#include <ESP8266Firebase.h> //Include ESP8266Firebase for Firebase Realtime Database Connection.

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h> // This assumes you are using the ILI9341-based TFT display

//Adding Custom Fonts for TFT Display
#include <Fonts/FreeSansBold12pt7b.h> 
#include <Fonts/FreeSans9pt7b.h>



// Define the pin connections for the TFT display
#define TFT_CS D8      // Chip select pin
#define TFT_RST D7      // Reset pin
#define TFT_DC D6      // Data/Command pin
#define TFT_MOSI D5    // SPI MOSI (Master Out Slave In) pin
#define TFT_SCK D4     // SPI SCK (Clock) pin
#define TFT_MISO D3    // SPI MISO (Master In Slave Out) pin


// Create an instance of the Adafruit_ILI9341 class with specified pin connections
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);

// Wi-Fi credentials
const char* ssid = "Your-WiFi-SSID"; //Replace with your WiFi SSID
const char* password = "Your-WiFi-Passwor"; //Replace with your WiFi password

//Delay before retrieving data again
int dt = 1000;


 // Firebase project reference URL
#define REFERENCE_URL "Your-Firebase-Database-URL"  //Replace with your Firebase Database URL
// Create a Firebase object with the specified reference URL
Firebase firebase(REFERENCE_URL);

//RGB value for the background color of the TFT screen  
uint16_t rgbColor = tft.color565(155, 0, 0);
//RGB values for temperature, humidity, and light text
uint16_t rgbColor2 = tft.color565(192, 150, 73);
uint16_t rgbColor3 = tft.color565(0, 0, 0);



void setup() {
  Serial.begin(9600);
   // Initialize the TFT display
  tft.begin();
  // Set display rotation (1 for 90 degrees clockwise)
  tft.setRotation(1);
    // Set text size
  tft.setTextSize(1);
  //set font
  tft.setFont(&FreeSansBold12pt7b);
  //set background color
  tft.fillScreen(rgbColor);



  //Display texts to TFT
  tft.setTextColor(rgbColor3);
  typewriter(30, 50, "Temp.");
  typewriter(30, 100, "Hum.");
  typewriter(30, 150, "Li.");


  //change TFT font
  tft.setFont(&FreeSans9pt7b);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    //Displaying 'Connecting to WiFi' on TFT
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(30, 200);
    tft.print("C o n n e c t i n g   t o   W i F i . . . ");
    Serial.println("Connecting to WiFi...");
  }
  //Connected to WiFi
  // Clear the previous value i.e. clear the text 'Connecting to WiFi'
  tft.fillRect(30, 180, 300, 30, rgbColor);
  tft.setCursor(30, 200);
  //Displaying 'Connected to WiFi' on TFT
  tft.print("C o n n e c t e d   t o   W i F i "); //updating WiFi Status on TFT
  Serial.println("Connected to WiFi");
  //set back regular font
  tft.setFont(&FreeSansBold12pt7b);
  
}

void loop() {
  // Check if Wi-Fi is connected
  if (WiFi.status() == WL_CONNECTED) {
    //WiFi is connected
    // Fetch temperature from Firebase
    float tempC = firebase.getFloat("temperature");
    Serial.println(tempC);
    //function to clear previous temperature value from TFT and type the new value
    updateTemp(tempC);
    
    // Fetch humidity from Firebase
    float humidity = firebase.getFloat("humidity");
    Serial.println(humidity);
    //function to clear previous humidity value from TFT and type the new value
    updateHumidity(humidity);
    
    // Fetch light from Firebase
    float light = firebase.getFloat("light");
    Serial.println(light);
    //function to clear previous light value from TFT and type the new value
    updateLight(light);

    //Delay before fetching again
    delay(dt);

  } else {
    //Wi-Fi is not connected, attempt to reconnect
    //Change TFT Font for WiFi Status display
    tft.setFont(&FreeSans9pt7b);
    // Clear the previous value i.e. clear the text 'Connected to WiFi'.
    tft.fillRect(30, 180, 300, 30, rgbColor);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(30, 200);
    tft.print("R e c o n n e c t i n g . . .");
    Serial.println("WiFi not connected. Reconnecting...");
    WiFi.reconnect();
    delay(5000); // 5 seconds delay
    if (WiFi.status() == WL_CONNECTED) {
      // Clear the previous value i.e. clear the text 'Reconnecting'
     tft.fillRect(30, 180, 300, 30, rgbColor);
     tft.setCursor(30, 200);
     tft.print("C o n n e c t e d   t o   W i F i "); //updating WiFi Status on TFT
     //set back regular font
     tft.setFont(&FreeSansBold12pt7b);
    }
  }
}

void updateTemp(float tempC) {
  // Clear the previous values
  tft.fillRect(150, 30, 240, 30, rgbColor);
  
  delay(1000); 

  // Convert to string
  String tempStr = String(tempC, 2) + " C";
  
  
  tft.setTextColor(rgbColor2);
  typewriter(150, 50, tempStr);
}


void updateHumidity(float humidity) {
  // Clear the previous values
  tft.fillRect(150, 80, 240, 30, rgbColor);
  
  delay(1000); 

  // Convert to string
  String humidityStr = String(humidity, 2) + " %";
  
  tft.setTextColor(rgbColor2);
  typewriter(150, 100, humidityStr);

}


void updateLight(float light) {
  // Clear the previous values
  tft.fillRect(150, 130, 240, 30, rgbColor);
  
  delay(1000); 

  //convert light to percentage
  light = (light / 1023.0) * 100.0;
  // Convert to string
  String lightStr = String(light, 2) + " %";

  tft.setTextColor(rgbColor2);
  typewriter(150, 150, lightStr);
}



void typewriter(int x, int y, String text) {
  tft.setCursor(x, y);
  for (int i = 0; i < text.length(); i++) {
    tft.print(text[i]);
    delay(50);
  }
}
