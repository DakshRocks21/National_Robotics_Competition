#include <smorphi.h>
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

// Smorphi is instantiated as my_robot
Smorphi my_robot;
HUSKYLENS huskylens;        // Create an object for HuskyLens

SoftwareSerial myHuskySerial(16, 17); // RX is Pin 16, TX is Pin 17

void printResult(HUSKYLENSResult result); // Function for serial print results from the sensor

// I2C addresses of the four boards
#define RX_PIN 27
#define TX_PIN 26

// Structure to hold ultrasonic sensor data
typedef struct UltrasonicSensors {
  uint16_t module1[4];
  uint16_t module2[4];
  uint16_t module3[4];
  uint16_t module4[5];
} UltrasonicSensors;

UltrasonicSensors ultrasonicData;

void requestUltrasonicData() {
}

void changeShape(int colourID);
int detectColour();
void wallAlign();
void leftRightSweep();
void frontBackSweep();
void rightLeftSweep();
void backFrontSweep();

int color_signature = 0;
int command_block = 0;

const int SDA_PIN = 18;
const int SCL_PIN = 19;

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  Serial.println("Starting...");
  
  my_robot.BeginSmorphi();
  myHuskySerial.begin(9600);  // Baud rate for HuskyLens communication. Can change from GUI of the HuskyLens

  while (!huskylens.begin(myHuskySerial)) {  // Establishing communication with HuskyLens
      Serial.println(F("Begin failed!"));
      Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
      Serial.println(F("2.Please recheck the connection."));
      delay(100);
  }
  Serial.println("Success!");
}

void getUltrasonicData(){
}

void loop() {
  leftRightSweep();
}

int detectColour() { // Read HuskyLens data
  if (!huskylens.request()) {
    Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
  } 
  else if (!huskylens.isLearned()) {
    Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  } 
  else if (!huskylens.available()) {
    Serial.println(F("No block or arrow appears on the screen!"));
  } 
  else {
    Serial.println(F("###########"));
    while (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();
      Serial.print("Command: ");
      Serial.println(result.command); 
      Serial.print("ID: ");
      Serial.println(result.ID);
      color_signature = result.ID;
      command_block = result.command;
      return color_signature;
    }
  }
  return -1;
}

void changeShape(int colourID) { // Change shape based on colour
  uint16_t wall_distance; // TODO: Initialize this with an appropriate value

  // Check if there is enough space to change shape
  // Left side sensor check
  int wall = 0;
  do {
    uint16_t sideSensorsLeft[] = {ultrasonicData.module1[2], ultrasonicData.module2[2], ultrasonicData.module2[3], ultrasonicData.module3[3]};  // Combine all left side sensors
    for (uint16_t i = 0; i < sizeof(sideSensorsLeft)/sizeof(sideSensorsLeft[0]); i++) {
      if (sideSensorsLeft[i] < wall_distance) {  // Check if any sensor detects a wall
        wall = 1;
        my_robot.MoveRight(80);
        delay(100);
        break;
      }
    }
  } while (wall);

  // Right side sensor check
  wall = 0;
  do {
    uint16_t sideSensorsRight[] = {ultrasonicData.module1[3], ultrasonicData.module2[3], ultrasonicData.module1[2], ultrasonicData.module4[2]};  // Combine all right side sensors
    for (uint16_t i = 0; i < sizeof(sideSensorsRight)/sizeof(sideSensorsRight[0]); i++) {
      if (sideSensorsRight[i] < wall_distance) {  // Check if any sensor detects a wall
        wall = 1;
        my_robot.MoveLeft(80);
        delay(100);
        break;
      }
    }
  } while (wall);

  // Shape change logic based on colourID
  if (colourID == 1) {
    // TODO: change shape code 
    my_robot.O();
  }
  if (colourID == 2) {
    // TODO: change shape code 
    my_robot.O();
  }
  if (colourID == 3) {
    // TODO: change shape code 
    my_robot.O();
  }
}

