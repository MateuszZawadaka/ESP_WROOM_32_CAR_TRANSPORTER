//-------------------------------------------------------------------------//
//------- Code for car transporter transmitter ----------------------------//
//-------------------------------------------------------------------------// 
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <pinsModeSetup.h>
// // put function declarations here:
// int myFunction(int, int);
// // void setInputMode(int);
// void testOutputsHigh(int);
// void testOutputsLow(int);
// void readMacAddress();
// void OnDataSent(const uint8_t , esp_now_send_status_t);
// const int switchesPin[] = {32, 33, 25, 26, 27, 14, 12, 23, 22, 21, 19, 18, 5, 17};
// const int statusLed = 16;
// uint8_t receiverMacAddress[] = {0x30, 0xc9, 0x22, 0x27, 0xe8, 0x28};
// typedef struct TransmitterStruct
// {
//   bool switchesAction[14];

// }TransmitterStruct;
// TransmitterStruct sentData;
// pinsModeSetup inputs;
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
// {
//   Serial.print("\r\nLast Packet Send Status:\t");
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
// }
// void setup() {
//   inputs.setInputMode(14, switchesPin);
//   Serial.begin(9600);
//   WiFi.mode(WIFI_STA);
//   WiFi.begin();
//   Serial.print("[DEFAULT] ESP32 Board MAC Address: ");
//   readMacAddress();
//   int result = myFunction(2, 3);

//   if(esp_now_init() != ESP_OK){
//     Serial.println("Error initializing ESP-NOW");
//     return;

//   }
//   esp_now_register_send_cb(OnDataSent);

//   esp_now_peer_info_t peerInfo;
//   memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
//   peerInfo.channel = 0;  
//   peerInfo.encrypt = false;
//   if(esp_now_add_peer(&peerInfo) != ESP_OK)
//   {
//     Serial.println("Failed to add peer");
//     return;
//   }
  
// }

// void loop() {
//   for (int i = 0; i < 14; i++)
//   {
//     delayMicroseconds(30);
//     sentData.switchesAction[i] = digitalRead(switchesPin[i]);
    
//   }
  
//   //-------------------------------------------------------------------------------------
//   esp_err_t result = esp_now_send(receiverMacAddress, (uint8_t *) &sentData, sizeof(sentData));
//   //-------------------------------------------------------------------------------------
//   if (result == ESP_OK) Serial.println("Sent with success");
//   else Serial.println("Error sending the data");
//   //-------------------------------------------------------------------------------------
//   delay(100);
//   // put your main code here, to run repeatedly:
// }

// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }
// // void setInputMode(int pin){
// //   pinMode(pin, INPUT_PULLUP);

// // }

// // void testOutputsHigh(int output){
// //   digitalWrite(output, HIGH);
// //   Serial.println(digitalRead(output));
// //   delayMicroseconds(100);
// // }

// // void testOutputsLow(int output){
// //   digitalWrite(output, LOW);
// //   Serial.println(digitalRead(output));
// //   delayMicroseconds(100);
// // }

// void readMacAddress(){
//   uint8_t baseMac[6];
//   esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);

//   if (ret == ESP_OK)
//   {
//     Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
//                   baseMac[0], baseMac[1], baseMac[2], baseMac[3],
//                   baseMac[4], baseMac[5]);
//   } else{
//     Serial.println("Failed to read MAC address");
//   }
  
// 
//------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------//


// Variables for test data
int int_value;
float float_value;
bool bool_value = true;

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
  delay(2000);
}