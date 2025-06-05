#include <WiFi.h>
#include <WebServer.h>
#include <FastLED.h>
#include "home.h"

// WiFi Access Point config
const char *ssid = "LED_Controller";
const char* password = "12345678";
IPAddress local_ip(192, 98, 24, 1);
IPAddress gateway(192, 98, 24, 1);
IPAddress subnet(255, 255, 255, 0);
WebServer server(80);
unsigned long lastToggle = 0;
bool cautionState = false;

// LED config
#define LED_PIN_STRIP1 5
#define LED_PIN_STRIP2 18
#define NUM_LEDS 255
CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];

enum LEDMode { OFF, ON, AMBER, RED, CAUTION };
LEDMode strip1Mode = OFF;
LEDMode strip2Mode = OFF;


void setupWiFi() {
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
}

void handleRoot() {
  server.send(200, "text/html", HOME_HTML);
}

void applyLEDMode(CRGB *leds, LEDMode mode) {
  switch (mode) {
    case ON:
      fill_solid(leds, NUM_LEDS, CRGB::White);
      break;
    case AMBER:
      fill_solid(leds, NUM_LEDS, CRGB(255, 100, 0));
      break;
    case RED:
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      break;
    case OFF:
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;
    case CAUTION:
      // Caution handled in loop (flashing state)
      break;
  }
  FastLED.show();
}

void defineRoutes() {
  server.on("/", handleRoot);

  server.on("/strip1/on", []() { strip1Mode = ON; applyLEDMode(leds1, ON); server.sendHeader("Location", "/"); server.send(303); });
  server.on("/strip1/off", []() { strip1Mode = OFF; applyLEDMode(leds1, OFF); server.sendHeader("Location", "/"); server.send(303); });
  server.on("/strip1/amber", []() { strip1Mode = AMBER; applyLEDMode(leds1, AMBER); server.sendHeader("Location", "/"); server.send(303); });
  server.on("/strip1/red", []() { strip1Mode = RED; applyLEDMode(leds1, RED); server.sendHeader("Location", "/"); server.send(303); });
  server.on("/strip1/caution", []() { strip1Mode = CAUTION; server.sendHeader("Location", "/"); server.send(303); });

  server.on("/strip2/on", []() { strip2Mode = ON; applyLEDMode(leds2, ON); server.sendHeader("Location", "/"); server.send(303); });
  server.on("/strip2/off", []() { strip2Mode = OFF; applyLEDMode(leds2, OFF); server.sendHeader("Location", "/"); server.send(303); });
  server.on("/strip2/amber", []() { strip2Mode = AMBER; applyLEDMode(leds2, AMBER); server.sendHeader("Location", "/"); server.send(303); });
  server.on("/strip2/red", []() { strip2Mode = RED; applyLEDMode(leds2, RED); server.sendHeader("Location", "/"); server.send(303); });
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  defineRoutes();
  server.begin();

  FastLED.addLeds<WS2811, LED_PIN_STRIP1, GRB>(leds1, NUM_LEDS);
  FastLED.addLeds<WS2811, LED_PIN_STRIP2, GRB>(leds2, NUM_LEDS);

  FastLED.clear(true);

  // set leds2 to white on power on
  fill_solid(leds2, NUM_LEDS, CRGB::White);
  FastLED.show();
}

void loop() {
  server.handleClient();

  // Flash amber at 2Hz if in CAUTION mode
  if (strip1Mode == CAUTION) {
    unsigned long now = millis();
    if (now - lastToggle >= 500) {
      cautionState = !cautionState;
      fill_solid(leds1, NUM_LEDS, cautionState ? CRGB(255, 100, 0) : CRGB::Black);
      FastLED.show();
      lastToggle = now;
    }
  }
}