void wallAlign(char side) {
  uint16_t threshold = 10;
  uint16_t sensors[2];
  if (side == 'L') {
    sensors[0] = ultrasonicData.module4[1]; // Update sensor array before use
    sensors[1] = ultrasonicData.module3[1];
    if (!(sensors[0] == 0 || sensors[1] == 0)){
      while (sensors[0]/sensors[1] <= (1 + threshold) && sensors[0]/sensors[1] >= (1 - threshold)) { // Not aligned
        if (sensors[0]/sensors[1] < (1 - threshold)) { // Robot is '\'
          my_robot.CenterPivotRight(80);
          delay(100);
        }
        if (sensors[0]/sensors[1] > (1 + threshold)) { // Robot is '/'
          my_robot.CenterPivotLeft(80);
          delay(100);
        }
      }
    }
  }

  if (side == 'R') {
    sensors[0] = ultrasonicData.module1[2]; // Update sensor array before use
    sensors[1] = ultrasonicData.module2[1];
    if (!(sensors[0] == 0 || sensors[1] == 0)){
      while (sensors[0]/sensors[1] <= (1 + threshold) && sensors[0]/sensors[1] >= (1 - threshold)) { // Not aligned
        if (sensors[0]/sensors[1] < (1 - threshold)) { // Robot is '\'
          my_robot.CenterPivotRight(80);
          delay(100);
        }
        if (sensors[0]/sensors[1] > (1 + threshold)) { // Robot is '/'
          my_robot.CenterPivotLeft(80);
          delay(100);
        }
      }
    }
  }

  if (side == 'F') {
    sensors[0] = ultrasonicData.module1[1]; // Update sensor array before use
    sensors[1] = ultrasonicData.module4[2];
    if (!(sensors[0] == 0 || sensors[1] == 0)){
      while (sensors[0]/sensors[1] <= (1 + threshold) && sensors[0]/sensors[1] >= (1 - threshold)) { // Not aligned
        if (sensors[0]/sensors[1] < (1 - threshold)) { // Robot is '\'
          my_robot.CenterPivotRight(80);
          delay(100);
        }
        if (sensors[0]/sensors[1] > (1 + threshold)) { // Robot is '/'
          my_robot.CenterPivotLeft(80);
          delay(100);
        }
      }
    }
  }

  if (side == 'B') {
    sensors[0] = ultrasonicData.module2[2]; // Update sensor array before use
    sensors[1] = ultrasonicData.module3[0];
    if (!(sensors[0] == 0 || sensors[1] == 0)){
      while (sensors[0]/sensors[1] <= (1 + threshold) && sensors[0]/sensors[1] >= (1 - threshold)) { // Not aligned
        if (sensors[0]/sensors[1] < (1 - threshold)) { // Robot is '\'
          my_robot.CenterPivotRight(80);
          delay(100);
        }
        if (sensors[0]/sensors[1] > (1 + threshold)) { // Robot is '/'
          my_robot.CenterPivotLeft(80);
          delay(100);
        }
      }
    }
  }
}

