#include <SoftwareSerial.h>

// Define RX and TX pins for SoftwareSerial
#define RX_PIN 14
#define TX_PIN 15

typedef struct UltrasonicSensors {
  uint16_t module1[5];
  uint16_t module2[5];
  uint16_t module3[5];
  uint16_t module4[5];
} UltrasonicSensors;

UltrasonicSensors ultrasonicData;

// Create a SoftwareSerial object
SoftwareSerial mySerial(RX_PIN, TX_PIN);

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(115200); 
  
  // Initialize SoftwareSerial at 9600 baud rate
  mySerial.begin(9600); 
}

void loop() {
  if (mySerial.available()) {
    // Read incoming data until a newline character is found
    String receivedData = mySerial.readStringUntil('\n');

    // Ensure the data is not empty
    if (receivedData.length() > 0) {
      Serial.println("Received from Arduino: " + receivedData);

      // Parse the received string and cast into the struct
      parseAndStoreData(receivedData);
    }
  }
}

void parseAndStoreData(String data) {
  // Find the position of the colon to separate the module number from the data
  int colonPos = data.indexOf(':');
  if (colonPos == -1) {
    return; // Invalid data format, skip processing
  }

  // Extract module number
  int moduleNumber = data.substring(0, colonPos).toInt();

  // Split the remaining string into values based on commas
  int dataArray[5];
  int index = 0;
  int startPos = colonPos + 1;

  // Iterate through the string and extract integer values
  while (index < 5) {
    int commaPos = data.indexOf(',', startPos);
    if (commaPos == -1) {
      dataArray[index++] = data.substring(startPos).toInt();
      break;
    } else {
      dataArray[index++] = data.substring(startPos, commaPos).toInt();
      startPos = commaPos + 1;
    }
  }

  // Store the parsed data in the correct module array
  switch (moduleNumber) {
    case 1:
      memcpy(ultrasonicData.module1, dataArray, sizeof(dataArray));
      break;
    case 2:
      memcpy(ultrasonicData.module2, dataArray, sizeof(dataArray));
      break;
    case 3:
      memcpy(ultrasonicData.module3, dataArray, sizeof(dataArray));
      break;
    case 4:
      memcpy(ultrasonicData.module4, dataArray, sizeof(dataArray));
      break;
    default:
      Serial.println("Invalid module number: " + String(moduleNumber));
      return; // Invalid module number, skip processing
  }

  // Print out the stored data for debugging
  Serial.print("Module ");
  Serial.print(moduleNumber);
  Serial.print(": ");
  for (int i = 0; i < 5; i++) {
    Serial.print(dataArray[i]);
    if (i < 4) {
      Serial.print(", ");
    }
  }
  Serial.println();
}
