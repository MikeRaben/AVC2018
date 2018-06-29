#define encoderPin  2

#define trigPinL 12
#define trigPinR 11
#define echoPinL 13
#define echoPinR 10

long distR, distL = 0;
unsigned long previousMillis = 0; //using to avoid delay()
const long interval = 100;       //how long to wait between data broadcasts

long ticks;
double wheelCircumf = 14.14;   //Circumference of wheen in inches
int distTravelled = 0;   //distance traveled in feet, will be sent for odometry

String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  Serial.begin(115200);
  distR = 0;
  distL = 0;
  ticks = 0;

  pinMode(trigPinL, OUTPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(echoPinR, INPUT);
  
  attachInterrupt(0, encoderTick, CHANGE);
}

void loop() {
  distR = ping(trigPinR, echoPinR);
  delay(50);
  distL = ping(trigPinL, echoPinL);
  delay(50);
}

void serialEvent(){
  //serialEvent is called between loops when data is received
  while (Serial.available()) {
      // get the new byte:
      char inChar = (char)Serial.read();
      // add it to the inputString:
      inputString += inChar;
      if (inChar == '\n') {
        if (inputString == "tx"){
          transmit();
          inputString = "";
        } 
      }
    }
}

void transmit(){
  //send dist in feet:  total ticks from wheel encoder * wheel diameter / number of ticks per rotation (8) / 12 inches
    distTravelled = (int)(((ticks * wheelCircumf)/8)/12);
    Serial.print("D:");
    Serial.print(distTravelled);
    Serial.print("R:");
    Serial.print(distR);
    Serial.print("L:);
    Serial.println(distL);
}

void encoderTick(){
  ticks++;
}

long ping(trigPin, echoPin) {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2 / 29.1);
  if (distance >= 200 || distance <= 0){
    distance = 999;
    //Out of range
  }
  return distance;
  
}
