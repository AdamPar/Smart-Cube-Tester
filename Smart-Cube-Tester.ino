// ====== INCLUDES =======
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ESP8266WiFi.h>

#include "thingProperties.h"

// ====== DEFINES =======
#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// ====== OBJECTS =======
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===== VARIABLES ======


// ====== FUNCTIONS ======

void displayOLED() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 10);
  display.println("Command:");
  display.setCursor(1, 30);
  if (command_recieved != "") {
    display.println(command_recieved);
  } else {
    display.println("n/a");
  }
  display.display();
}

void onCommandChange() {
  Serial.println("Updated command: " + command_recieved);
  displayOLED();
}


// ======= SETUP ========
void setup() {
  Serial.begin(9600);
  Serial.println("ESP8266 Smart Cube Display Starting...");

  // Initialize the OLED display
  delay(250); // Wait for the OLED to power up
  display.begin(i2c_Address, true);
  delay(250);
  display.clearDisplay();
  displayOLED();

// Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

// ======== LOOP ========
void loop() {
  ArduinoCloud.update();
  onCommandChange();          // Update the OLED display
  delay(100);
}