void leftRightSweep() {
  my_robot.O();
  uint16_t wall_distance;
  uint16_t threshold;
  int exit = 1;
  int wall = 0;
  int colour = 0;
  int counter = 0;

  requestUltrasonicData();
  

  // Declare arrays outside the loop
  uint16_t mapping[4] = {0, 0, 0, 0}; // x-left, x-right, y-up, y-down
  uint16_t old_mapping[4]; // Used for comparing
  uint16_t sideSensors[2]; // Used for side sensor checks
  uint16_t frontSensors[2]; // Used for front sensor checks

  while (exit == 1) {
    // Update old_mapping with the current mapping
    for (int i = 0; i < 4; i++) {
      old_mapping[i] = mapping[i];
    }

    // Update the mapping values
    mapping[0] = (ultrasonicData.module3[1] + ultrasonicData.module4[1]) / 2; // x-left
    mapping[1] = (ultrasonicData.module1[2] + ultrasonicData.module2[1]) / 2; // x-right
    mapping[2] = (ultrasonicData.module1[1] + ultrasonicData.module4[2]) / 2; // y-up
    mapping[3] = (ultrasonicData.module2[2] + ultrasonicData.module3[0]) / 2; // y-down

    // Move Left
    wall_distance = 30; // Set wall_distance before use
    sideSensors[0] = ultrasonicData.module3[1]; // Update sensor array before use
    sideSensors[1] = ultrasonicData.module4[1];
    do {
      wall = 0;
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for side walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0) {
        break;
      }
      my_robot.MoveLeft(80);
      delay(100);
    } while (!wall);
    wallAlign('R');

    // Front sensor checks
    threshold = 10;
    wall_distance = 30;
    frontSensors[0] = ultrasonicData.module1[1]; // Update sensor array before use 
    frontSensors[1] = ultrasonicData.module4[2];
    do {
      wall = 0;
      for (uint16_t i = 0; i < sizeof(frontSensors) / sizeof(frontSensors[0]); i++) {
        if (frontSensors[i] < wall_distance) { // Check for front walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      if (!(frontSensors[0] == 0 || frontSensors[1] == 0 || frontSensors[2] == 0)){
        if (frontSensors[0] / frontSensors[1] <= (1 + threshold) || frontSensors[0] / frontSensors[1] >= (1 - threshold)) { // Flat wall
          colour = detectColour();
          if (colour > 0) {
            break;
          }
          my_robot.MoveForward(80);
          delay(100);
        }

        if (frontSensors[0] / frontSensors[1] > (1 + threshold)) { // Slated wall '\'
          colour = detectColour();
          if (colour > 0) {
            break;
          }
          my_robot.MoveDiagUpLeft(80);
          delay(100);
        }

        if (frontSensors[0] / frontSensors[1] < (1 - threshold)) { // Slated wall '/'
          colour = detectColour();
          if (colour > 0) {
            break;
          }
          my_robot.MoveDiagUpRight(80);
          delay(100);
        }
      }
    }while (!wall);
    // Move Right
    wall_distance = 30;
    sideSensors[0] = ultrasonicData.module1[1]; // Update sensor array before use
    sideSensors[1] = ultrasonicData.module4[2];
    do {
      wall = 0;
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for side walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0) {
        break;
      }
      my_robot.MoveRight(80);
      delay(100);
    } while (!wall);
    wallAlign('L');

    // If missing area
    if (mapping[0] > old_mapping[0] + threshold) {
      if (counter != 1){
        do {
          my_robot.MoveBackward(80);
          counter++;
        } while (mapping[3] > 0);
      }
    }

    // Handle corner case
    if ((mapping[0] == old_mapping[0] + threshold || mapping[0] == old_mapping[0] - threshold) && mapping[0] < 2000) { // Cannot move forward
      wall_distance = 200;
      int left_wall = 0;
      sideSensors[0] = ultrasonicData.module3[1]; // Update sensor array before use
      sideSensors[1] = ultrasonicData.module4[1];
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for side walls
          left_wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }

      int right_wall = 0;
      sideSensors[0] = ultrasonicData.module1[1]; // Update sensor array before use
      sideSensors[1] = ultrasonicData.module4[2];
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for side walls
          right_wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }

      if (left_wall) {
        do {
          my_robot.MoveRight(80);
          delay(100);
          colour = detectColour();
        } while (colour < 0);
      }
      if (right_wall) {
        do {
          my_robot.MoveLeft(80);
          delay(100);
          colour = detectColour();
        } while (colour < 0);
      }
    }

    // Threshold check
    if (mapping[0] == old_mapping[0] + threshold || mapping[0] == old_mapping[0] - threshold) { // horizontal distance is equal
      wall_distance = 200;
      sideSensors[0] = ultrasonicData.module1[2]; // Update sensor array before use
      sideSensors[1] = ultrasonicData.module4[2];
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for side walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      if (wall){
        changeShape(colour);
        exit = 0;
      }
    }
  }
}

