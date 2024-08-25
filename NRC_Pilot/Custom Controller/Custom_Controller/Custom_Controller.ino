#include <esp_now.h>
#include <WiFi.h>

const int leftJoystickXPin = 36;   // Analog pin for left joystick X-axis
const int leftJoystickYPin = 39;   // Analog pin for left joystick Y-axis
const int rightJoystickXPin = 34;  // Analog pin for right joystick X-axis
const int rightJoystickYPin = 35;  // Analog pin for right joystick Y-axis

const int buttonPins[8] = { 0, 2, 15, 13, 19, 23, 18, 22 };  // Digital pins for main buttons
const int solenoidPins[6] = { 4, 32, 33, 27, 14, 12 };       // Digital pins for solenoid buttons
const int masterPins[2] = { 25, 26 };                        // Digital pins for master buttons

const int dataPin = 5;    // SER (Serial Data Input)
const int latchPin = 17;  // RCLK (Register Clock / Latch Pin)
const int clockPin = 16;  // SRCLK (Shift Register Clock)

char connected = 0;

// Array to hold the LED states (16 LEDs, 2 bytes for 2 shift registers)
uint16_t ledState = 0x0000;  // Initially all LEDs off

// Structure to hold joystick, button, solenoid, and master button data
typedef struct struct_message {
  int leftX;
  int leftY;
  int rightX;
  int rightY;
  bool buttons[8];
  bool solenoids[6];
  bool master[2];
} struct_message;

struct_message myData;

// Structure to receive solenoid and motor status from the robot
typedef struct struct_response {
  char solenoidStatus[6];
  int motorSpeeds[16];
  float ultrasonicDistance[10];
} struct_response;

struct_response robotData;

// ROBOT MAC ADDRESS
uint8_t broadcastAddress[] = { 0xec, 0x64, 0xc9, 0xa9, 0x08, 0x88 };

void writeLeDs(char solenoids[6], char l1 = 'w', char l2 = 'w', char l3 = 'w', char connected = 0) {
  updateShiftRegister();
  int j = 0;

  // Set RGB LED 1 (l1) color
  ledState &= ~(1 << 0 | 1 << 4 | 1 << 8);  // Clear the bits for l1 (red, blue, green)
  setColor(l1, 0, 4, 8);                    // l1: red = bit 0, blue = bit 4, green = bit 8

  // Set RGB LED 2 (l2) color
  ledState &= ~(1 << 9 | 1 << 5 | 1 << 6);  // Clear the bits for l2 (red, blue, green)
  setColor(l2, 9, 5, 6);                    // l2: red = bit 9, blue = bit 5, green = bit 6

  // Set RGB LED 3 (l3) color
  ledState &= ~(1 << 15 | 1 << 12 | 1 << 13);  // Clear the bits for l3 (red, blue, green)
  setColor(l3, 15, 12, 13);                    // l3: red = bit 15, blue = bit 12, green = bit 13

  // Control solenoids based on the array
  for (int i = 0; i < 16; i++) {
    if (i == 3 || i == 7 || i == 11 || i == 14) {
      if (solenoids[j]) bitSet(ledState, i);
      else bitClear(ledState, i);
      j++;
    } else if (i == 1) {
      if (solenoids[j]) bitSet(ledState, 2);
      else bitClear(ledState, 2);
      j++;
    } else if (i == 2) {
      if (solenoids[j]) bitSet(ledState, 1);
      else bitClear(ledState, 1);
      j++;
    } else if (i == 10) {
      if (connected) bitSet (ledState, 10);
      else bitClear(ledState, 10);
    }
  }

  // Update the shift register with the new states
  updateShiftRegister();
}

// Function to update the shift registers
void updateShiftRegister() {
  digitalWrite(latchPin, LOW);                                // Prepare to shift data
  shiftOut(dataPin, clockPin, MSBFIRST, highByte(ledState));  // Shift out the high byte (second shift register)
  shiftOut(dataPin, clockPin, MSBFIRST, lowByte(ledState));   // Shift out the low byte (first shift register)
  digitalWrite(latchPin, HIGH);                               // Latch the data into the output register
}

// Function to set the color based on the character code
void setColor(char color, int redBit, int blueBit, int greenBit) {
  switch (color) {
    case 'r':
      bitSet(ledState, redBit);  // Red on
      break;
    case 'g':
      bitSet(ledState, greenBit);  // Green on
      break;
    case 'b':
      bitSet(ledState, blueBit);  // Blue on
      break;
    case 'c':
      bitSet(ledState, greenBit);  // Cyan (Green + Blue)
      bitSet(ledState, blueBit);
      break;
    case 'y':
      bitSet(ledState, redBit);  // Yellow (Red + Green)
      bitSet(ledState, greenBit);
      break;
    case 'p':
      bitSet(ledState, redBit);  // Purple (Red + Blue)
      bitSet(ledState, blueBit);
      break;
    case 'w':
      bitSet(ledState, redBit);  // White (Red + Green + Blue)
      bitSet(ledState, greenBit);
      bitSet(ledState, blueBit);
      break;
    default:
      // No color (all bits remain clear)
      break;
  }
}

void onSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == ESP_NOW_SEND_SUCCESS) {
    connected = 1;
  } else {
    connected = 0;
    Serial.println("dw");
  }
}

void onDataRecv(const esp_now_recv_info* mac, const unsigned char* incomingData, int len) {
  memcpy(&robotData, incomingData, sizeof(robotData));
  Serial.println("Received robot status:");

  // Print solenoid statuses
  for (int i = 0; i < 6; i++) {
    Serial.print("Solenoid ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(robotData.solenoidStatus[i] ? "Active" : "Inactive");
  }

  // Print motor speeds
  for (int i = 0; i < 16; i++) {
    Serial.print("Motor ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(robotData.motorSpeeds[i]);
  }

  for (int i = 0; i < 10; i++) {
    Serial.print("Ultrasonic Distances ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(robotData.ultrasonicDistance[i]);
  }
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
  esp_now_register_recv_cb(onDataRecv);

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

  // Initialize solenoid buttons as input
  for (int i = 0; i < 6; i++) {
    pinMode(solenoidPins[i], INPUT_PULLUP);
  }

  // Initialize master buttons as input
  for (int i = 0; i < 2; i++) {
    pinMode(masterPins[i], INPUT_PULLUP);
  }

  // Initialize joysticks as input
  pinMode(leftJoystickXPin, INPUT);
  pinMode(leftJoystickYPin, INPUT);
  pinMode(rightJoystickXPin, INPUT);
  pinMode(rightJoystickYPin, INPUT);

  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop() {
  // Read joystick values
  myData.leftX = analogRead(leftJoystickXPin);
  myData.leftY = analogRead(leftJoystickYPin);
  myData.rightX = analogRead(rightJoystickXPin);
  myData.rightY = analogRead(rightJoystickYPin);

  // Read main button states
  for (int i = 0; i < 8; i++) {
    myData.buttons[i] = digitalRead(buttonPins[i]) == LOW;  // LOW means pressed
  }

  // Read solenoid button states
  for (int i = 0; i < 6; i++) {
    myData.solenoids[i] = digitalRead(solenoidPins[i]) == LOW;  // LOW means pressed
  }

  // Read master button states
  for (int i = 0; i < 2; i++) {
    myData.master[i] = digitalRead(masterPins[i]) == LOW;  // LOW means pressed
  }

  writeLeDs(robotData.solenoidStatus, 'p','p','b', connected);

  // Send data via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }

  delay(100);  // Adjust the delay as needed
}
