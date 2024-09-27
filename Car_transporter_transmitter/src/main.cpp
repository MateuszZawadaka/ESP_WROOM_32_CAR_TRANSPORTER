
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <pinsModeSetup.h>

// void readMacAddress();

int int_value;
float float_value;
bool bool_value = true;
bool bMacAddressReaded = false;
// MAC Address of responder - edit as required
uint8_t broadcastAddress[] = {0x30, 0xc9, 0x22, 0x27, 0xe8, 0x28};

// Define a data structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
  bool relaysActions[16];
} struct_message;

// Create a structured object
struct_message myData;

// Peer info
esp_now_peer_info_t peerInfo;
const int switchesPin[] = {32, 33, 25, 26, 27, 14, 12, 23, 22, 21, 19, 18, 5, 17};
// Callback function called when data is sent
void readMacAddressFromUser();
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  
  // Set up Serial Monitor
  Serial.begin(115200);
  for (int i = 0; i < 14; i++)
  {
    pinMode(switchesPin[i], INPUT_PULLUP);
  }
  if(bMacAddressReaded == false)
  {
    readMacAddressFromUser();
  }

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  
  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {

  // Create test data

  // Generate a random integer
  int_value = random(1,20);

  // Use integer to make a new float
  float_value = 1.3 * int_value;

  // Invert the boolean value
  bool_value = !bool_value;
  
  // Format structured data
  strcpy(myData.a, "Welcome to the Workshop!");
  myData.b = int_value;
  myData.c = float_value;
  myData.d = bool_value;
  for (int i = 0; i < 14; i++)
  {
    myData.relaysActions[i] = digitalRead(switchesPin[i]);
  }
  
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  delay(200);
}

// void readMacAddress(){
//   esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, broadcastAddress);
//   if (ret == ESP_OK) {
//     Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
//                   broadcastAddress[0], broadcastAddress[1], broadcastAddress[2],
//                   broadcastAddress[3], broadcastAddress[4], broadcastAddress[5]);
//   bMacAddressReaded = true;
//   } else {
//     Serial.println("Failed to read MAC address");
//     bMacAddressReaded = false;
//   }
// }
void readMacAddressFromUser(){

    Serial.println("Write first recieiver MAC char example: 0xc9");
    broadcastAddress[0] = Serial.parseInt();
    while (Serial.available() == 0)
    {
      /* code */
    }
    
    Serial.println("Write second recieiver MAC char example: 0xc9");
    broadcastAddress[1] = Serial.parseInt();
    while (Serial.available() == 0)
    {
      /* code */
    }
    Serial.println("Write third recieiver MAC char example: 0xc9");
    broadcastAddress[2] = Serial.parseInt();
    while (Serial.available() == 0)
    {
      /* code */
    }
    Serial.println("Write fourth recieiver MAC char example: 0xc9");
    broadcastAddress[3] = Serial.parseInt();
    while (Serial.available() == 0)
    {
      /* code */
    }
    Serial.println("Write fifth recieiver MAC char example: 0xc9");
    broadcastAddress[4] = Serial.parseInt();
    while (Serial.available() == 0)
    {
      /* code */
    }
    Serial.println("Write sixth recieiver MAC char example: 0xc9");
    broadcastAddress[5] = Serial.parseInt();
    while (Serial.available() == 0)
    {
      /* code */
    }
    
    Serial.print(broadcastAddress[0]);
    Serial.print(broadcastAddress[1]);
    Serial.print(broadcastAddress[2]);
    Serial.print(broadcastAddress[3]);
    Serial.print(broadcastAddress[4]);
    Serial.print(broadcastAddress[5]);

    bMacAddressReaded = true;
}