void frontBackSweep() {
  my_robot.O();  // Set robot to 'O' shape
  uint16_t wall_distance;
  uint16_t threshold = 10; 
  int exit = 1;
  int wall = 0;
  int colour = 0;

  requestUltrasonicData();

  uint16_t mapping[4] = {0, 0, 0, 0}; // x-left, x-right, y-up, y-down
  uint16_t old_mapping[4]; // For comparison
  uint16_t sideSensors[2]; // For side (left-right) sensor checks
  uint16_t frontSensors[2]; // For front-back sensor checks

  while (exit == 1) {
    // Update old_mapping with the current mapping
    for (int i = 0; i < 4; i++) {
      old_mapping[i] = mapping[i];
    }

    // Update the mapping values (adjust for up-down motion)
    mapping[0] = (ultrasonicData.module2[1] + ultrasonicData.module3[1]) / 2; // x-left
    mapping[1] = (ultrasonicData.module2[2] + ultrasonicData.module3[2]) / 2; // x-right
    mapping[2] = (ultrasonicData.module1[0] + ultrasonicData.module2[0]) / 2; // y-up
    mapping[3] = (ultrasonicData.module3[4] + ultrasonicData.module4[4]) / 2; // y-down

    // Move Forward
    wall_distance = 200; // Set wall_distance before use
    frontSensors[0] = ultrasonicData.module1[0]; // Update front sensor array before use
    frontSensors[1] = ultrasonicData.module2[0];
    do {
      wall = 0;
      for (uint16_t i = 0; i < sizeof(frontSensors) / sizeof(frontSensors[0]); i++) {
        if (frontSensors[i] < wall_distance) { // Check for front walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0) {
        break;
      }
      my_robot.MoveForward(80);
      delay(100);
    } while (!wall);
    wallAlign('B');  // Align backward after forward sweep

    // Move Left
    wall_distance = 200;
    sideSensors[0] = ultrasonicData.module2[1]; // Update side sensor array before use
    sideSensors[1] = ultrasonicData.module3[1];
    do {
      wall = 0;
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for left walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0) {
        break;
      }
      my_robot.MoveLeft(80);
      delay(100);
    } while (!wall);
    wallAlign('R');  // Align right after left movement

    // Move Backward
    wall_distance = 200;
    frontSensors[0] = ultrasonicData.module3[4]; // Update rear sensor array before use
    frontSensors[1] = ultrasonicData.module4[4];
    do {
      wall = 0;
      for (uint16_t i = 0; i < sizeof(frontSensors) / sizeof(frontSensors[0]); i++) {
        if (frontSensors[i] < wall_distance) { // Check for rear walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0) {
        break;
      }
      my_robot.MoveBackward(80);
      delay(100);
    } while (!wall);
    wallAlign('F');  // Align forward after backward sweep

    // Move Left again
    wall_distance = 200;
    sideSensors[0] = ultrasonicData.module2[1]; // Update side sensor array before use
    sideSensors[1] = ultrasonicData.module3[1];
    do {
      wall = 0;
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for left walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0) {
        break;
      }
      my_robot.MoveLeft(80);
      delay(100);
    } while (!wall);
    wallAlign('R');  // Align right after left movement

    // If missing vertical area
    if (mapping[2] > old_mapping[2] + threshold) {
      do {
        my_robot.MoveRight(80);  // Correct by moving right
        delay(100);
      } while (mapping[1] > 0);
    }

    // Handle corner case
    if ((mapping[2] == old_mapping[2] + threshold || mapping[2] == old_mapping[2] - threshold) && mapping[2] < 2000) { // Cannot move forward
      wall_distance = 200;
      int left_wall = 0;
      sideSensors[0] = ultrasonicData.module1[2]; // Update side sensor array before use
      sideSensors[1] = ultrasonicData.module4[2];
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for side walls
          left_wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }

      int right_wall = 0;
      sideSensors[0] = ultrasonicData.module2[1]; // Update side sensor array before use
      sideSensors[1] = ultrasonicData.module3[1];
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for side walls
          right_wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }

      if (left_wall) {
        do {
          my_robot.MoveRight(80);
          delay(100);
          colour = detectColour();
        } while (colour < 0);
      }
      if (right_wall) {
        do {
          my_robot.MoveLeft(80);
          delay(100);
          colour = detectColour();
        } while (colour < 0);
      }
    }

    // Threshold check
    if (mapping[2] == old_mapping[2] + threshold || mapping[2] == old_mapping[2] - threshold) { // vertical distance is equal
      wall_distance = 200;
      sideSensors[0] = ultrasonicData.module1[2]; // Update sensor array before use
      sideSensors[1] = ultrasonicData.module4[2];
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for side walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      changeShape(colour);
      exit = 0;
    }
  }
}

