#include <ezButton.h>
#include <Wire.h>
int num = 1;
int colour = 0;  //leds
int button = 0;
int delayMin = 1;
bool homing = false;
int change = 0;
int power = 1;
int pause = 0;
//int right=0;
//int arrayLength = 0;
//float slope = 0;
float m = 0;
float distance = 1;


int dx = 10; // for bounce
int dy = 5;


int width = 165;
int height = 190;
int buttonState = 1;
int buttonState2 = 1;
//x , y


//math diameter 15mm
// ONe rotation = 1600 pulses
//int incomingByte = 0;
// defines pins
#define stepPin 2  //x
#define dirPin 5   //x
#define enPin 8    //
#define stepPin2 3  //y
#define dirPin2 6   //y
#define green A0  
#define blue A1  
#define red A2
#define ySwitch 10
#define xSwitch 9
int speed = 800;
float rotations = 15;  //15mm per rotations = 1600 pulses
float x = 0;
float y = 0;
//int x1, y1, x2, y2 = 0;
int ranNum = 0;
//in mm
double turns = (distance / rotations) * 1600;


#define SLAVE_ADDRESS 9  // I2C address for this slave

void setup() {  
  Serial.println("setup");
    Serial.begin(9600);


  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  // Sets the two pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(enPin, OUTPUT);
  pinMode(ySwitch, INPUT_PULLUP);
  pinMode(xSwitch, INPUT_PULLUP);
  digitalWrite(enPin, LOW);


  digitalWrite(green, HIGH);
  digitalWrite(red, HIGH);
  digitalWrite(blue, HIGH);

  Wire.begin(SLAVE_ADDRESS);  // Initialize I2C as slave with address 9
  Wire.onReceive(receiveEvent);  // Register a function to handle received data

  Serial.println("finished setup");

  //home();
}

void receiveEvent(int bytesReceived) {
  String receivedData = "";  // Variable to store the received data

  while (Wire.available()) {
    char c = Wire.read();  // Read one byte from the I2C bus
    receivedData += c;     // Append the received character to the string
  }

  Serial.print("Received: ");
  Serial.println(receivedData);  // Print the received data to the Serial Monitor
  home();
}



void loop(){
  delay(100);
}


void home() {
  Serial.println("home(reset)");
  homing = true;
  //x = width;
  //y = height;


  // button=0;


  buttonState = digitalRead(ySwitch);
  while (buttonState == 1) {
    buttonState = digitalRead(ySwitch);
    down(0.2);
  }
  while (buttonState2 == 1) {
    buttonState2 = digitalRead(xSwitch);
    left(0.2);
  }
  delay(500);
  while (buttonState2 != 1) {
    buttonState2 = digitalRead(xSwitch);
    right(0.2);
  }
  delay(500);
  while (buttonState != 1) {
    buttonState = digitalRead(ySwitch);
    up(0.2);
  }
  x = 0;
  y = 0;
  homing = false;
}




/// Basic Functions
void stepperMotor(float distance) {
  turns = (distance / rotations) * 1600;
  for (int i = 0; i < turns; i++) {


    digitalWrite(stepPin, HIGH);
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(speed);
    while (pause == 1) {
      Serial.println("pause");
    };




    //if (power==1){return;}
  }
}




void right(float distance) {


  if (button > 0) { return; }
  digitalWrite(dirPin, HIGH);   // Enables the motor to move right
  digitalWrite(dirPin2, HIGH);  // Enables the motor to move right
  stepperMotor(distance);
  x = x + distance;
}


void up(float distance) {


  if (button > 0) { return; }
  digitalWrite(dirPin, HIGH);  // Enables the motor to move right
  digitalWrite(dirPin2, LOW);  // Enables the motor to move right/HIGH left/LOW
  stepperMotor(distance);
  y = y + distance;
}


void down(float distance) {


  if (button > 0) { return; }
  digitalWrite(dirPin, LOW);    // Enables the motor to move right
  digitalWrite(dirPin2, HIGH);  // Enables the motor to move right/HIGH left/LOW
  stepperMotor(distance);
  y = y - distance;
}


void left(float distance) {


  if (button > 0) { return; }
  digitalWrite(dirPin, LOW);   //Changes the rotations direction left
  digitalWrite(dirPin2, LOW);  //Changes the rotations direction left
  stepperMotor(distance);
  x = x - distance;
}

