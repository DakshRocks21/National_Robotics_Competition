// Inclusion of necessary libraries
#include <smorphi.h>
#include "HUSKYLENS.h"      // Library for HuskyLens
#include "SoftwareSerial.h" // Library for Software 

// Smorphi is instantiated as my_robot
Smorphi my_robot;
HUSKYLENS huskylens;        // Create an object for HuskyLens

SoftwareSerial myHuskySerial(16, 17); // RX is Pin 26, TX is Pin 27

void printResult(HUSKYLENSResult result); //Function for serial print results from the sensor

struct UltrasonicSensors {
  unsigned int module1[4];
  unsigned int module2[4];
  unsigned int module3[4];
  unsigned int module4[5];
};
UltrasonicSensors ultrasonicData;

void changeShape(int colourID, char currentShape);
int color_signature;
int command_block;


void setup() {
  // put your setup code here, to run once:
  Serial.println("Starting...");
  Serial.begin(115200);
  my_robot.BeginSmorphi();
  myHuskySerial.begin(9600);         //Baud rate for HuskyLens communication. Can change from GUI of the HuskyLens
  while (!huskylens.begin(myHuskySerial)) //Establishing communication with HuskyLens
  {
      Serial.println(F("Begin failed!"));
      Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
      Serial.println(F("2.Please recheck the connection."));
      delay(100);
  }
  Serial.println("Success!");
}

void loop() {
}

int detectColour(){
  if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
  else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
  else{
    Serial.println(F("###########"));
    while (huskylens.available()){
      HUSKYLENSResult result = huskylens.read();
      Serial.print("Command: ");
      Serial.println(result.command); 
      Serial.print("ID:");
      Serial.println(result.ID);
      color_signature = result.ID;
      command_block = result.command;
      return color_signature;
      break
    }
  }
  return -1;
}

void changeShape(int colourID, char currentShape){
  int wall = 0;
  
  // Check if there is enough space to change shape
  if (currentShape == 'I'){
    unsigned int wall_distance; //TODO
    // Write code to check for left and right wall based on sensor info
    do{
      unsigned int sideSensors[] = {module1[2], module2[2]};  // TODO: Initialize the array with left side sensors
      for (unsigned int i = 0; i < sizeof(sideSensors)/sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) {  // Check for side walls
            wall = 1;  // Set wall to 1 near wall
            my_robot.MoveRight(80);
            delay(100);
            break;  // Exit the loop once the condition is met
        }
      }
      unsigned int sideSensors[] = {module1[3], module2[3]};  // TODO: Initialize the array with right side sensors
      for (unsigned int i = 0; i < sizeof(sideSensors)/sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) {  // Check for side walls
            wall = 1;  // Set wall to 1 near wall
            my_robot.MoveLeft(80);
            break;  // Exit the loop once the condition is met
        }
      }
    }while(wall);
  }
  if (currentShape == "O"){
    unsigned int wall_distance; //TODO
    // Write code to check for left and right wall based on sensor info
    do{
      unsigned int sideSensors[] = {module2[3], module3[3]};  // TODO: Initialize the array with left side sensors
      for (unsigned int i = 0; i < sizeof(sideSensors)/sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) {  // Check for side walls
            wall = 1;  // Set wall to 1 near wall
            my_robot.MoveRight(80);
            delay(100);
            break;  // Exit the loop once the condition is met
        }
      }
      unsigned int sideSensors[] = {module1[2], module4[2]};  // TODO: Initialize the array with right side sensors
      for (unsigned int i = 0; i < sizeof(sideSensors)/sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) {  // Check for side walls
            wall = 1;  // Set wall to 1 near wall
            my_robot.MoveLeft(80);
            break;  // Exit the loop once the condition is met
        }
      }
    }while(wall);
  }
  if (currentShape == "L"){
    unsigned int wall_distance; //TODO
    // Write code to check for left and right wall based on sensor info
    do{
      unsigned int sideSensors[] = {module2[3], module3[3]};  // TODO: Initialize the array with left side sensors
      for (unsigned int i = 0; i < sizeof(sideSensors)/sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) {  // Check for side walls
            wall = 1;  // Set wall to 1 near wall
            my_robot.MoveRight(80);
            delay(100);
            break;  // Exit the loop once the condition is met
        }
      }
      unsigned int sideSensors[] = {module1[2], module4[2]};  // TODO: Initialize the array with right side sensors
      for (unsigned int i = 0; i < sizeof(sideSensors)/sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) {  // Check for side walls
            wall = 1;  // Set wall to 1 near wall
            my_robot.MoveLeft(80);
            break;  // Exit the loop once the condition is met
        }
      }
    }while(wall);
  }

  if (colourID == 1){
    // TODO: change shape code 
    my_robot.O();
  }
  if (colourID == 2){
    // TODO: change shape code 
  }
  if (colourID == 3){
    // TODO: change shape code 
  }
}