void rightLeftSweep() {
  my_robot.O();  // Set robot to 'O' shape
  uint16_t wall_distance;
  uint16_t threshold = 5;  // Threshold for determining missed areas or alignment
  int exit = 1;
  int wall = 0;
  int colour = 0;

  requestUltrasonicData();

  uint16_t mapping[4] = {0, 0, 0, 0}; // x-left, x-right, y-up, y-down
  uint16_t old_mapping[4]; // For comparison
  uint16_t sideSensors[2]; // For side (left-right) sensor checks
  uint16_t frontSensors[2]; // For front-back sensor checks

  while (exit == 1) {
    // Update old_mapping with the current mapping
    for (int i = 0; i < 4; i++) {
      old_mapping[i] = mapping[i];
    }

    // Update the mapping values (adjust for up-down motion)
    mapping[0] = (ultrasonicData.module2[1] + ultrasonicData.module3[1]) / 2; // x-left
    mapping[1] = (ultrasonicData.module2[2] + ultrasonicData.module3[2]) / 2; // x-right
    mapping[2] = (ultrasonicData.module1[0] + ultrasonicData.module2[0]) / 2; // y-up
    mapping[3] = (ultrasonicData.module3[4] + ultrasonicData.module4[4]) / 2; // y-down

    // Move Right
    wall_distance = 200; // Set wall_distance before use
    sideSensors[0] = ultrasonicData.module2[2]; // Update side sensor array before use
    sideSensors[1] = ultrasonicData.module3[2];
    do {
      wall = 0;
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for right walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0) {
        break;
      }
      my_robot.MoveRight(80);
      delay(100);
    } while (!wall);
    wallAlign('L');  // Align left after right movement

    // Move Front
    wall_distance = 200;
    frontSensors[0] = ultrasonicData.module1[0]; // Update front sensor array before use
    frontSensors[1] = ultrasonicData.module2[0];
    do {
      wall = 0;
      for (uint16_t i = 0; i < sizeof(frontSensors) / sizeof(frontSensors[0]); i++) {
        if (frontSensors[i] < wall_distance) { // Check for front walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0) {
        break;
      }
      my_robot.MoveForward(80);
      delay(100);
    } while (!wall);
    wallAlign('B');  // Align backward after front movement

    // Move Left
    wall_distance = 200;
    sideSensors[0] = ultrasonicData.module2[1]; // Update side sensor array before use
    sideSensors[1] = ultrasonicData.module3[1];
    do {
      wall = 0;
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for left walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0) {
        break;
      }
      my_robot.MoveLeft(80);
      delay(100);
    } while (!wall);
    wallAlign('R');  // Align right after left movement

    // Move Front again
    wall_distance = 200;
    frontSensors[0] = ultrasonicData.module1[0]; // Update front sensor array before use
    frontSensors[1] = ultrasonicData.module2[0];
    do {
      wall = 0;
      for (uint16_t i = 0; i < sizeof(frontSensors) / sizeof(frontSensors[0]); i++) {
        if (frontSensors[i] < wall_distance) { // Check for front walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0) {
        break;
      }
      my_robot.MoveForward(80);
      delay(100);
    } while (!wall);
    wallAlign('B');  // Align backward after front movement

    // If missing vertical area
    if (mapping[2] > old_mapping[2] + threshold) {
      do {
        my_robot.MoveBackward(80);  // Correct by moving backward
        delay(100);
      } while (mapping[3] > 0);
    }

    // Handle corner case
    if ((mapping[2] == old_mapping[2] + threshold || mapping[2] == old_mapping[2] - threshold) && mapping[2] < 2000) { // Cannot move forward
      wall_distance = 200;
      int left_wall = 0;
      sideSensors[0] = ultrasonicData.module1[2]; // Update side sensor array before use
      sideSensors[1] = ultrasonicData.module4[2];
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for side walls
          left_wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }

      int right_wall = 0;
      sideSensors[0] = ultrasonicData.module2[1]; // Update side sensor array before use
      sideSensors[1] = ultrasonicData.module3[1];
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for side walls
          right_wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }

      if (left_wall) {
        do {
          my_robot.MoveRight(80);
          delay(100);
          colour = detectColour();
        } while (colour < 0);
      }
      if (right_wall) {
        do {
          my_robot.MoveLeft(80);
          delay(100);
          colour = detectColour();
        } while (colour < 0);
      }
    }

    // Threshold check
    if (mapping[2] == old_mapping[2] + threshold || mapping[2] == old_mapping[2] - threshold) { // vertical distance is equal
      wall_distance = 200;
      sideSensors[0] = ultrasonicData.module1[2]; // Update sensor array before use
      sideSensors[1] = ultrasonicData.module4[2];
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for side walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      changeShape(colour);
      exit = 0;
    }
  }
}

