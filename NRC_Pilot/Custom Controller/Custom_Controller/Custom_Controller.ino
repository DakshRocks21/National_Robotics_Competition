#include <esp_now.h>
#include <WiFi.h>

// Pin definitions for joysticks and buttons
const int leftJoystickXPin = 32;  // Analog pin for left joystick X-axis
const int leftJoystickYPin = 33;  // Analog pin for left joystick Y-axis
const int rightJoystickXPin = 34; // Analog pin for right joystick X-axis
const int rightJoystickYPin = 35; // Analog pin for right joystick Y-axis

const int buttonPins[8] = {5,12,13,14,15,25,26,27};  // Digital pins for buttons

// Structure to hold joystick and button data-  
typedef struct struct_message {
  int leftX;
  int leftY;
  int rightX;
  int rightY;
  bool buttons[8];
} struct_message;

struct_message myData;

// ROBOT MAC ADDRESS
uint8_t broadcastAddress[] = {0xec, 0x64, 0xc9, 0xa9, 0x06, 0x14};

void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(onSent);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Initialize buttons as input
  for (int i = 0; i < 8; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Initialize joysticks as input
  pinMode(leftJoystickXPin, INPUT);
  pinMode(leftJoystickYPin, INPUT);
  pinMode(rightJoystickXPin, INPUT);
  pinMode(rightJoystickYPin, INPUT);
}

void loop() {
  // Read joystick values
  myData.leftX = analogRead(leftJoystickXPin);
  myData.leftY = analogRead(leftJoystickYPin);
  myData.rightX = analogRead(rightJoystickXPin);
  myData.rightY = analogRead(rightJoystickYPin);

  // Map joystick values to -100 to 100 range for ease of use
  myData.leftX = map(myData.leftX, 0, 4095, -100, 100);
  myData.leftY = map(myData.leftY, 0, 4095, -100, 100);
  myData.rightX = map(myData.rightX, 0, 4095, -100, 100);
  myData.rightY = map(myData.rightY, 0, 4095, -100, 100);

  // Print joystick values
  Serial.print("Left Joystick X: ");
  Serial.println(myData.leftX);
  Serial.print("Left Joystick Y: ");
  Serial.println(myData.leftY);
  Serial.print("Right Joystick X: ");
  Serial.println(myData.rightX);
  Serial.print("Right Joystick Y: ");
  Serial.println(myData.rightY);

  // Read and print button states
  for (int i = 0; i < 8; i++) {
    myData.buttons[i] = digitalRead(buttonPins[i]) == LOW;  // LOW means pressed
    Serial.print("Button ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(myData.buttons[i] ? "Pressed" : "Released");
  }

  // Send data via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }

  delay(100);  // Adjust the delay as needed
}
