#include <Servo.h>

const uint8_t LeftLegPin = D8;
const uint8_t RightLegPin = D4;
const uint8_t LeftFootPin = D7;
const uint8_t RightFootPin = D3;

Servo LeftLeg;
Servo LeftFoot;
Servo RightFoot;
Servo RightLeg;

bool walkMode = false;
bool rollMode = false;
int roll_right_forward_speed = 40;
int roll_left_forward_speed = 40;
int roll_right_backward_speed = 40;
int roll_left_backward_speed = 40;
String command = "";
int currentmillis1 = 0;
int currentmillis2 = 0;
int currentmillis3 = 0;

long ultrasound_distance_1() {
   long duration, distance;
   digitalWrite(12,LOW);
   delayMicroseconds(2);
   digitalWrite(12, HIGH);
   delayMicroseconds(10);
   digitalWrite(12, LOW);
   duration = pulseIn(14, HIGH);
   distance = duration/58;
   return distance;
}

void AvoidanceWalk() {
  if (ultrasound_distance_1() <= 15) {
    Home();
    delay(50);
    WalkRight();
  }
  WalkForward();
}

void AvoidanceRoll() {
  if (ultrasound_distance_1() <= 15) {
    DancerRollStop();
    DancerRollRight(roll_right_forward_speed, roll_left_forward_speed);
  }
  Dancer(roll_right_forward_speed, roll_left_forward_speed);
}

void Home() {
  LeftFoot.attach(LeftFootPin, 544, 2400);
  RightFoot.attach(RightFootPin, 544, 2400);
  LeftLeg.attach(LeftLegPin, 544, 2400);
  RightLeg.attach(RightLegPin, 544, 2400);
  delay(100);
  LeftFoot.write(90);
  RightFoot.write(90);
  LeftLeg.write(60);
  RightLeg.write(120);
  delay(100);
  LeftLeg.detach();
  RightLeg.detach();
}

void SetWalk() {
  walkMode = true;
  rollMode = false;
  LeftLeg.attach(LeftLegPin, 544, 2400);
  RightLeg.attach(RightLegPin, 544, 2400);
  LeftLeg.write(60);
  RightLeg.write(120);
  delay(100);
  LeftLeg.detach();
  RightLeg.detach();
  delay(100);
}

void SetRoll() {
  walkMode = false;
  rollMode = true;
  LeftLeg.attach(LeftLegPin, 544, 2400);
  RightLeg.attach(RightLegPin, 544, 2400);
  LeftLeg.write(170);
  RightLeg.write(10);
  delay(100);
  LeftLeg.detach();
  RightLeg.detach();
  delay(100);
}

void TiltToRight() {
  LeftLeg.attach(LeftLegPin, 544, 2400);
  RightLeg.attach(RightLegPin, 544, 2400);
  LeftLeg.write(0);
  RightLeg.write(65);
  delay(300);
  LeftLeg.detach();
  RightLeg.detach();
  delay(300);
}

void TiltToLeft() {
  LeftLeg.attach(LeftLegPin, 544, 2400);
  RightLeg.attach(RightLegPin, 544, 2400);
  LeftLeg.write(120);
  RightLeg.write(180);
  delay(300);
  LeftLeg.detach();
  RightLeg.detach();
  delay(300);
}

void MoveRightFoot(int s, int t) {
  RightFoot.attach(RightFootPin, 544, 2400);
  RightFoot.write(s);
  delay(t);
  RightFoot.write(90);
  delay(100);
  RightFoot.detach();
  delay(300);
}

void MoveLeftFoot(int s, int t) {
  LeftFoot.attach(LeftFootPin, 544, 2400);
  LeftFoot.write(s);
  delay(t);
  LeftFoot.write(90);
  delay(100);
  LeftFoot.detach();
  delay(300);
}

void ReturnFromRight() {
  LeftLeg.attach(LeftLegPin, 544, 2400);
  LeftLeg.write(60);
  RightLeg.attach(RightLegPin, 544, 2400);
  for(int count=65;count<=120;count+=2) {
    RightLeg.write(count);
    delay(25);
  }
  delay(300);
  LeftLeg.detach();
  RightLeg.detach();
}

void ReturnFromLeft() {
  RightLeg.attach(RightLegPin, 544, 2400);
  RightLeg.write(120);
  LeftLeg.attach(LeftLegPin, 544, 2400);
  for(int count=120;count>=60;count-=2) {
    LeftLeg.write(count);
    delay(25);
  }
  delay(300);
  LeftLeg.detach();
  RightLeg.detach();
}

void WalkForward() {
  TiltToRight();
  delay(100);
  MoveRightFoot(70, 250);
  delay(100);
  ReturnFromRight();
  
  TiltToLeft();
  delay(100);
  MoveLeftFoot(140, 350);
  delay(100);
  ReturnFromLeft();
}

