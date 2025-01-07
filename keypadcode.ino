#include <Wire.h>
#include <Keypad.h>


const byte numRows = 4;  // Number of rows on the keypad
const byte numCols = 4;  // Number of columns on the keypad


// Keymap for the 4x4 keypad layout
char keymap[numRows][numCols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Pin assignments for the keypad
byte rowPins[numRows] = {9, 8, 7, 6};  // Rows 0 to 3
byte colPins[numCols] = {5, 4, 3, 2};  // Columns 0 to 3

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

String input = "";  // Store input from the keypad

void setup() {
  Serial.begin(9600);  // For debugging
  Wire.begin();        // Initialize I2C as master
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  char key = myKeypad.getKey();  // Get keypress from keypad

  if (key) {
    Serial.println(key);  // Print key to Serial Monitor for debugging
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    if (key == '#') {
      // When "#" is pressed, send input to slave and reset input
      sendToSlave(input);
      input = "";  // Clear input after sending
    }
    else if (key == '*') {
      // When "*" is pressed, reset the input
      input = "";
    }
    else {
      input += key;  // Add key to input string
    }
  }
}

void sendToSlave(String data) {
  Wire.beginTransmission(9);  // Address of the slave (I2C address 9)
  Wire.write(data.c_str());   // Send the input string
  Wire.endTransmission();     // End transmission
}