void leftRightSweep(int colourID){
  my_robot.O();
  unsigned int wall_distance;
  int wall = 0;
  char shape = my_robot.sm_getShape()

  while (1){
    int colour = 0;
    unsigned int mapping[4] = {0,0,0,0} // x-left, x-right, y-up, y-down
    unsigned int old_mapping[4]; // Used for comparing
    for (int i = 0, i < 4, i++){
      old_mapping[i] = mapping[i];
    }
    mapping[0] = (module2[1] + module3[1])/2; // x-left
    mapping[1] = (module2[2] + module3[2])/2; // x-right
    mapping[2] = (module1[0] + module2[0])/2; // y-up
    mapping[3] = (module3[4] + module4[4])/2; // y-down

    // Move Left
    do{
      float wall_distance;
      unsigned int sideSensors[] = {module2[1], module3[1]};  // Initialize the array with side sensors
      for (unsigned int i = 0; i < sizeof(sideSensors)/sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) {  // Check for side walls
          wall = 1;  // Set wall to 1 near wall
          break;  // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0){
        break;
      }
      my_robot.MoveLeft(80);
      delay(100);
    }while (!wall);

    do{
      float threshold;
      float wall_distance;
      unsigned int frontSensors[] = {module1[0], module2[0]};  // Initialize the array with side sensors
      for (unsigned int i = 0; i < sizeof(frontSensors)/sizeof(frontSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) {  // Check for side walls
          wall = 1;  // Set wall to 1 near wall
          break;  // Exit the loop once the condition is met
        }
      }
      if (module1[0]/module2[0] < (1+threshold) || module1[0]/module2[0] > (1-threshold)){ // Flat wall
        colour = detectColour();
        if (colour > 0){
          break;
        }
        my_robot.MoveForward(80);
        delay(100);
      }

      if (module1[0]/module2[0] > (1+threshold)){ // Slated wall '\'
        colour = detectColour();
        if (colour > 0){
          break;
        }
        my_robot.MoveDiagUpLeft(80);
        delay(100);
      }

      if (module1[0]/module2[0] < (1-threshold)){ // Slated wall '/'
        colour = detectColour();
        if (colour > 0){
          break;
        }
        my_robot.MoveDiagUpRight(80);
        delay(100);
      }
    }while (!wall);

    do{
      float wall_distance;
      unsigned int sideSensors[] = {module1[2], module4[2]};  // Initialize the array with side sensors
      for (unsigned int i = 0; i < sizeof(sideSensors)/sizeof(sideSensors[0]); i++) {
        if (sideSensors[i] < wall_distance) {  // Check for side walls
          wall = 1;  // Set wall to 1 near wall
          break;  // Exit the loop once the condition is met
        }
      }
      colour = detectColour();
      if (colour > 0){
        break;
      }
      my_robot.MoveRight(80);
      delay(100);
    }while (!wall);

    if (mapping[0] > old_mapping[0] + threshold){
      do{
        my_robot.MoveBackwards(80);
      }while (mapping[3] > 0);
    }
    if (mapping[0] == old_mapping[0] + threshold || mapping[0] == old_mapping[0] - threshold){
      if (mapping[1])
    }
    else{
      unsigned int threshold;
      if (mapping[0] == old_mapping[0] + threshold || mapping[0] == old_mapping[0] - threshold){
        if (colour == 1){
          float wall_distance;
          unsigned int sideSensors[] = {module1[2], module4[2]};  // Initialize the array with side sensors
          for (unsigned int i = 0; i < sizeof(sideSensors)/sizeof(sideSensors[0]); i++) {
            if (sideSensors[i] < wall_distance) {  // Check for side walls
              wall = 1;  // Set wall to 1 near wall
              break;  // Exit the loop once the condition is met
            }
          }

          // change shape
        }
        if (colour == 2){
          float wall_distance;
          unsigned int sideSensors[] = {module1[2], module4[2]};  // Initialize the array with side sensors
          for (unsigned int i = 0; i < sizeof(sideSensors)/sizeof(sideSensors[0]); i++) {
            if (sideSensors[i] < wall_distance) {  // Check for side walls
              wall = 1;  // Set wall to 1 near wall
              break;  // Exit the loop once the condition is met
            }
          }

          // change shape

        }
        if (colour == 3){
          float wall_distance;
          unsigned int sideSensors[] = {module1[2], module4[2]};  // Initialize the array with side sensors
          for (unsigned int i = 0; i < sizeof(sideSensors)/sizeof(sideSensors[0]); i++) {
            if (sideSensors[i] < wall_distance) {  // Check for side walls
              wall = 1;  // Set wall to 1 near wall
              break;  // Exit the loop once the condition is met
            }
          }

          // change shape
        }
      }

    }


  }

// String last_action = "";
// bool I_shape_done = false;
// bool O_shape_done = false;
// bool red_color_done = false;
// bool yellow_color_done = false;
// Husky color_detected status written as variable
//     if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
//   else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
//   else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
//   else
//   {
//       Serial.println(F("###########"));
//       while (huskylens.available())
//       {
//           HUSKYLENSResult result = huskylens.read();
// //          color_signature=result.ID;
// //          command_block = result.command;
//           if (result.ID == 1 && result.command == COMMAND_RETURN_BLOCK && my_robot.sm_getShape() != 'i') //If the learned face is detected and recognized as ID 1
//           {
//               Serial.println("This is green colour"); //Display in serial monitor
//               my_robot.sm_reset_M1();
//               my_robot.sm_reset_M2();
//               my_robot.sm_reset_M3();
//               my_robot.sm_reset_M4();
//               my_robot.I();
//               front_sensor_status = my_robot.module1_sensor_status(0);
//               right_sensor_status = my_robot.module1_sensor_status(4);
//               rear_sensor_status = my_robot.module3_sensor_status(0);
//               left_sensor_status = my_robot.module1_sensor_status(10);
// //                my_robot.O(); //If the learned face is detected and recognized, Smorphi will reconfigure to 'O' shape.
//           } 
//           else if (result.ID == 2 && result.command == COMMAND_RETURN_BLOCK && my_robot.sm_getShape() != 'o') //If the learned face is detected and recognized as ID 1
//           {
//               Serial.println("This is red colour"); //Display in serial monitor
//               my_robot.sm_reset_M1();
//               my_robot.sm_reset_M2();
//               my_robot.sm_reset_M3();
//               my_robot.sm_reset_M4();
//               my_robot.O();
//               front_sensor_status = my_robot.module1_sensor_status(0);
//               right_sensor_status = my_robot.module1_sensor_status(4);
//               rear_sensor_status = my_robot.module3_sensor_status(6);
//               left_sensor_status = my_robot.module4_sensor_status(0);
// //                my_robot.O(); //If the learned face is detected and recognized, Smorphi will reconfigure to 'O' shape.
//           }
//           else  
//           {
//               Serial.println("There is no colour present"); //Display in serial monitor
// //                my_robot.O(); //If the learned face is detected and recognized, Smorphi will reconfigure to 'O' shape.
//           } 
// //          break;
 
//       }    
//   }
//  if (front_sensor_status==0 && red_color_done != "True" && color_signature == 1 && command_block == COMMAND_RETURN_BLOCK){ // 1 is red color
//    if (I_shape_done != "True" && my_robot.sm_getShape() != 'i'){
//      my_robot.sm_reset_M1();
//      my_robot.sm_reset_M2();
//      my_robot.sm_reset_M3();
//      my_robot.sm_reset_M4();
//      my_robot.I();
//      front_sensor_status = my_robot.module1_sensor_status(0);
//      right_sensor_status = my_robot.module1_sensor_status(4);
//      rear_sensor_status = my_robot.module3_sensor_status(0);
//      left_sensor_status = my_robot.module1_sensor_status(10);
//      I_shape_done = "True";
//      red_color_done = "True";
//      
//    }
//  }
//  if (front_sensor_status==0 && yellow_color_done != "True" && color_signature == 2 && command_block == COMMAND_RETURN_BLOCK){ // 2 is yellow color
//    if (O_shape_done != "True" && my_robot.sm_getShape() != 'o'){
//      my_robot.sm_reset_M1();
//      my_robot.sm_reset_M2();
//      my_robot.sm_reset_M3();
//      my_robot.sm_reset_M4();
//      my_robot.O();
//      front_sensor_status = my_robot.module1_sensor_status(0);
//      right_sensor_status = my_robot.module1_sensor_status(4);
//      rear_sensor_status = my_robot.module3_sensor_status(6);
//      left_sensor_status = my_robot.module4_sensor_status(0);
//      O_shape_done = "True";
//      yellow_color_done = "True";
//      
//    }
//  }
  
// These conditions never execute as robot always falls under else condition    
//  my_robot.stopSmorphi();
//  my_robot.sm_reset_M1();
//  my_robot.sm_reset_M2();
//  my_robot.sm_reset_M3();
//  my_robot.sm_reset_M4();
 


// robotSides = [
//   [0, 0, 0, 0], [0, 0, 0, 0]. [0, 0, 0, 0], [0, 0, 0, 0]
// ]

// sideO = [
//   [1,2], // indexes of the sensors in robotSides 
//   [0,2], // indexes of the sensors in robotSide
//   [3], // indexes of the sensors in robotSide
//   [1]// indexes of the sensors in robotSide
// ]

// sideI = ...


// get sensor readings and store them in robotSides
// based on the shape we use the sideX array to pull the data from robotSides




// hy psudo code cause he cant c++
/*

float walltrace(int front, int right, int back, int left, int huskylenseval){ // i cbb to add the rest of the yeah

double kp speedL speedR speed treshold; // i forgot some stuff ltr yall declare

  if (front = 1,right = 1); 

  if (right = 1,back = 1);

  if (back = 1,left = 1);

  if (left = 1,front = 1){ //copy paste to the others
    while (frontultrasonic > 5mm){ //while true might be better idk

      walltracing(leftultrasonic);

      run motor at speedL and speedR

      if (left ultrasonic < 5mm && front ultrasonic < 5mm) || (left ultrasonic > 5mm && huskylenseval = 1) || (huskylenseval = 1 && front ultrasonic > 5mm){
        return;
        break;
      }
    }
  }
}

int walltracing(ultrasonicoutput){
treshold = 5
kp = 1
speed = 100
  speedL = speed - kp * (ultrasonicoutput -  treshold); //should = 0 when 5mm away from wall
  speedR = speed + kp * (ultrasonicoutput -  treshold);

  return speedL, speedR;
}

void changeshape (char botshape){
  yes idk get char input at if char = ... change shape to ...
}

void clearnroom(){
  while true{

    int i = 0 x = 0 y = 0;

    while (i < 1){

      while (rightultrasonic > 5mm){
        move right or walltracing(rightultrasonic);
      }
      while (leftultrasonic > 5mm){
        move left or walltracing(leftultrasonic);
      }
      i++;
    }

    
    x = leftultrasonic;
    y = rightultrasonic;

    while (leftultrasonic > 5mm){
      move left or walltracing(leftultrasonic);

      if (leftultrasonic > x){
        x = leftultrasonic;
      }
      if (frontultrasonic > y){
        y = rightultrasonic;
      }
    }

    move forward  set distance 


    // rightwards missing wall

    while (rightultrasonic > 5mm){
      move right or walltracing(rightultrasonic);
      if (leftultrasonic > x){
        x = leftultrasonic;
      }
      if (rigjtultrasonic > y){
        y = rightultrasonic;
      }
    }

    move forward  set distance 

    if (leftultrasonic < x +- 5mm) or smt{
    missingx = x - leftultrasonic;
    }

    if (rightultrasonic <  x +- 5mm) or smt{
    missingx = x - rightultrasonic;
    }


    if  (leftultrasonic > missingx, missingx != 0){
      while (leftultrasonic > 5mm){
        move left or walltracing(leftultrasonic);
      }
      while (backultrasonic > 5mm){
        move back or walltracing(backultrasonic);
      }
      while (rightultrasonic < +-y){
        while (rightultrasonic > 5mm){
          move right or walltracing(rightultrasonic);
          if (leftultrasonic > x){
            x = leftultrasonic;
          }
          if (rigjtultrasonic > y){
            y = rightultrasonic;
          }
        }

        move forward  set distance

      while (rightultrasonic > 5mm){
        move right or walltracing(rightultrasonic);
        if (leftultrasonic > x){
          x = leftultrasonic;
        }
        if (rigjtultrasonic > y){
          y = rightultrasonic;
        }
      }

        if (frontultrasonic > 5mm){
          move forward  set distance 
        }      
      }

    }

    if (rightultrasonic > missingx, missingx != 0){
      while (rightultrasonic > 5mm){
        move right or walltracing(rightultrasonic);
      }
      while (backultrasonic > 5mm){
        move back or walltracing(backultrasonic);
      }
      while (leftultrasonic < +-y){
        while (leftultrasonic > 5mm){
          move left or walltracing(leftultrasonic);
          if (leftultrasonic > x){
            x = leftultrasonic;
          }
          if (rigjtultrasonic > y){
            y = rightultrasonic;
          }
        }

        move forward  set distance

      while (leftultrasonic > 5mm){
        move left or walltracing(leftultrasonic);
        if (leftultrasonic > x){
          x = leftultrasonic;
        }
        if (rigjtultrasonic > y){
          y = rightultrasonic;
        }
      }

        if (frontultrasonic > 5mm){
          move forward  set distance 
        }
      }
    }

    if (missingx !=0, (leftultrasonic|| rightultrasonic) && frontultrasonic) <5mm{ //for partitions at the top
      while(leftultrasonic > 5mm){
        move left or walltracing(leftultrasonic);
      }

      while (leftultrasonic < 7mm){
        move back;
      }
      move back abit of aditional amount
      missingx = 0;
    }


    // implement the rest of the missing y
    // to all movement, add if detect colour, store colour val


    if (missingx = 0, (leftultrasonic|| rightultrasonic) && frontultrasonic) <5mm && colour = 1{ 
      // hard code this???
      changeshape('I');
    }

    
  }
}

implement a fuction to detect colour at the start of the room, and call code sequence based on the colour detected(just incase kena reset)


*/


