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
  unsigned int module4[4];
};

UltrasonicSensors ultrasonicData;

void changeShape(int colourID, char currentShape);
String last_action = "";
bool I_shape_done = false;
bool O_shape_done = false;
bool red_color_done = false;
bool yellow_color_done = false;
// Husky color_detected status written as variable
int color_signature;
int command_block;


void setup() {
  // put your setup code here, to run once:
  Serial.println("workie?");
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
  Serial.println("Success?");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("workie?");
  my_robot.MoveForward(80);
  // sensor_initialisation();

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
      break
    }
  }
}

void changeShape(int colourID, char currentShape){
  int wall = 0;
  
  if (currentShape == 'I'){
    // Write code to check for left and right wall based on sensor info
    // if (){
    //   wall == 1;
    // }
  }
  if (currentShape == "O"){
    // Write code to check for left and right wall based on sensor info
    // if (){
    //   wall == 1;
    // }
  }
  if (currentShape == "L"){
    // Write code to check for left and right wall based on sensor info
    // if (){
    //   wall == 1;
    // }
  }
  if (colourID == 1 && wall){
    // change shape code
  }
  if (colourID == 2 && wall){
    // change shape code
  }
  if (colourID == 3 && wall){
    // change shape code
  }
}

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
 
}







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

}




*/


