// ====== INCLUDES =======
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

#include <ArduinoJson.h>

// ====== DEFINES =======
#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// ====== OBJECTS =======
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ESP8266WebServer server(80);

// ===== VARIABLES ======
const char *ssid_ap = "Smart-Cube-Tester";
//const char* password_ap = "secret";
String last_command = "";

// ====== FUNCTIONS ======

void displayOLED(){
  //display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 10);
  display.println("Command:");
  display.setCursor(10, 30);
  if(last_command != ""){
    display.println(last_command);
  } else {
    display.println("n/a");
  }
  display.display();
}

void handleHomePage() {
  server.send(200, "text/plain", "Smart Cube Tester");
}

void handleOnNotFound(){
  server.send(404, "text/plain", "Not found");
}

void handleCommand() {
  if (server.hasArg("plain")) {
    String input_json = server.arg("plain");
    Serial.println("Received JSON: " + input_json);

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, input_json);

    if (error) {
      Serial.println("Failed to parse JSON");
      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
      return;
    }

    String command = doc["command"];
    Serial.println("Command: " + command);

    last_command = command;
    displayOLED();

    server.send(200, "application/json", "{\"status\":\"success\",\"message\":\"Command received\"}");
  } else {
    server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"No JSON received\"}");
  }
}

// ======= SETUP ========
void setup()   {

  Serial.begin(9600);
  
  // Set esp8266 as access point
  WiFi.softAP(ssid_ap);
  Serial.println("Smart-Cube-Tester ssid: " + String(ssid_ap));
  Serial.print("Smart-Cube-Tester IP address: ");
  Serial.println(WiFi.softAPIP());

  delay(250); // Wait for the OLED to power up
  display.begin(i2c_Address, true);
  delay(250);
  display.clearDisplay();

  server.on("/", handleHomePage);
  server.on("/command", HTTP_POST, handleCommand); 
  server.onNotFound(handleOnNotFound);
  server.begin();
  
}

// ======== LOOP ========
void loop() {
  server.handleClient();

  //displayOLED();
  delay(1000);
  
}

