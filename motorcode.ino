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
  
 if(input == "1"){
  home();
  growSquares();
 }
  if(input == "2"){
    home();
    growTriangles(width/2 - 2, height/2 + 2, width/2 + 2, height/2 + 2);
  }
  if(input == "3"){
    home();
    growCircles();
  }

  if(input == "4"){
    home();
    rotateTriangles(6, 6);
  }
  if(input == "4A"){
    home();
    rotateTriangles(3, 3);
  }
  if(input == "4B"){
    home();
    rotateTriangles(5, 3);
  }
   if(input == "4C"){
    home();
    rotateTriangles(7, 4);
  }
  if(input == "4D"){
    home();
    rotateTriangles(8, 8);
  }

  if(input == "5"){
    home();
    rotateSquares(5);
  }
  if(input == "5A"){
    home();
    rotateSquares(2);
  }
  if(input == "5B"){
    home();
    rotateSquares(3);
  }
  if(input == "5C"){
    home();
    rotateSquares(6);
  }
  if(input == "5D"){
    home();
    rotateSquares(7);
  }
  
  if(input == "6"){
    stem();
  }
  
  if(input == "7"){
    upDown();
  }
  if(input == "8"){
    leftRight();
  }
  if(input == "9"){
    zigZag();
    zagZig();
  }
  if(input == "0"){
    home();
    spiral();
  }

}