void backFrontSweep() {
  my_robot.O();  // Set robot to 'O' shape
  uint16_t wall_distance;
  uint16_t threshold = 5;  // Threshold for determining missed areas or alignment
  int exit = 1;
  int wall = 0;
  int colour = 0;

  requestUltrasonicData();

  uint16_t mapping[4] = {0, 0, 0, 0}; // x-left, x-right, y-up, y-down
  uint16_t old_mapping[4]; // For comparison
  uint16_t sideSensors[2]; // For side (left-right) sensor checks
  uint16_t frontSensors[2]; // For front-back sensor checks

  while (exit == 1) {
    // Update old_mapping with the current mapping
    for (int i = 0; i < 4; i++) {
      old_mapping[i] = mapping[i];
    }

    // Update the mapping values (adjust for up-down motion)
    mapping[0] = (ultrasonicData.module2[1] + ultrasonicData.module3[1]) / 2; // x-left
    mapping[1] = (ultrasonicData.module2[2] + ultrasonicData.module3[2]) / 2; // x-right
    mapping[2] = (ultrasonicData.module1[0] + ultrasonicData.module2[0]) / 2; // y-up
    mapping[3] = (ultrasonicData.module3[4] + ultrasonicData.module4[4]) / 2; // y-down

    // Move Backward
    wall_distance = 200; // Set wall_distance before use
    frontSensors[0] = ultrasonicData.module3[4]; // Update rear sensor array before use
    frontSensors[1] = ultrasonicData.module4[4];
    do {
      wall = 0;
      for (uint16_t i = 0; i < sizeof(frontSensors) / sizeof(frontSensors[0]); i++) {
        if (frontSensors[i] < wall_distance) { // Check for rear walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0) {
        break;
      }
      my_robot.MoveBackward(80);
      delay(100);
    } while (!wall);
    wallAlign('F');  // Align forward after backward movement

    // Move Left
    wall_distance = 200;
    sideSensors[0] = ultrasonicData.module2[1]; // Update side sensor array before use
    sideSensors[1] = ultrasonicData.module3[1];
    do {
      wall = 0;
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for left walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0) {
        break;
      }
      my_robot.MoveLeft(80);
      delay(100);
    } while (!wall);
    wallAlign('R');  // Align right after left movement

    // Move Forward
    wall_distance = 200;
    frontSensors[0] = ultrasonicData.module1[0]; // Update front sensor array before use
    frontSensors[1] = ultrasonicData.module2[0];
    do {
      wall = 0;
      for (uint16_t i = 0; i < sizeof(frontSensors) / sizeof(frontSensors[0]); i++) {
        if (frontSensors[i] < wall_distance) { // Check for front walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0) {
        break;
      }
      my_robot.MoveForward(80);
      delay(100);
    } while (!wall);
    wallAlign('B');  // Align backward after forward movement

    // Move Left again
    wall_distance = 200;
    sideSensors[0] = ultrasonicData.module2[1]; // Update side sensor array before use
    sideSensors[1] = ultrasonicData.module3[1];
    do {
      wall = 0;
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for left walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0) {
        break;
      }
      my_robot.MoveLeft(80);
      delay(100);
    } while (!wall);
    wallAlign('R');  // Align right after left movement

    // If missing vertical area
    if (mapping[2] > old_mapping[2] + threshold) {
      do {
        my_robot.MoveRight(80);  // Correct by moving right
        delay(100);
      } while (mapping[1] > 0);
    }

    // Handle corner case
    if ((mapping[2] == old_mapping[2] + threshold || mapping[2] == old_mapping[2] - threshold) && mapping[2] < 2000) { // Cannot move forward
      wall_distance = 200;
      int left_wall = 0;
      sideSensors[0] = ultrasonicData.module1[2]; // Update side sensor array before use
      sideSensors[1] = ultrasonicData.module4[2];
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for side walls
          left_wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }

      int right_wall = 0;
      sideSensors[0] = ultrasonicData.module2[1]; // Update side sensor array before use
      sideSensors[1] = ultrasonicData.module3[1];
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for side walls
          right_wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }

      if (left_wall) {
        do {
          my_robot.MoveRight(80);
          delay(100);
          colour = detectColour();
        } while (colour < 0);
      }
      if (right_wall) {
        do {
          my_robot.MoveLeft(80);
          delay(100);
          colour = detectColour();
        } while (colour < 0);
      }
    }

    // Threshold check
    if (mapping[2] == old_mapping[2] + threshold || mapping[2] == old_mapping[2] - threshold) { // vertical distance is equal
      wall_distance = 200;
      sideSensors[0] = ultrasonicData.module1[2]; // Update sensor array before use
      sideSensors[1] = ultrasonicData.module4[2];
      for (uint16_t i = 0; i < sizeof(sideSensors) / sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) { // Check for side walls
          wall = 1; // Set wall to 1 near wall
          break; // Exit the loop once the condition is met
        }
      }
      changeShape(colour);
      exit = 0;
    }
  }
}
