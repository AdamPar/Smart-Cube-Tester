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

#define LED_PIN_1 14
#define LED_PIN_2 12
#define LED_PIN_3 13
#define LED_PIN_4 16


// ====== OBJECTS =======
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===== VARIABLES ======
const int ledPins[] = {LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4};
const int numLEDs = sizeof(ledPins) / sizeof(ledPins[0]);

int currentLED = 0;
bool flickering_flag = false;

unsigned long lastActionTime = 0;
const unsigned long actionInterval = 300;

bool actionActive = false;
String last_command = "";

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

void commandHandler(){
  // ----- TURNING -----
  if (command_recieved == "TURN_FORWARD") {
    switchOnAllLEDs();
    actionActive = false;
  } else if (command_recieved == "TURN_BACKWARD") {
    switchOffAllLEDs();
    actionActive = false;
  } else if (command_recieved == "TURN_RIGHT") {
    actionActive = true;
    forwardLED();
  } else if (command_recieved == "TURN_LEFT") {
    actionActive = true;
    backwardLED();
  } // ----- ROTATIONS -----
   else if (command_recieved == "ROT_RIGHT") {
    // lightOneLED();
    actionActive = false;
  } else if (command_recieved == "ROT_LEFT") {
    // lightOneLED();
    actionActive = false;
  } // ----- SLIDINGS -----
   else if (command_recieved == "SLIDE_FORWARD") {
    switchOffAllLEDs();
    digitalWrite(LED_PIN_1, HIGH);
    actionActive = false;
  } else if (command_recieved == "SLIDE_BACKWARD") {
    switchOffAllLEDs();
    digitalWrite(LED_PIN_4, HIGH);
    actionActive = false;
  } else if (command_recieved == "SLIDE_RIGHT") {
    switchOffAllLEDs();
    digitalWrite(LED_PIN_2, HIGH);
    actionActive = false;
  } else if (command_recieved == "SLIDE_LEFT") {
    switchOffAllLEDs();
    digitalWrite(LED_PIN_3, HIGH);
    actionActive = false;
  } // ----- SHAKING -----
   else if (command_recieved == "SHAKING") {
    actionActive = true;
    flickeringLEDs();
  } else {
    Serial.println("Unknown command: " + command_recieved);
  }
}

void onCommandChange() {
  
  if(command_recieved != last_command){
    Serial.println("Command from cloud: " + command_recieved);
    last_command = command_recieved;
    displayOLED();

    Serial.println("triggering action");
    commandHandler();
  }
}

void switchOnAllLEDs() {
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
}

void switchOffAllLEDs() {
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void forwardLED() {
  unsigned long currentTime = millis();
  if (currentTime - lastActionTime >= actionInterval) {
    lastActionTime = currentTime;
    switchOffAllLEDs();
    digitalWrite(ledPins[currentLED], HIGH);
    currentLED = (currentLED + 1) % numLEDs; // Move to the next LED
  }
}

void backwardLED() {
  unsigned long currentTime = millis();
  if (currentTime - lastActionTime >= actionInterval) {
    lastActionTime = currentTime;
    switchOffAllLEDs();
    currentLED = (currentLED - 1 + numLEDs) % numLEDs; // Move to the previous LED
    digitalWrite(ledPins[currentLED], HIGH);
  }
}

void flickeringLEDs(){
  unsigned long currentTime = millis();
  if (currentTime - lastActionTime >= actionInterval) {
    lastActionTime = currentTime;
    if (flickering_flag) {
      switchOffAllLEDs();
    } else {
      switchOnAllLEDs();
    }
    flickering_flag = !flickering_flag;
  }
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

  // pin setup
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

}

// ======== LOOP ========
void loop() {
  ArduinoCloud.update();
  onCommandChange();

   if (actionActive) {
    if (command_recieved == "TURN_RIGHT") {
      forwardLED();
    } else if (command_recieved == "TURN_LEFT") {
      backwardLED();
    } else if (command_recieved == "SHAKING") {
      flickeringLEDs();
    }
  }

  delay(100);
}
