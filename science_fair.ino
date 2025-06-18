#include <Servo.h>

const int min_distance = 18; 
const int wait = 1000; // Delay between action and returning to original position
const int wait2 = 1500; // Delay between presence check and moisture check
const int wait3 = 100; // Delay between each cycle
const int wait4 = 500; // Delay between returning to original position and checking again
const int defaultangle = 80;
const int rotationangle = 70;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int servopin = 8;
const int trigPin = 9;
const int echoPin = 10;
const int buttonpin = 11;
const int red = A4;
const int green = A3;
const int blue = A2;
//bgr
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int dis, pos;
String read="";
Servo servo;

void setup() {
	pinMode(red, OUTPUT);
  digitalWrite(red,HIGH);
	pinMode(green, OUTPUT);
	pinMode(blue, OUTPUT);
  pinMode(trigPin, OUTPUT);  
  pinMode(buttonpin, INPUT);
	pinMode(echoPin, INPUT);
	pinMode(green, OUTPUT);
	pinMode(blue, OUTPUT);


  servo.attach(servopin);
  Serial.begin(9600);

  servo.write(defaultangle);
  delay(wait);
  // lock("connected");
  rgb(0);
}

float get_distance() {
  float duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return (pulseIn(echoPin, HIGH) * .0343) / 2;
}
void move(int where) {
  /* 
  where:
  -1 - left
   0 - default
   1 - right
  */
  pos = defaultangle + (rotationangle*where);
  servo.write(pos);
}
void act() {
  Serial.println(1); //send signal to PC
  rgb(1);
  while(read==""){
    delay(100);
    read = Serial.readString();//Wait for pc to process
  }
  if(read == "A"){
    move(1);
    rgb(2);
  }else if(read=="B"){
    move(-1);
    rgb(3);
  }else{
    rgb(1);
  }
  delay(wait);
  move(0);
  delay(wait4);
  rgb(0);
  read="";
}
void check() {
  dis = get_distance();
  if(dis < 1){
    
  }else if(dis < min_distance){
    rgb(4);
    delay(wait2);
    rgb(0);
    act();
  }else{
  }
}
void rgb(int color){// 0-default 1-r, 2-g, 3-b, 4-w
  if(color == 0){
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);
  }
  else if(color == 1){
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);
  }
  else if(color == 2){
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    digitalWrite(blue, LOW);
  }
  else if(color == 3){
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue, HIGH);
  }
  else if(color == 4){
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue, HIGH);
  }
}
int buttonstate;
void loop() {
  if(millis()>2000){
    buttonstate = digitalRead(buttonpin);
    if(buttonstate==HIGH){//Let user emptify
          // rgb(1);
          move(1);
          delay(wait);
          do{
            buttonstate = digitalRead(buttonpin);
            delay(wait3);
          }while(buttonstate==LOW);

          do{
            buttonstate = digitalRead(buttonpin);
            delay(wait3);
          }while(buttonstate==HIGH);

          move(0);
          delay(wait);
          // rgb(0);
    }else{//Main code
      check();

    }
    delay(wait3);
  }
}