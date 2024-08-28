// ====== INCLUDES =======
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ESP8266WiFi.h>

// ====== DEFINES =======
#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// ====== OBJECTS =======
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===== VARIABLES ======
const char *ssid_ap = "Smart-Cube-Tester";
//const char* password_ap = "secret";

// ======= SETUP ========
void setup()   {

  Serial.begin(9600);
  
  // set esp8266 as access point
  WiFi.softAP(ssid_ap);
  Serial.println("Smart-Cube-Tester ssid: " + String(ssid_ap));
  Serial.print("Smart-Cube-Tester IP address: ");
  Serial.println(WiFi.softAPIP());

  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true);
  delay(250);
  display.clearDisplay();

  
}

// ======== LOOP ========
void loop() {
  mockOutputOLED();
  delay(2000);
}

// ====== FUNCTIONS ======

void mockOutputOLED(){

  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 10);
  display.println("Command:");
  display.setCursor(10, 30);
  display.println("undefined");
  display.display();
}