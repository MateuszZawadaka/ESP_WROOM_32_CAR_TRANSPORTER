#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

int aiRelaysPin[16] ={34,35,32,33,25,26,27,14,12,17,5,18,19,21,22,23};
bool bRelaysActions[14] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false};
// put function declarations here:
void relaysMapping(int);

typedef struct struct_message
{
  bool actions[14] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false};

} struct_message;

struct_message receivedData;

esp_now_peer_info_t peerInfo;
void onDataReceived(const uint8_t * mac, const uint8_t *incomingData, int len){
  memcpy(&receivedData, incomingData, sizeof(incomingData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  for (int i = 0; i < 14; i++)
  {
    bRelaysActions[1] = receivedData.actions[1];
    Serial.print(bRelaysActions[i]);
  }
  
}


void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 16; i++)
  {
    relaysMapping(aiRelaysPin[i]);
  }

  WiFi.mode(WIFI_STA);

  if(esp_now_init() != ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
  }
   esp_now_register_recv_cb(onDataReceived);
  // for (int i = 0; i < 14; i++)
  // {
  //   bRelaysActions[i] = receivedData.actions[i];
  //   Serial.print(receivedData.actions[i]);
  //   delay(200);
    
  // }
}

void loop() {
 
  // for (int i = 0; i < 14; i++)
  // {
  //   bRelaysActions[i] = receivedData.actions[i];
  //   Serial.print(receivedData.actions[i]);
  //   delay(200);
    
  // }
  delay(20);
}

// put function definitions here:


void relaysMapping(int iPin){
  pinMode(iPin, OUTPUT);

}