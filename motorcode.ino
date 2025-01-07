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
float defaultDistance = 0.2;


int dx = 10; // for bounce
int dy = 5;


int width = 21;
int height = 23;
int buttonStateX = 1;
int buttonStateY = 1;
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
#define xSwitch 9
#define ySwitch 10
int speed = 1600;
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
pinMode(LED_BUILTIN, OUTPUT);

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

  //Serial.print("Received: ");
  Serial.println(receivedData); 
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  interpretInput(receivedData); // Print the received data to the Serial Monitor
  //home();

}
void interpretInput(String input){
  
 

  if(input == "0"){ 
    down(5);
  }
  if(input == "5"){
    up(5);
  }
  if(input == "7"){
    left(5);
  }
  if(input == "9"){
    right(5);
  }

  if(input == "2"){
    circle(5);
  }

  if(input == "C"){
    plot(10, 10, 15, 15);
  }
  if(input == "D"){
    plot(10, 10, 5, 15);
  }

  if(input == "1"){
    up(1);
    delay(100000);
    down(1);
    delay(100000);
    left(1);
    delay(100000);
    right(1);
  }

  if(input == "4"){
    plot(x, y, 0, height);
  }
 
  if(input == "8"){
    home();
  }

  if(input == "A"){
    leftRight();
  }
  if(input == "B"){
    String xStr = String(x);
    String yStr = String(y);
    Serial.println("(" + xStr + ", " + yStr + ")");
  }

}


void home(){
  buttonStateY = digitalRead(ySwitch);
  digitalWrite(dirPin, LOW);    
  digitalWrite(dirPin2, HIGH);  //moves down until limit switch is triggered
  while(buttonStateY == 1){

    digitalWrite(stepPin, HIGH);
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(speed);
    buttonStateY = digitalRead(ySwitch);
  }
  buttonStateX = digitalRead(xSwitch);

  digitalWrite(dirPin, HIGH);   
  digitalWrite(dirPin2, HIGH);  //moves right until limit switch is triggered
  
  while(buttonStateX == 1){
  digitalWrite(stepPin, HIGH);
  digitalWrite(stepPin2, HIGH);
  delayMicroseconds(speed);
  digitalWrite(stepPin, LOW);
  digitalWrite(stepPin2, LOW);
  delayMicroseconds(speed);
  buttonStateX = digitalRead(xSwitch);
  }
  x=21;
  y=0;

}

void loop(){
  /*
   if(digitalRead(ySwitch) == 0){
    Serial.println("ySwitch");
   }
   if(digitalRead(xSwitch) == 0){
    Serial.println("xSwitch");
   }
   */
}

void leftRight()  //clear
{
  home(); // (21, 0)
  float originalValue = speed;
  speed = 800;
  for(int i = 0; i < height; i++){
    if(x < 5){
      plot(x, y, width, i);
    }
    else if(x == width){
      plot(x, y, 0, i);
    }
    up(1);
  }
  speed = originalValue;
}
void circle(float r) {
  // Start at the midpoint and go up by the radius
  float angleStep = 1; // Increment in degrees for plotting points (controls resolution)
  float xOrigin = x;
  float yOrigin = y;
  // Loop to draw points around the circle
  for (float angle = 0; angle < 360; angle += angleStep) {
    // Convert angle to radians
    float angleRad = angle * (PI / 180);
    
    // Parametric equations for circle
    float x2 = (r * cos(angleRad)) + xOrigin;  // X coordinate
    float y2 = (r * sin(angleRad)) + yOrigin;  // Y coordinate

    plot(x, y, x2, y2);
  }
}


void circle1(float r ){
  //starts at midpoint, goes up by radius
  up(r);
  for(float i = 0; i < r/2; i++){
    float x2 = sqrt((pow(r,2) - pow(y, 2)));
    Serial.println(x2);
    float y2 = sqrt((pow(r,2) - pow(y, 2)));
    Serial.println(y2);
    plot(x, y, x2, y2);
  }
}

///////////Main plot function between two points
void plot(int x1, int y1, int x2, int y2) {
  //if (power==1){home(); return;}

  if (button > 0) { return; }
  m = (float(y2 - y1) / float(x2 - x1))*defaultDistance;  // y/x  slope

  Serial.print("dy=");
  Serial.print((y2 - y1));
  Serial.print(" dx=");
  Serial.print((x2 - x1));

  Serial.print(" slope=");
  Serial.print(m);
  Serial.print(" loc=");

  Serial.print(x);
  Serial.print(",");
  Serial.print(y);

  Serial.print(" num=");
  Serial.print(num);
  Serial.print(" pow=");
  Serial.print(power);
  Serial.print(" button=");
  Serial.print(button);
  Serial.print(" ranNum=");
  Serial.println(ranNum);
  //Serial.println(button);

  //Serial.print("slope=");


  while (y + (y2 - y1) > height + 3 || x + (x2 - x1) > width + 2 || x + (x2 - x1) < -2 || y + (y2 - y1) < -2) {
    Serial.print("outsideRange");
  }
  if ((x2 - x1 == 0))  // dx=0 up or down.
  {
    for (int i = 0; i < abs(y2 - y1); i++)  //x  one// unit in x dir.
    {
      //    if (power==1){break;}
      distance = 1;  //in mm
      //turns = (distance / rotations) * 1600;

      if ((y2 - y1) > 0) {
        up(distance);
      }

      if (y2 - y1 < 0) {
        down(distance);
      }
    }
  }
  // go on angles
  else {
    if (x2 - x1 > 0)  // going right
    {
      for (float i = 0; i < abs(x2 - x1); i+= defaultDistance)  //x  one unit in x dir.
      {
        distance = defaultDistance;  //in mm
                       // turns = (distance / rotations) * 1600;
        right(distance);
        distance = abs(m);  //in mm in y dir.
                            // turns = (distance / rotations) * 1600;
        if (m > 0) {
          up(distance);
        }

        if (m < 0) {
          down(distance);
        }
      }
    }

    if (x2 - x1 < 0)  // going left
    {
      for (float i = 0; i < abs(x2 - x1); i+=defaultDistance)  //x  one unit in x dir.
      {
        distance = defaultDistance;  //in mm
                       //   turns = (distance / rotations) * 1600;
        left(distance);
        distance = abs(m);  //in mm in y dir.
                            //   turns = (distance / rotations) * 1600;
        if (m > 0) {
          down(distance);
        }

        if (m < 0) {
          up(distance);
        }
      }
    }
  }
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
      //Serial.println("pause");
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