void WalkRight() {
  TiltToRight();
  delay(100);
  MoveRightFoot(70, 350);
  delay(100);
  ReturnFromRight();
}

void WalkBackward() {
  TiltToRight();
  delay(100);
  MoveRightFoot(120, 250);
  delay(100);
  ReturnFromRight();
  TiltToLeft();
  delay(100);
  MoveLeftFoot(60, 350);
  delay(100);
  ReturnFromLeft();
}

void WalkLeft() {
  TiltToLeft();
  delay(100);
  MoveLeftFoot(140, 350);
  delay(100);
  ReturnFromLeft();
}

void joystickWalk(int x, int y) {
  if ((x >= -5)&&(x <= 5)&&(y >= -5)&&(y <= 5)){ command = "home"; }
  else if (y < -25 && x < -25 || y > 25 && x < -25) { command = "left"; }
  else if (y < -25 && x > 25 || y > 25 && x > 25) { command = "right"; }
  else if (y < -25) { command = "forward"; }
  else if (y > 25) { command = "backward"; }
}
  
void joystickRoll(int x, int y) {
  if ((x >= -5)&&(x <= 5)&&(y >= -5)&&(y <= 5)){DancerStop();}
  else{
    LeftFoot.attach(LeftFootPin, 544, 2400);
    RightFoot.attach(RightFootPin, 544, 2400);
    int LWS= map(y, -50, 50, 135, 45);
    int RWS= map(y, -50, 50, 45, 135);
    int LWD= map(x, 50, -50, 45, 0);
    int RWD= map(x, 50, -50, 0, -45);
    LeftFoot.write(LWS+LWD);
    RightFoot.write(RWS+RWD);
    }
}

void Settings(String speeds) {
  decodeSpeeds(speeds);
}

void decodeSpeeds(String c) {
  int counter = 0;
  String RF = "";
  String LF = "";
  String RB = "";
  String LB = "";
  
  for (int i=1; i<c.length(); i++) {
      if(isDigit(c[i])) {
          if(counter == 0) {
              RF += c[i];
          }
          else if (counter == 1) {
              LF += c[i];
          }
          else if (counter == 2) {
              RB += c[i];
          }
          else if (counter == 3) {
              LB += c[i];
          }
      }
      else if (c[i] == '&') {
          counter++;
      }
  }

  roll_right_forward_speed = RF.toInt();
  roll_left_forward_speed = LF.toInt();
  roll_right_backward_speed = RB.toInt();
  roll_left_backward_speed = LB.toInt();
  Serial.println("");
  Serial.println(c);
  Serial.println(RF.toInt());
  Serial.println(LF.toInt());
  Serial.println(RB.toInt());
  Serial.println(LB.toInt());
  
}

void setup() {
  Serial.begin(115200);
  delay(10);  
  pinMode(5, OUTPUT);
  tone(5, 440, 1000);
  Home();
}

void loop() {
  CheckClient();//if something is coming at us

  if (command == "walkmode") {
    SetWalk();
    command = "";
  }
  else if (command == "rollmode") {
    SetRoll();
    command = "";
  }
  else if (command == "home") {
    Home();
    command = "";
  }
  else if (command == "forward") {
    if(walkMode) {
      WalkForward();
      command = "";
    } else {
      DancerRollForward(roll_right_forward_speed, roll_left_forward_speed);
    }
  }
  else if (command == "backward") {
    if(walkMode) {
      WalkBackward();
      command = "";
    } else {
      DancerRollBackward(roll_right_backward_speed, roll_left_backward_speed);
    }
  }
  else if (command == "right") {
    if(walkMode) {
      WalkRight();
      command = "";
    } else {
      DancerRollRight(roll_right_forward_speed, roll_left_forward_speed);
      command = "";
    }
  }
  else if (command == "left") {
    if(walkMode) {
      WalkLeft();
      command = "";
    } else {
      DancerRollLeft(roll_right_backward_speed, roll_left_backward_speed);
      command = "";
    }
  }
  else if (command == "stop") {
    DancerRollStop();
  }
  else if (command == "avoidancewalk") {
    AvoidanceWalk();
  }
  else if (command == "avoidanceroll") {
    AvoidanceRoll();
  }
}

  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += "</html>\n";
}

void GetCoords(String str) {
  String x = str.substring(str.lastIndexOf('J')+1, str.lastIndexOf(','));
  String y = str.substring(str.lastIndexOf(',')+1,str.lastIndexOf('H')-1);
  //Serial.println("X:" + x + ", Y:" + y);
  walkMode == true ? joystickWalk(x.toInt(), y.toInt()) : joystickRoll(x.toInt(), y.toInt());
}
