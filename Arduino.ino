/*
For the automatic aspect, this robot will drive around on its own and drive within a distance of any wall to its' right side.
The robot will turn left and adjust if it senses it is too close to the wall, and inversely, it will
turn right if it senses it is straying too far from the wall. It also contains a kill switch.

It also has a manual mode that will wireless receive commands through the wi-fi and execute them as they are received.
*/
#define goalDist 20.0           //sets the distance we want to be from the wall

//the left motor will be controlled by the motor A pins on the motor driver
const int AIN1 = 13;            //control pin 1 on the motor driver for the left motor
const int AIN2 = 12;            //control pin 2 on the motor driver for the left motor
const int PWMA = 11;            //speed control pin on the motor driver for the left motor

//the right motor will be controlled by the motor B pins on the motor driver
const int PWMB = 10;           //speed control pin on the motor driver for the right motor
const int BIN2 = 9;            //control pin 2 on the motor driver for the right motor
const int BIN1 = 8;            //control pin 1 on the motor driver for the right motor


//distance variables
const int trigPin = 6;        //sets the pin that outputs the ultrasonic wave
const int echoPin = 5;        //sets the pin that reads the inputs of the rebounding ultrasonic wave

int switchPin = 7;             //switch to turn the robot on and off
int distanceError = 0;         //initializes the distance error, and sets it to zero
float distance = 0;            //variable to store the distance measured by the distance sensor
boolean wallfollow = false;    //boolean variable that can be controlled to set the robot in and out of the automatic state
String inputType;              //initializes a string for input
/********************************************************************************/
void setup()
{
  pinMode(trigPin, OUTPUT);       //this pin will send ultrasonic pulses out from the distance sensor
  pinMode(echoPin, INPUT);        //this pin will sense when the pulses reflect back to the distance sensor
  pinMode(switchPin, INPUT_PULLUP); //set as 'pullup' to sense whether the switch is on or not

  //set the motor control pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  Serial.begin(115200);               //begin serial communication with the computer
  Serial.print("Is this thing on?");  //test the serial connection
}

/********************************************************************************/
void loop()
{
  String input = readESPin();

  if (input!=""){Serial.print('n');Serial.print(input);Serial.print('n');}

  if (input.charAt(0)=='a') {wallfollow=true;}                    //if the input is read in as 'a', the robot will be set so that it can automatically follow walls
  else if (input.charAt(0)=='m') {wallfollow=false;}              //if the input is read in as 'm', the robot will be set so that it will move only on manual commands
  else if (input.charAt(0)=='f'&&!wallfollow) {motorForward();}   //if the input is read in as 'f' and is set in manual mode, the robot will call a function to move forward
  else if (input.charAt(0)=='r'&&!wallfollow) {motorRight();}     //if the input is read in as 'r' and is set in manual mode, the robot will call a function to move to the right
  else if (input.charAt(0)=='b'&&!wallfollow) {motorBack();}      //if the input is read in as 'b' and is set in manual mode, the robot will call a function to move backwards
  else if (input.charAt(0)=='l'&&!wallfollow) {motorLeft();}      //if the input is read in as 'l' and is set in manual mode, the robot will call a function to move left

 if (wallfollow){
  if(digitalRead(switchPin) == LOW){  //if the switch is on, allow driving

    distance = getDistance();                     //distance is read in from the ultrasonic sensor and stored in a variable
    distanceError = distance - goalDist;          //the error in distance is set to the difference in actual distance and the goal
    float driveDuration=5*abs(distanceError);     //drive duration is set in accordance with the distance error
    if (driveDuration>1000){driveDuration = 1000;}//unless it is over 1 second, then it caps at 1 second

  Serial.print("Distance: ");
  Serial.print(distance);

    if(distanceError < 5){                    //if the wall is detected as being too close
      rightMotor(255);                        //turn the right wheel forwards
      leftMotor(-255);                        //turn the left wheel backwards
      delay(driveDuration*3);                 //for a duration proportional to the calculated value above
      rightMotor(255);                      
      leftMotor(255);                         //now go straight
      delay(driveDuration*2);                 //for a duration proportional to the calculated value above
      stop();
      delay(500);
    }

    else if (distanceError > -5){               //if the robot is too far from the wall
      rightMotor(-255);                         //turn the right wheel backwards
      leftMotor(255);  //turn the robot right   //turn the left wheel forwards
      delay(driveDuration*5);                   //for a duration proportional to the calculated value above
      rightMotor(255);
      leftMotor(255);                           //now go straight
      delay(driveDuration*2);                   //for a duration proportional to the calculated value above
      stop();
      delay(500);
    }
    
    else{                         //robot is within a desired range from wall
      rightMotor(255;
      leftMotor(255);             //move forward             
      delay(2000);
      }
    }
  else{delay(1000);}
  }
}
/********************************************************************************/
void leftMotor(int motorSpeed)                       //function for driving the left motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(AIN1, HIGH);                         //set pin 1 to high
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMA, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}

/********************************************************************************/
void rightMotor(int motorSpeed)                        //function for driving the right motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(BIN1, HIGH);                         //set pin 1 to high
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMB, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}

/********************************************************************************/
//RETURNS THE DISTANCE MEASURED BY THE HC-SR04 DISTANCE SENSOR
float getDistance()
{
  float echoTime;                   //stores the time it takes for a ping to bounce off an object
  float calculatedDistance;         //stores the distance calculated from the echo time

  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);

  echoTime = pulseIn(echoPin, HIGH);      //use the pulsein command to see how long it takes for the
                                          //pulse to bounce back to the sensor

  calculatedDistance = echoTime / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)

  return calculatedDistance;              //send back the distance that was calculated
}

void stop(){
rightMotor(0);
leftMotor(0);     //halts the motors
}

// this handles commands received via the specified Serial Input
String readESPin(){
  
   if (Serial.available() > 0){           
   
        inputType = Serial.readString();    //read the input and store it into the inputType string
        return inputType;                   //return the string
    }
    return "";
}

//go straight for one second
void motorForward(){
      rightMotor(255);  
      leftMotor(255);
      delay(1000);
      stop(); 
}

//go backwards for one second
void motorBack(){
      rightMotor(-255);
      leftMotor(-255);
      delay(1000);
      stop(); 
}

//go right for half a second, then go forward for one second
void motorRight(){
      rightMotor(-255);
      leftMotor(255);
      delay(500);
      motorForward();
      stop(); 
}

//go left for half a second, then go forward for one second
void motorLeft(){
      rightMotor(255);
      leftMotor(-255);
      delay(1000);
      motorForward();
      stop(); 
}
