#include <Wire.h>
#include <Adafruit_PN532.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <FastLED.h>
#include "template_config.h"
#include <Arduino.h >

// --- PIN DEFINITIONS ---
// I2C Pins (User Specified)
#define PN532_SDA_PIN   15
#define PN532_SCL_PIN   2

// LED Pin (Moved to 6 to avoid conflict with SDA on 8)
#define LED_PIN         4

// PN532 Control Pins (Not used in basic I2C mode, set to dummy)
#define PN532_IRQ       -1 
#define PN532_RESET     -1 

// Create the NFC instance
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

#define NUM_LEDS 50
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);

  // --- 1. Setup LEDs (Updated for WS2812B) ---
  // GRB is the standard color order for WS2812Bxxxxx 
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100); // Optional: Set brightness to save power/eyes

  // --- 2. Setup I2C ---
  Wire.begin(PN532_SDA_PIN, PN532_SCL_PIN);
  
  // --- 3. Setup NFC ---
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1) {
      singleBlink(CRGB::Red); 
      delay(500);
    }
  }
  
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  
  // Configure board to read RFID tags
  nfc.SAMConfig();

  // --- 4. Setup WiFi ---
  initWiFi();
}

void loop() {
  checkWiFi();
  readNFC();
}

void checkWiFi(){
   if (WiFi.status() == WL_CONNECTED) return;

   WiFi.reconnect();
   while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    singleBlink(CRGB::Yellow);
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  singleBlink(CRGB::Green); 
}

void singleBlink(const CRGB::HTMLColorCode colour) 
{
    for(int j = NUM_LEDS - 1; j >= 0 ; j--) 
    {
      leds[j] = colour;
      FastLED.show();
      delay(160);
    }
    
    delay(600);
    for(int j = 0; j < NUM_LEDS; j++) 
    {
      leds[j] = CRGB::Black;
    }
    FastLED.show();
}

void blink(const CRGB::HTMLColorCode colour) 
{
  for(int i =0; i < 3; i++) 
  {
    for(int j = 0; j < NUM_LEDS; j++) 
    {
      leds[j] = colour;      
    }
    FastLED.show();

    delay(200);
    for(int j = 0; j < NUM_LEDS; j++) 
    {
      leds[j] = CRGB::Black;
    }
    FastLED.show();

    delay(200);
  }
}

void readNFC() {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; 
  uint8_t uidLength;
  
  // Timeout set to 50ms to keep the loop non-blocking
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 50);

  if (success) {
    Serial.println("Found an NFC card!");

    String tagId = "";
    for (uint8_t i = 0; i < uidLength; i++) {
      if (uid[i] < 0x10) {
        tagId += "0";
      }
      tagId += String(uid[i], HEX);
    }
    tagId.toUpperCase();
    
    Serial.print("UID Value: ");
    Serial.println(tagId);

    bool status = postIDToServer(tagId);
    
    if (status) {
      blink(CRGB::Green);
    } else {
      blink(CRGB::Red);
    }
    
    delay(1000); // Prevent duplicate reads
  }
  else {
    delay(50); 
  }
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); 
  Serial.print("Connecting to WiFi ..");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
    singleBlink(CRGB::Yellow);
  }
  Serial.println(WiFi.localIP());
  singleBlink(CRGB::Green);
}

int postIDToServer(String id) {
  // Check if WiFi is actually connected before starting
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Lost! Reconnecting...");
    return -1;
  }

  Serial.println("Attempting to POST ID To Server");

  WiFiClient client;
  HTTPClient http;

  String url = "http://192.168.1.113:80/api/access/print-window/update?uuid=" + id;
  Serial.println(url);

  // 1. Begin with the specific client
  http.begin(client,url);
  // if (!http.begin(client, url)) {
  //   Serial.println("Unable to connect to server");
  //   return -1;
  // }

  // 2. FORCE the connection to close after the request
  // This prevents the "zombie socket" issue
  http.addHeader("Connection", "close");

  // 3. Set Authentication
  http.setAuthorization(USERNAME, PASSWORD);

  // 4. Send POST request
  int httpResponseCode = http.POST("");

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  // 5. Clean up aggressively
  http.end();
  client.stop(); 
  
  // 6. Brief pause to let the network stack recover
  delay(100);

  return httpResponseCode == 200;
}