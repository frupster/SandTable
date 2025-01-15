#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
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
LiquidCrystal_I2C lcd(0x27, 16, 2); 
int column = 0;
String input = "";  // Store input from the keypad


void setup() {
  Serial.begin(9600);  // For debugging
  Wire.begin();        // Initialize I2C as master
  pinMode(LED_BUILTIN, OUTPUT);
  lcd.init(); 
  lcd.backlight();
  lcd.print("Input Command:");
  lcd.setCursor(0, 1);    


}


void loop() {
  char key = myKeypad.getKey();  // Get keypress from keypad
/*
    if(column == 5) {       
      lcd.clear();
      column = 0;
    }
*/
  if (key) {
    Serial.println(key);  // Print key to Serial Monitor for debugging
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);

    
    if (key == '#') {
      // When "#" is pressed, send input to slave and reset input
      sendToSlave(input);
      input = "";  // Clear input after sending
       lcd.clear();
       lcd.print("Input Command:");
       lcd.setCursor(0, 1);
    }
    
    else {
      input += key;  // Add key to input string
    }

    lcd.clear();
    lcd.print("Input Command:");
    lcd.setCursor(0, 1);
    lcd.print(input);

    if(input == ""){
      lcd.clear();
      lcd.print("Enter Command");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }

    if(input == "1"){
      lcd.clear();
      lcd.print("Grow Triangle");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
    else if(input == "2"){
      lcd.clear();
      lcd.print("Grow Square");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }

    else if(input == "3"){
      lcd.clear();
      lcd.print("Grow Polygon");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
    else if(input == "4"){
      lcd.clear();
      lcd.print("Rotate Triangle A/B/C/D");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
    else if(input == "4A"){
      lcd.clear();
      lcd.print("Rotate Triangle A");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
    else if(input == "4B"){
      lcd.clear();
      lcd.print("Rotate Triangle B");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
     else if(input == "4C"){
      lcd.clear();
      lcd.print("Rotate Triangle C");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
     else if(input == "4D"){
      lcd.clear();
      lcd.print("Rotate Triangle D");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
    else if(input == "5"){
      lcd.clear();
      lcd.print("Rotate Square A/B/C/D");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
    else if(input == "5A"){
      lcd.clear();
      lcd.print("Rotate Square A");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
    else if(input == "5B"){
      lcd.clear();
      lcd.print("Rotate Square B");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
    else if(input == "5C"){
      lcd.clear();
      lcd.print("Rotate Square C");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
    else if(input == "5D"){
      lcd.clear();
      lcd.print("Rotate Square D");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
    else if(input == "6"){
      lcd.clear();
      lcd.print("STEM");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
    else if(input == "7"){
      lcd.clear();
      lcd.print("UpDown");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
      else if(input == "8"){
      lcd.clear();
      lcd.print("LeftRight");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
    else if(input == "9"){
      lcd.clear();
      lcd.print("ZigZag");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
      else if(input == "0"){
      lcd.clear();
      lcd.print("Spiral");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
    

    else if (key == '*') {
      // When "*" is pressed, reset the input
      input = "";
    lcd.clear();
    lcd.print("Input Command:");
    lcd.setCursor(0, 1);
    }
    else if (key == '#') {
      input = "";  // Clear input after sending
       lcd.clear();
       lcd.print("Input Command:");
       lcd.setCursor(0, 1);
    }
    

    else{
      lcd.clear();
      lcd.print("Invalid Command");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
  }
}


void sendToSlave(String data) {
  Wire.beginTransmission(9);  // Address of the slave (I2C address 9)
  Wire.write(data.c_str());   // Send the input string
  Wire.endTransmission();     // End transmission
}
