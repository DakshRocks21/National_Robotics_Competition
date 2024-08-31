// sending code for m1, m2, m3

#include <Wire.h>

// const int trigPin[] = {6,8,10,12};  // Trigger pin
// const int echoPin[] = {5,7,9,11}; // Echo pin
const int trigPin[] = {6,8,10,12};  // Trigger pin
const int echoPin[] = {5,7,9,11}; // Echo pin

uint16_t ultrasonicData[5] = {0};

void setup() {
  Wire.begin(); // Join the I2C bus as a master
  Serial.begin(9600); // Start the serial communication 
    
  for (int i = 0; i < 4; i++) {
    pinMode(trigPin[i], OUTPUT);
    pinMode(echoPin[i], INPUT);
  }
  
  Serial.println(" ready.");
}

void loop() {
  // get ultrasonic data
  for (int i = 0; i < 4; i++) {
    ultrasonicData[i] = readUltrasonic(trigPin[i], echoPin[i]);
  }

  // send data to s1
  Wire.beginTransmission(8); // Transmit to the receiver Arduino with address 8
  Wire.write("2"); // Send the data with an identifier
  for (int i = 0; i < sizeof(ultrasonicData)/sizeof(uint16_t); i++) {
    Wire.write((ultrasonicData[i]  >> 8) & 0xFF); // Send high byte
    Wire.write(ultrasonicData[i] & 0xFF); // Send low byte
    Serial.println("im here");
  }
  Wire.endTransmission(); // Stop transmitting

  // print ultrasonic data to Serial Monitor
  Serial.print("Ultrasonic Data: ");
  for (int i = 0; i < 5; i++) {
    Serial.print(ultrasonicData[i]);
    if (i < sizeof(ultrasonicData)/sizeof(uint16_t) - 1) {
      Serial.print(", ");
    }
  }
  Serial.println();
}

uint16_t readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  unsigned long duration = pulseIn(echoPin, HIGH);
  
  uint16_t distance = duration * 0.343 / 2;
  
  return distance;
}
