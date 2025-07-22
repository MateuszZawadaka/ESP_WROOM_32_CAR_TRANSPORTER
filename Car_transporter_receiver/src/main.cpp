//-----------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------//
//------------------------Code for car transporter receiver--------------------------------//
//-----------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------//

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include<math.h>

int relaysPin[] = {32, 33, 25, 26, 27, 14, 12, 17, 5, 18, 19, 21, 22, 23};
int wpsPin = 13;
long result;

typedef struct struct_message {
  bool relaysActions[16];
} struct_message;
 
struct_message myData;
 

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Data received: ");
  Serial.println(len);
  
  for (int i = 0; i < 14; i++)
  {
    digitalWrite(relaysPin[i], !myData.relaysActions[i]);
  }
  
}
 
void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 14; i++)
  {
    pinMode(relaysPin[i], OUTPUT);
  }
  pinMode(wpsPin, INPUT_PULLUP);
  
  WiFi.mode(WIFI_STA);
  const uint8_t defaultMacAddress[] = {0x30, 0xc9, 0x22, 0x27, 0xe8, 0x29};
  esp_wifi_stop();
  esp_wifi_set_mac(WIFI_IF_STA, defaultMacAddress);
  Serial.print("Current MAC: ");
  Serial.println(WiFi.macAddress());
  esp_wifi_start();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");    
    return;
  }
  
  
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  if (WiFi.status() != WL_CONNECTION_LOST)
  {
      for (int i = 0; i < 14; i++)
      {
        digitalWrite(relaysPin[i], false);
        delay(100);
      }

  }
  
}



