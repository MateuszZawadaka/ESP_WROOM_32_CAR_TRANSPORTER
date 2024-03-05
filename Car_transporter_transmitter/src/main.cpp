#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>


uint8_t broadcastAddress[] = {0x30, 0xC9, 0x22, 0x27, 0xE8, 0x28}; // PUt your receiver MAC Address examply: Receiver MAC address 30:C9:22:27:E8:28 then broadcastAddress[] = {0x30, 0xC9, 0x22, 0x27, 0xE8, 0x28}
String succes;

int switches[14] = {32,33,25,26,27,14,12,23,22,21,19,18,5,17};
bool switchesActions[14] ={false,false,false,false,false,false,false,false,false,false,false,false,false,false};
int statusLed = 16;


typedef struct struct_message
{
  bool actions[14] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false};

} struct_message;
struct_message buttonActions;

esp_now_peer_info_t peerInfo;


void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if(status == 0){
    succes = "Delivery Success";
  }
  else{
    succes = "Delivery Fail";
  }
}

// put function declarations here:
void switchesMapping(int);
void switchesActionsMapping(int);

void setup() {
  Serial.begin(9600);
  pinMode(statusLed, OUTPUT);
  for (int i = 0; i < 14; i++)
  {
    switchesMapping(switches[i]);
    Serial.print(switches[i]);
    delay(100);
  }
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(onDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if(esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to peer");
    digitalWrite(statusLed, LOW);
    return;
  }
  else{
      digitalWrite(statusLed, HIGH);
  }
  
}

void loop() {
  for (int i = 0; i < 14; i++)
  {
    switchesActionsMapping(switches[i]);
    buttonActions.actions[i] = switchesActions[i];
    Serial.println(buttonActions.actions[i]);
    delay(10);
    
    
  }
  //esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &buttonActions, sizeof(buttonActions));
 }



void switchesMapping(int iPin){
  pinMode(iPin, INPUT_PULLUP);
}
void switchesActionsMapping(int iPin){
  if(digitalRead(iPin) == LOW){
    switchesActions[iPin] = true;
    Serial.print(iPin);
    Serial.println("Pressed");
  }
  else{
    switchesActions[iPin] = false;
  }
}