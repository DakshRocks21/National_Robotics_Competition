#include <esp_now.h>
#include <WiFi.h>
#include <smorphi.h>

// Structure to receive joystick, button, solenoid, and master button data
typedef struct struct_message {
  int leftX;
  int leftY;
  int rightX;
  int rightY;
  bool buttons[8];
  bool solenoids[6];
  bool master[2];
} struct_message;

struct_message incomingData;

// Structure to send solenoid and motor status
typedef struct struct_response {
  char solenoidStatus[6];
  int motorSpeeds[16]; 
  float ultrasonicDistance[10];
} struct_response;

struct_response robotData;

// Initialize the Smorphi robot
Smorphi my_robot;

// Maximum movement speeds
int max_WASD_speed = 100;
int max_Rot_speed = 180;
int max_Dia_speed = 100;

uint8_t controllerAddress[] = {0xec,0x64,0xc9,0x98,0x68,0x1c};

// Function to update robotData with current solenoid and motor status
void updateRobotData() {
  // Example logic to update solenoidStatus and motorSpeeds
  for (int i = 0; i < 6; i++) {
    robotData.solenoidStatus[i] = my_robot.sm_feedback(i+1);
  }

  for (int i = 0; i < 16; i++) {
    robotData.motorSpeeds[i] = 20;
  }

  for (int i = 0; i < 10; i++) {
    robotData.ultrasonicDistance[i] = 2.0;
  }
}

// Callback function that gets called when data is received
void onDataRecv(const esp_now_recv_info * info, const uint8_t *controllerData, int len) { 
  memcpy(&incomingData, controllerData, sizeof(incomingData));

  // Calculate speed based on joystick input
  int speedLeftX = map(abs(incomingData.leftX-2047), 0, 2048, 0, max_WASD_speed);
  int speedLeftY = map(abs(incomingData.leftY-2047), 0, 2048, 0, max_WASD_speed);
  int speedRightX = map(abs(incomingData.rightX-2047), 0, 2048, 0, max_Rot_speed);
  int speedRightY = map(abs(incomingData.rightY-2047), 0, 2048, 0, max_Rot_speed);
  int speedDiagonal = map(max(abs(incomingData.leftX-2047), abs(incomingData.leftY-2047)), -2047, 2048, 0, max_Dia_speed);
  Serial.println("Speeds:");
  Serial.println(speedLeftX);
  Serial.println(speedLeftY);
  Serial.println(speedRightX);
  Serial.println(speedRightY);
  Serial.println(speedDiagonal);

  // Process joystick inputs for movement
  if(incomingData.rightX > 2000) {
    my_robot.CenterPivotLeft(speedRightX);
    Serial.println("Left Turn");
  }
  else if(incomingData.rightX < 1750) {
    my_robot.CenterPivotRight(speedRightX);
    Serial.println("Right Turn");
  }
  else if(incomingData.leftX > 2000 && incomingData.leftY > 2000) {
    my_robot.MoveDiagDownLeft(speedDiagonal);
    Serial.println("DownLeft");
  }
  else if(incomingData.leftX > 2000 && incomingData.leftY < 1750) {
    my_robot.MoveDiagUpLeft(speedDiagonal);
    Serial.println("UpLeft");
  }
  else if(incomingData.leftX < 1750 && incomingData.leftY > 2000) {
    my_robot.MoveDiagDownRight(speedDiagonal);
    Serial.println("DownRight");
  }
  else if(incomingData.leftX < 1750 && incomingData.leftY < 1750) {
    my_robot.MoveDiagUpRight(speedDiagonal);
    Serial.println("UpRight");
  }
  else if(incomingData.leftY > 2000) {
    my_robot.MoveBackward(speedLeftY);
    Serial.println("Backward");
  }
  else if(incomingData.leftY < 1750) {
    my_robot.MoveForward(speedLeftY);
    Serial.println("Forward");
  }
  else if(incomingData.leftX < 1750) {
    my_robot.MoveRight(speedLeftX);
    Serial.println("Right");
  }
  else if(incomingData.leftX > 2000) {
    my_robot.MoveLeft(speedLeftX);
    Serial.println("Left");
  }
  else {
    my_robot.stopSmorphi();
    Serial.println("Stop");
  }

  // Process button inputs for actions
  if(incomingData.buttons[0]) {
    my_robot.sm_reset_M1();
    my_robot.sm_reset_M2();
    my_robot.sm_reset_M3();
    my_robot.sm_reset_M4();
    my_robot.S(); 
    Serial.println("Shape S transform");
  }
  if(incomingData.buttons[1]) {
    my_robot.sm_reset_M1();
    my_robot.sm_reset_M2();
    my_robot.sm_reset_M3();
    my_robot.sm_reset_M4();
    my_robot.T();
    Serial.println("Shape T transform");
  }
  if(incomingData.buttons[2]) {
    my_robot.sm_reset_M1();
    my_robot.sm_reset_M2();
    my_robot.sm_reset_M3();
    my_robot.sm_reset_M4();
    my_robot.Z();
    Serial.println("Shape Z transform");
  }
  if(incomingData.buttons[3]) {
    my_robot.sm_reset_M1();
    my_robot.sm_reset_M2();
    my_robot.sm_reset_M3();
    my_robot.sm_reset_M4();
    my_robot.J();
    Serial.println("Shape J transform");
  }
  if(incomingData.buttons[4]) {
    my_robot.sm_reset_M1();
    my_robot.sm_reset_M2();
    my_robot.sm_reset_M3();
    my_robot.sm_reset_M4();
    my_robot.O();
    Serial.println("Shape O transform");
  }
  if(incomingData.buttons[5]) {
    my_robot.sm_reset_M1();
    my_robot.sm_reset_M2();
    my_robot.sm_reset_M3();
    my_robot.sm_reset_M4();
    my_robot.L();
    Serial.println("Shape L transform");
  }
  if(incomingData.buttons[6]) {
    my_robot.sm_reset_M1();
    my_robot.sm_reset_M2();
    my_robot.sm_reset_M3();
    my_robot.sm_reset_M4();
    my_robot.I();
    Serial.println("Shape I transform");
  }

  // Process solenoid button inputs
  for (int i = 0; i < 6; i++) {
    if (incomingData.solenoids[i]) {
      // Activate corresponding solenoid
      my_robot.sm_solenoid_set(i+1, true);
      Serial.print("Solenoid ");
      Serial.print(i + 1);
      Serial.println(" activated");
    } else {
      my_robot.sm_solenoid_set(i+1, false);
    }
  }

  // Process master button inputs
  if (incomingData.master[0]) {
    // Perform master button action 1
    Serial.println("Master button 1 pressed");
    my_robot.stopSmorphi();
    my_robot.sm_reset_M1();
    my_robot.sm_reset_M2();
    my_robot.sm_reset_M3();
    my_robot.sm_reset_M4();
  }
  if (incomingData.master[1]) {
    // Perform master button action 2
    for (int i = 0; i < 6; i++) {
      my_robot.sm_solenoid_set(i+1, true);
    }
    Serial.println("Master button 2 pressed");
    delay(1000);
    for (int j = 0; j < 6; j++) {
      my_robot.sm_solenoid_set(j+1, false);
    }
  }

  // Update robotData with the latest statuses
  updateRobotData();

  // Send the updated status back to the controller
  esp_err_t result = esp_now_send(controllerAddress, (uint8_t *) &robotData, sizeof(robotData));
  
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
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
  
  // Register peer (controller) to send data back
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, controllerAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  my_robot.I();
  Serial.println("Robot ready and waiting for commands...");
}

void loop() {
  // The loop is empty because we process data in the onDataRecv
}