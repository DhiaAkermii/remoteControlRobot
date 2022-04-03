// connect motor controller pins to Arduino digital pins
// ************************************************************************
/*


	   Remote control robot developed by Dhia Akermi 

*/
int enB = 6;
int enA = 5;
int in2 = 7;
int in1 = 8;
int in3=9;
int in4 = 10;
int LED = 2;
//**************************************************************************
#include<Servo.h>

//**********************capteur ultraasons**********************************
/* Constantes pour les broches */
const byte TRIGGER_PIN = 11; // Broche TRIGGER
const byte ECHO_PIN = 12;    // Broche ECHO
/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;
//**************************************************************************
String text,textAux;
Servo servo;
void setup()
{
  // etait initial du moteur servo
  servo.attach(2);
  servo.write(100);
  //****************************Wheels**************************************
  // Moteur 1
  pinMode(enA, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in1, OUTPUT);

  //Moteur 2
  pinMode(enA, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  analogWrite(enA, 70); // speed right wheels 
  analogWrite(enB, 70); // speed left wheels 
  //*************************************************************************
  //*******************************Capteur ultrasons**************************
   /* Initialise le port série */
  Serial.begin(9600);
  /* Initialise les broches */
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN, INPUT);
  //****************************************************************************

}
void loop()
{
while(Serial.available())
  {
    delay(10);
    char c = Serial.read();
    textAux += c;
  }
  if(textAux.length() > 0) text = textAux;
  if(text.length() > 0) // si il ya un ordre vocale 
  {
    if(text == "stop") Stop();
    else if( text == "start" || text == "go") DetectObstacles();
    else if (text == "drift") Drift();
    else if(text == "right"){ GoRight(); Stop(); }
    else if (text == "left"){ GoLeft(); Stop(); }
    else if ( text == "back" || text =="bac" || text =="turn back" || text=="turn bac") { GoBack(); Stop();}
    else if (text == "go back") GoBackward();
    if(text != "go" && text != "stat") text = "";
    textAux = "";
  }

}


void DetectObstacles(){
   float distance_cm = Look();
    if(distance_cm <= 25){
      Stop();
      float DL = LookLeft();
      float DR = LookRight();
      if(DL <= 25 && DR <= 25){
        GoBackward();
      }else if ( DR <= 15){
        GoLeft();
      }
      else 
        GoRight();
    }
    else{
        GoForward();
    }
}


//
void Stop(){
        analogWrite(in1,0);
        digitalWrite(in2, 0);
        // turn on motor B
        digitalWrite(in3,0);
        analogWrite(in4, 0);
        delay(500);
}
void GoForward(){
        //left wheels
        digitalWrite(in1,1);
        digitalWrite(in2, 0);
        //Right wheels
        digitalWrite(in3, 1); 
        digitalWrite(in4,0);
}

void GoBack(){ // turn 180°
  analogWrite(enA, 255); // speed right wheels 
  analogWrite(enB, 255); // speed left wheels 
        analogWrite(in1, 255);
        digitalWrite(in2, 0);
        // turn on motor B
        digitalWrite(in3,0);
        analogWrite(in4, 0);
        delay(600);
  analogWrite(enA, 70); // speed right wheels 
  analogWrite(enB, 70); // speed left wheels 
}

void GoRight(){ // turn 90° right
  GoBackward();
  analogWrite(enA, 255); // speed right wheels 
  analogWrite(enB, 255); // speed left wheels 
        analogWrite(in1, 255);
        digitalWrite(in2, 0);
        // turn on motor B
        digitalWrite(in3,0);
        analogWrite(in4, 0);
        delay(290);
  analogWrite(enA, 70); // speed right wheels 
  analogWrite(enB, 70); // speed left wheels 
}
void GoLeft(){   // turn 90° left
GoBackward(); // ywa5er chwaya
  analogWrite(enA, 255); // speed right wheels 
  analogWrite(enB, 255); // speed left wheels 
        analogWrite(in1,0);
        analogWrite(in2, 0);
        // turn on motor B
        analogWrite(in3,  255);
        analogWrite(in4, 0);
        delay(290);
    analogWrite(enA, 70); // speed right wheels 
  analogWrite(enB, 70); // speed left wheels 
}

void GoBackward(){ // ywa5er betwely
        analogWrite(in1,0);
        analogWrite(in2, 255);
        // turn on motor B
        analogWrite(in3, 0);
        analogWrite(in4, 255);
        delay(250);
        Stop();
}

int LookRight(){
    servo.write(0);
     delay(500);
    float  Right = Look(); 
    servo.write(90);
     delay(500);
    return Right;
}


int LookLeft(){
    servo.write(180);
     delay(500);
    float  Left = Look();
   
    servo.write(90);
     delay(500);
    return Left;
}

int Look(){
 // Clears the trigPin
digitalWrite(TRIGGER_PIN, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(TRIGGER_PIN, HIGH);
delayMicroseconds(10);
digitalWrite(TRIGGER_PIN, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
int duration = pulseIn(ECHO_PIN, HIGH);

// Calculating the distance
float distance= duration*0.034/2;
return distance;
}

void Drift(){
  analogWrite(enA, 200); // speed right wheels 
  analogWrite(enB, 200); // speed left wheels 
   GoLeft();
  analogWrite(enA, 70); // speed right wheels 
  analogWrite(enB, 70); // speed left wheels 
}
