#include <Wire.h>

const int trigPin[] = {6,8,10,12};  // Trigger pin
const int echoPin[] = {5,7,9,11}; // Echo pin

const int slaveAddress = 8; // Address of the slave device
uint8_t receivedData[11]; // Buffer to store received data (1 byte for ID + 4 x 2 bytes for ultrasonic data)
uint16_t ultrasonicDataReceived[5];

typedef struct UltrasonicSensors {
  uint16_t module1[5];
  uint16_t module2[5];
  uint16_t module3[5];
  uint16_t module4[5];
} UltrasonicSensors;

UltrasonicSensors ultrasonicData;

void setup() {
  Wire.begin(slaveAddress); // Join I2C bus with the specified address
  Wire.onReceive(receiveEvent); // Register event handler for receiving data
  Wire.onRequest(requestEvent); // Register event handler for sending data
  Serial.begin(9600); // Start serial communication for debugging
  Serial.println("Slave Ready.");
}

void loop() {
  
  Serial.print("1:");
  for (int i = 0; i < 4; i++) {
    Serial.print(ultrasonicData.module1[i]);
    Serial.print(',');
  }
  Serial.print(ultrasonicData.module1[4]);
  Serial.println();
  Serial.print("2:");
  for (int i = 0; i < 4; i++) {
    Serial.print(ultrasonicData.module2[i]);
    Serial.print(',');
  }
  Serial.print(ultrasonicData.module2[4]);
  Serial.println();
  Serial.print("3:");
  for (int i = 0; i < 4; i++) {
    Serial.print(ultrasonicData.module3[i]);
    Serial.print(',');
  }
  Serial.print(ultrasonicData.module3[4]);
  Serial.println();
  Serial.print("4:");
  for (int i = 0; i < 4; i++) {
    Serial.print(ultrasonicData.module4[i]);
    Serial.print(',');
  }
  Serial.print(ultrasonicData.module4[4]);
  Serial.println();
  Serial.println();
  delay(100);
}

void receiveEvent(int howMany) {
  // Read all incoming data
  int index = 0;
  char id = 0;
  while (Wire.available()) {
    if (index < 11) {
      receivedData[index++] = Wire.read();
    }
  }

  // Process received data
  if (index >= 11) {
    // Extract the ID (first byte)
    id = receivedData[0];
    
    // Extract the ultrasonic data
    for (int i = 0; i < 5; i++) {
      ultrasonicDataReceived[i] = (receivedData[2 * i + 1] << 8) | receivedData[2 * i + 2];
    }

    // Print received data to Serial Monitor
    // Serial.print("Received ID: ");
    // Serial.println(id);
    // Serial.print("Ultrasonic Data: ");
    // for (int i = 0; i < 5; i++) {
    //   Serial.print(ultrasonicDataReceived[i]);
    //   if (i < 4) {
    //     Serial.print(", ");
    //   }
    // }
    // Serial.println();
  }

  if (id == '1') {
    for (int i = 0; i < 5; i++) {
      ultrasonicData.module1[i] = ultrasonicDataReceived[i];
    }
  }
  else if (id == '2')
  {
    for (int i = 0; i < 5; i++) {
      ultrasonicData.module2[i] = ultrasonicDataReceived[i];
    }
  }
  else if (id == '3')
  {
    for (int i = 0; i < 5; i++) {
      ultrasonicData.module3[i] = ultrasonicDataReceived[i];
    }
  }
  else if (id == '4')
  {
    for (int i = 0; i < 5; i++) {
      ultrasonicData.module4[i] = ultrasonicDataReceived[i];
    }
  }
}

void requestEvent() {
  // This function is called when the master requests data from the slave
  // You can use this to send data back to the master if needed
}

// uint16_t readUltrasonic(int trigPin, int echoPin) {
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);
  
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);
  
//   unsigned long duration = pulseIn(echoPin, HIGH);
  
//   uint16_t distance = duration * 0.343 / 2;
  
//   return distance;
// }