//-----------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------//
//------------------------Code for car transporter receiver--------------------------------//
//-----------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------//

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>


int relaysPin[] = {32, 33, 25, 26, 27, 14, 12, 17, 5, 18, 19, 21, 22, 23};

// Define a data structure
typedef struct struct_message {
  bool relaysActions[16];
} struct_message;
 
// Create a structured object
struct_message myData;
 
 
// Callback function executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Data received: ");
  for (int i = 0; i < 14; i++)
  {
    digitalWrite(relaysPin[i], !myData.relaysActions[i]);
  }
  
}
 
void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
  for (int i = 0; i < 14; i++)
  {
    pinMode(relaysPin[i], OUTPUT);
  }
  
  
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
 
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
 
}