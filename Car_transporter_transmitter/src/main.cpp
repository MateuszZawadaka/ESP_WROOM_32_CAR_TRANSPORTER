
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

// void readMacAddress();

int int_value;
float float_value;
bool bool_value = true;
bool bMacAddressReaded = false;
uint8_t broadcastAddress[] = {0x30, 0xc9, 0x22, 0x27, 0xe8, 0x28};

typedef struct struct_message {
  bool relaysActions[16];
} struct_message;

struct_message myData;
int statusLed = 16;
esp_now_peer_info_t peerInfo;
const int switchesPin[] = {32, 33, 25, 26, 27, 14, 12, 23, 22, 21, 19, 18, 5, 17};
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  
  Serial.begin(115200);
  for (int i = 0; i < 16; i++)
  {
    pinMode(switchesPin[i], INPUT_PULLUP);
  }
  pinMode(statusLed, OUTPUT);
  if (bMacAddressReaded != true)
  {
    if (esp_base_mac_addr_set(broadcastAddress) == ESP_OK)
    {
      bMacAddressReaded = true;
    }
    else
    {
      bMacAddressReaded = false;
      return;
    }
  }
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  
  esp_now_register_send_cb(OnDataSent);
  
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
   
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    digitalWrite(statusLed, HIGH);
    return;
  }    
}
  



void loop() {

  for (int i = 0; i < 16; i++)
  {
    myData.relaysActions[i] = digitalRead(switchesPin[i]);
  }
  
  
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    digitalWrite(statusLed, LOW);
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
    

  }
  delay(100);
}

void readMacAddress(){
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, broadcastAddress);
  if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  broadcastAddress[0], broadcastAddress[1], broadcastAddress[2],
                  broadcastAddress[3], broadcastAddress[4], broadcastAddress[5]);
  bMacAddressReaded = true;
  } else {
    Serial.println("Failed to read MAC address");
    bMacAddressReaded = false;
  }
}