void home(){
  left(1);
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

void leftRight()  //clear function
{
  home(); // (21, 0)
  float originalValue = speed;
  //speed = 800;
  for(int i = 0; i < height; i++){
    if(x < 5){
      plot(x, y, width, i);
    }
    else if(x == width){
      plot(x, y, 0, i);
    }
    up(1);
  }
  //speed = originalValue;
}

void upDown(){ //clear function
  home();
  float originalValue = speed;
  //speed = 800;
  for(int i = width; i > 0; i--){
    if(y < 5){
      plot(x, y, i, height);
    }
    else if(y > 5){
      plot(x, y, i, 0);
    }
    left(1);
  }
  speed = originalValue;
}

void zigZag(){ //clear function
  home();
  left(1);
  float offset = 2;
  float step = 2;
  plot(x, y, width - 1, height - offset); // -1 is to prevent crashing into wall
  while(width - offset > 0){
    plot(x, y, width - 1 - offset, height - 1);
    left(step);
    offset += step;
    plot(x, y, width -1 , height - 1 - offset);
    down(step);
    offset += step;
  }
}
void zagZig(){
  left(1);
  home();
  up(1);
  float offset = 2;
  float step = 2;
  plot(x, y, 0 + offset, 1);
  while(offset < width){
    plot(x, y, 1, offset);
    up(step);
    offset += step;
    plot(x, y, offset, 1);
    right(step);
    offset += step;
  }
}

void spiral(){  // r = k * theta
  center();
  float r = 0;
  float angleRad = 0;
  float angleStep = PI/64;
  float xOrigin = x;
  float yOrigin = y;
  bool inBounds = true;
  float x2 = 0;
  float y2 = 0;

  while(x2 + xOrigin < width - 1 && y2 +yOrigin < height - 1 && (x2+xOrigin) * (y2 + yOrigin) + 1 > 0){
    x2 = (r * cos(angleRad));
    y2 = (r * sin(angleRad));

    if(x2 + xOrigin < width - 1 && y2 +yOrigin < height - 1 && (x2+xOrigin) * (y2 + yOrigin) + 1 > 0){
      plot(x, y, x2 + xOrigin, y2 + yOrigin);
    }
    angleRad += angleStep;
    r = 0.5 * angleRad;
  }
}

void circle(float r) {
  center();
  // Start at the midpoint 
  float angleStep = 10; // Increment in degrees for plotting points (controls resolution)
  float xOrigin = x;
  float yOrigin = y;
  // Loop to draw points around the circle
  for (float angle = 0; angle <= 360; angle += angleStep) {
    // Convert angle to radians
    float angleRad = angle * (PI / 180);
    
    // Parametric equations for circle
    float x2 = (r * cos(angleRad));  // X coordinate
    float y2 = (r * sin(angleRad));  // Y coordinate

    Serial.println("");
    Serial.print(x2);
    Serial.print(", ");
    Serial.print(y2);
    plot(x, y, x2 + xOrigin, y2 + yOrigin);
  }
}

void growCircles(){
  center();
  float r = 2;
  while(width/2 + r < width && height/2 + r < height){
    circle(r);
    r += 2;
  }
}

void rotateTriangles(float x1, float y1){
  center();
  float xOrigin = x;
  float yOrigin = y;

  //(x1, y1) is first point
  //(x2, y2) is second point

  float x2 = -x1;
  float y2 = y1;

  //calculate radius
  float angle1 = atan(y1/x1);
  float r = abs(y1/(sin(angle1)));

//initial triangle
  plot(x, y, x1 + xOrigin, y1 + yOrigin);
  plot(x, y, x2 + xOrigin, y2 + yOrigin);
  plot(x, y, xOrigin, yOrigin);

  float angleStep = PI/6;
  for(float i = 0; i < 11*PI/6; i += angleStep){
    //first point
    float angle1 = atan(y1/x1);
    
    if(x1 < 0) angle1 += PI; //put in correct quadrant
    //calculate next point
    x1 = r * cos(angle1 + angleStep);
    y1 = r * sin(angle1 + angleStep);
    plot(x, y, x1 + xOrigin, y1 + yOrigin);
    Serial.println("");
    Serial.println("");
    Serial.print(x1);
    Serial.print(", ");
    Serial.print(y1);

    //second point
    float angle2 = atan(y2/x2);
    if (x2 < 0) angle2 += PI;
    x2 = r * cos(angle2 + angleStep);
    y2 = r * sin(angle2 + angleStep);
    plot(x, y, x2 + xOrigin, y2 + yOrigin);
    Serial.println("");
    Serial.print(x2);
    Serial.print(", ");
    Serial.print(y2);

    //return  to center
    plot(x, y, xOrigin, yOrigin);
  }
}

void rotateSquares(float s){
  center();
  float xOrigin = x;
  float yOrigin = y;

  float x1 = -s;
  float y1 = s;

  float x2 = s;
  float y2 = s;
  
  float x4 = -s;
  float y4 = -s;

  float x3 = s;
  float y3 = -s;

  //calculate radius
  float angle1 = atan(y1/x1);
  float r = abs(y1/(sin(angle1)));

  //initial square
  plot(x, y, x1 + xOrigin, y1 + yOrigin);
  plot(x, y, x2 + xOrigin, y2 + yOrigin);
  plot(x, y, x3 + xOrigin, y3 + yOrigin);
  plot(x, y, x4 + xOrigin, y4 + yOrigin);
  plot(x, y, x1 + xOrigin, y1 + yOrigin);

  
  float angleStep = PI/6;
  for(float i = 0; i < 5*PI/6; i += angleStep){
    //first point
    float angle1 = atan(y1/x1);
    
    if(x1 < 0) angle1 += PI; //put in correct quadrant
    //calculate next point
    x1 = r * cos(angle1 + angleStep);
    y1 = r * sin(angle1 + angleStep);
    plot(x, y, x1 + xOrigin, y1 + yOrigin);
    Serial.println("");
    Serial.println("");
    Serial.print(x1);
    Serial.print(", ");
    Serial.print(y1);

    //second point
    float angle2 = atan(y2/x2);
    if (x2 < 0) angle2 += PI;
    x2 = r * cos(angle2 + angleStep);
    y2 = r * sin(angle2 + angleStep);
    plot(x, y, x2 + xOrigin, y2 + yOrigin);
    Serial.println("");
    Serial.print(x2);
    Serial.print(", ");
    Serial.print(y2);

     //third point
    float angle3 = atan(y3/x3);
    if (x3 < 0) angle3 += PI;
    x3 = r * cos(angle3 + angleStep);
    y3 = r * sin(angle3 + angleStep);
    plot(x, y, x3 + xOrigin, y3 + yOrigin);
    Serial.println("");
    Serial.print(x3);
    Serial.print(", ");
    Serial.print(y3);
    
     //fourth point
    float angle4 = atan(y4/x4);
    if (x4 < 0) angle4 += PI;
    x4 = r * cos(angle4 + angleStep);
    y4 = r * sin(angle4 + angleStep);
    plot(x, y, x4 + xOrigin, y4 + yOrigin);
    Serial.println("");
    Serial.print(x4);
    Serial.print(", ");
    Serial.print(y4);

    //back to first
   // plot(x, y ,x1+xOrigin, y1 + yOrigin);
  }
}

void growTriangles(float x2, float y2, float x3, float y3){ 
  // arguments should be ACTUAL POINTS, within table range
  // (x2, y2) on the left, (x3, y3) on the right          
  center();
  float x1 = x;
  float y1 = y;
  float coordinates[] = {x1, y1, x2, y2, x3, y3};
 
  while(inRange(coordinates)){ // check if points are in range
    plot(x, y, coordinates[0], coordinates[1]);
    plot(x, y, coordinates[2], coordinates[3]);
    plot(x, y, coordinates[4], coordinates[5]);
    plot(x, y, coordinates[0], coordinates[1]);
    float growFactor = 2;
    coordinates[1] -= growFactor;
    coordinates[2] -= growFactor;
    coordinates[3] += growFactor;
    coordinates[4] += growFactor;
    coordinates[5] += growFactor;
  }
}

void growSquares(){
  center();
  float x1 = x-1;
  float y1 = y+1;
  float x2 = x+1;
  float y2 = y+1;
  float x3 = x+1;
  float y3 = y-1;
  float x4 = x-1;
  float y4 = y-1;
  float coordinates[8] = {x1, y1, x2, y2, x3, y3, x4, y4}; //first square
  float growFactor = 2;
  while(inRange(coordinates)){
    plot(x, y, coordinates[0], coordinates[1]);
    plot(x, y, coordinates[2], coordinates[3]);
    plot(x, y, coordinates[4], coordinates[5]);
    plot(x, y, coordinates[6], coordinates[7]);
    plot(x, y, coordinates[0], coordinates[1]);
    for(int i = 0; i < 8; i++){ // add to all coordinates, growing shape
      if(i % 2 ==0){ //x value
        if(coordinates[i] > width/2){ //check quadrant
          coordinates[i] += growFactor;
        }
        else coordinates[i] -= growFactor;
      }
      else { //y value
        if(coordinates[i] > height/2){ //check quadrant
          coordinates[i] += growFactor;
        }
        else coordinates[i] -= growFactor;
      }
    }
  }
}

bool inRange(float coordinates[]){ // check if coordinates are in range, even=x, odd=y
  int size = sizeof(coordinates) / sizeof(coordinates[0]);

  for(int i = 0; i < size; i++){ //check if inRange
    if (fmod(coordinates[i], 2) == 0){ // if x coordinate
      if(coordinates[i] >= width - 2 || coordinates[i] <= 1){
        return false;
      }
    }
    if (fmod(coordinates[i], 2) != 0){ // if y coordinate
      if(coordinates[i] >= height - 2 || coordinates[i] <= 1){
        return false;
      }
    }
  }
  return true;
}

void stem(){
  home();
  float coordinates[] = {
    1, 1,
    1, 9,
    5, 9,
    5, 12,
    1, 12,
    1, 15,
    8, 15,
    8, 9,
    8, 15,
    15, 15,
    12, 15,
    12, 12, 
    15, 12,
    12, 12,
    12, 9,
    16, 9,
    16, 15,
    19, 15,
    19, 9,
    19, 15,
    21, 15,
    21, 9,
  };

 int size = sizeof(coordinates) / sizeof(coordinates[0]);
 for(int i = 0; i < size; i+= 2){
  plot(x, y, coordinates[i], coordinates[i+1]);
 }
}

void center(){
  plot(x, y, width/2, height/2);
}

///////////Main plot function between two points
void plot(int x1, int y1, int x2, int y2) {
  //if (power==1){home(); return;}

  if (button > 0) { return; }
  m = (float(y2 - y1) / float(x2 - x1))*defaultDistance;  // y/x  slope
/*
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
*/

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
