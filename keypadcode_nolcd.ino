

#include <Wire.h>
#include <Keypad.h>


const byte numRows = 4; //number of rows on the keypad
const byte numCols = 4; //number of columns on the keypad

//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

String room = "";

//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {9, 8, 7, 6}; //Rows 0 to 3
byte colPins[numCols] = {5, 4, 3, 2}; //Columns 0 to 3

//initializes an instance of the Keypad class
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup()
{
  Serial.begin(9600);
  Wire.begin(); 
  pinMode(LED_BUILTIN, OUTPUT);



}

//If key is pressed, this key is stored in 'keypressed' variable
//If key is not equal to 'NO_KEY', then this key is printed out
//if count=17, then count is reset back to 0 (this means no key is pressed during the whole keypad scan process
void loop()
{
  char pressedKey = myKeypad.getKey();
  switch (pressedKey) {

    case '#':
      
      if( room.length()== 1 && room == "A" || "B" || "C" || "D"){
        room+= "0";
      }
      Serial.println("case #");
      transmit();
      room = "";
      break;
    case '*':
      Serial.println("Reset!");
      
      delay(1000);
     
      room = "";
      break;

    default:
      if (pressedKey == NO_KEY) break;
      digitalWrite(LED_BUILTIN, HIGH);
	    delay(200);
  	  digitalWrite(LED_BUILTIN, LOW);

      if (room.length() >= 4  && room.length() > 1) break;
      //if (room.length() == 0 && !(pressedKey == ('A') || pressedKey == ('B') || pressedKey == ('C') || pressedKey == ('D') )) break;
      //if (room.length() >= 1 && (pressedKey == ('A') || pressedKey == ('B') || pressedKey == ('C') || pressedKey == ('D') )) break;

      room += (pressedKey);
      Serial.println(room);

      break;
  }

}

void transmit(){

  int x = room.length();
   // transmit to device #9
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write('R');
  Wire.endTransmission();
  for(int y = 0; y<x; y++){
     Wire.beginTransmission(9); 
  Wire.write(room[y]);
    Wire.endTransmission();
  }


  
}
