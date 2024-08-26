#include <esp_now.h>
#include <WiFi.h>
#include <smorphi.h>
//0xec,0x64,0xc9,0x98,0x68,0x1c


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

// Movement parameters
int WASD_speed = 180;
int Rot_speed = 180;
int Dia_speed = 100;

uint8_t controllerAddress[] = {0xec,0x64,0xc9,0x98,0x68,0x1c};
// Function to update robotData with current solenoid and motor status
void updateRobotData() {
  // Example logic to update solenoidStatus and motorSpeeds
  for (int i = 0; i < 6; i++) {
    robotData.solenoidStatus[i] = my_robot.sm_feedback(i+1);
  }

  // Update motor speeds (example values, replace with actual motor speed values)
  // robotData.motorSpeeds[0] = my_robot.GetMotorSpeed(1);
  // robotData.motorSpeeds[1] = my_robot.GetMotorSpeed(2);
  // robotData.motorSpeeds[2] = my_robot.GetMotorSpeed(3);
  // robotData.motorSpeeds[3] = my_robot.GetMotorSpeed(4);
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

  // Process joystick inputs for movement
  if(incomingData.rightX > 15) {
    my_robot.CenterPivotLeft(Rot_speed);
    Serial.println("Left Turn");
  }
  else if(incomingData.rightX < -15) {
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
  else if(incomingData.leftX < -15 && abs(incomingData.leftY) < 15) {
    my_robot.MoveRight(WASD_speed);
    Serial.println("Right");
  }
  else if(incomingData.leftX > 15 && abs(incomingData.leftY) < 15) {
    my_robot.MoveLeft(WASD_speed);
    Serial.println("Left");
  }
   else if(incomingData.leftX > 35 && incomingData.leftY > 35) {
    my_robot.MoveDiagDownLeft(Dia_speed);
    Serial.println("DownLeft");
  }
   else if(incomingData.leftX > 35 && incomingData.leftY < -35) {
    my_robot.MoveDiagUpLeft(Dia_speed);
    Serial.println("UpLeft");
  }
  else if(incomingData.leftX < -35 && incomingData.leftY > 35) {
    my_robot.MoveDiagDownRight(Dia_speed);
    Serial.println("DownRight");
  }
  else if(incomingData.leftX < -35 && incomingData.leftY < -35) {
    my_robot.MoveDiagUpRight(Dia_speed);
    Serial.println("UpRight");
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
  if(incomingData.buttons[7]) {
    my_robot.sm_reset_M1();
    my_robot.sm_reset_M2();
    my_robot.sm_reset_M3();
    my_robot.sm_reset_M4();
    my_robot.CenterPivotLeft(50);
    Serial.println("Left Turn");
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
  // The loop is empty because we process data in the onDataRecv callback
}
