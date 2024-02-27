#include <Arduino.h>
#include <WiFi.h>
int iStatusLed = 16;
int aiSwitches[14] = {32,33,25,26,27,14,12,23,22,21,19,18,5,17};
const char* ssid = "PLAY_Swiatlowodowy_A098";
const char* password = "AkX4NXYv#5zX";
const char* serverAddress = "192.168.0.180";
const int serverPort = 4080;

WiFiClient TCPclient;
// put function declarations here:
int myFunction(int, int);
void switchesPinout(int);
void switchAction(int);
void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  Serial.begin(9600);
  pinMode(iStatusLed, OUTPUT);
  for (int i = 0; i < 14; i++)
  {
    switchesPinout(aiSwitches[i]);
  }
  Serial.begin(9600);
   // button.setDebounceTime(50); // set debounce time to 50 milliseconds

    Serial.println("ESP32: TCP CLIENT + A BUTTON/SWITCH");

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
      digitalWrite(iStatusLed, LOW);
    }
    Serial.println("Connected to WiFi");
    digitalWrite(iStatusLed, HIGH);
    // connect to TCP server (Arduino #2)
    if (TCPclient.connect(serverAddress, serverPort)) {
      Serial.println("Connected to TCP server");
    } else {
      Serial.println("Failed to connect to TCP server");
    }

}

void loop() {
    for (int i = 0; i < 14; i++)
    {
      switchAction(aiSwitches[i]);
    }
    if(WiFi.status() != WL_CONNECTED){
      digitalWrite(iStatusLed, LOW);
    }
    else{
      digitalWrite(iStatusLed, HIGH);
    }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

void switchesPinout(int pinNumber){
  pinMode(pinNumber, INPUT_PULLUP);
  Serial.print(pinNumber);
}
void switchAction(int pinNumber){
  if(digitalRead(pinNumber) == LOW){
    Serial.print("Button");
    Serial.print(pinNumber);
    Serial.print("Pressed");
  }
}