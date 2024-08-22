#include <esp_now.h>
#include <WiFi.h>
#include <smorphi.h>

// Structure to receive joystick and button data
typedef struct struct_message {
  int leftX;
  int leftY;
  int rightX;
  int rightY;
  bool buttons[8];
} struct_message;

struct_message incomingData;

// Initialize the Smorphi robot
Smorphi my_robot;

// Movement parameters
int WASD_speed = 50;
int Rot_speed = 50;
int Dia_speed = 50;

// Callback function that gets called when data is received
void onDataRecv(const esp_now_recv_info * info, const uint8_t *yoloWorld, int len) { 
  memcpy(&incomingData, yoloWorld, sizeof(incomingData));

  // Process joystick inputs for movement
  if(incomingData.rightX < -15) {
    my_robot.CenterPivotLeft(Rot_speed);
    Serial.println("Left Turn");
  }
  else if(incomingData.rightX > 15) {
    my_robot.CenterPivotRight(Rot_speed);
    Serial.println("Right Turn");
  }
  else if(incomingData.leftY > 15 && abs(incomingData.leftX) < 35) {
    my_robot.MoveBackward(WASD_speed);
    Serial.println("Backward");
  }
  else if(incomingData.leftY < -15 && abs(incomingData.leftX) < 35) {
    my_robot.MoveForward(WASD_speed);
    Serial.println("Forward");
  }
  else if(incomingData.leftX > 15 && abs(incomingData.leftY) < 15) {
    my_robot.MoveRight(WASD_speed);
    Serial.println("Right");
  }
  else if(incomingData.leftX < -15 && abs(incomingData.leftY) < 15) {
    my_robot.MoveLeft(WASD_speed);
    Serial.println("Left");
  }
  else if(incomingData.leftX < -35 && incomingData.leftY > 35) {
    my_robot.MoveDiagDownLeft(Dia_speed);
    Serial.println("DownLeft");
  }
  else if(incomingData.leftX < -35 && incomingData.leftY < -35) {
    my_robot.MoveDiagUpLeft(Dia_speed);
    Serial.println("UpLeft");
  }
  else if(incomingData.leftX > 35 && incomingData.leftY > 35) {
    my_robot.MoveDiagDownRight(Dia_speed);
    Serial.println("DownRight");
  }
  else if(incomingData.leftX > 35 && incomingData.leftY < -35) {
    my_robot.MoveDiagUpRight(Dia_speed);
    Serial.println("UpRight");
  }
  else {
    my_robot.stopSmorphi();
    Serial.println("Stop");
  }

  // Process button inputs for actions
  if(incomingData.buttons[0]) {
    my_robot.S();
    Serial.println("Shape S transform");
  }
  if(incomingData.buttons[1]) {
    my_robot.T();
    Serial.println("Shape T transform");
  }
  if(incomingData.buttons[2]) {
    my_robot.Z();
    Serial.println("Shape Z transform");
  }
  if(incomingData.buttons[3]) {
    my_robot.J();
    Serial.println("Shape J transform");
  }
  if(incomingData.buttons[4]) {
    my_robot.O();
    Serial.println("Shape O transform");
  }
  if(incomingData.buttons[5]) {
    my_robot.L();
    Serial.println("Shape L transform");
  }
  if(incomingData.buttons[6]) {
    my_robot.I();
    Serial.println("Shape I transform");
  }
  if(incomingData.buttons[7]) {
    my_robot.CenterPivotLeft(50);
    Serial.println("Left Turn");
  }
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize Smorphi robot
  my_robot.BeginSmorphi();

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register receive callback
  esp_now_register_recv_cb(onDataRecv);

  Serial.println("Robot readdy and waiting for commands...");
}

void loop() {
  // The loop is empty because we process data in the onDataRecv callback
}
