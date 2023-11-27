#include "DHT.h"
#define DHTPIN 2     // Digital pin 2 connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE); //Initialize the DHT component
//defines motor driver pins
int m1a = 9;
int m1b = 10;
int m2a = 11;
int m2b = 12;
char val;
//defines led pins
int lfled = 7;
int rfled = 4;
int lbled = 8;
int rbled = 13;

// defines pins numbers for supersonic, buzzer and rgb led
const int trigPin = 6;
const int echoPin = 5;
const int buzzer = 3;
int redPin = A0;
int greenPin = A1;
int bluePin = A2;

// defines variables
long duration;
int distance;
int safetyDistance;

//milis time events
const long eventTime_carControl = 300;
const long eventTime_dhtSensor = 10000;
const long eventTime_distanceSensor = 400;

unsigned long previousTime_carControl,previousTime_dhtSensor,previousTime_distanceSensor = 0;

void setup() 
{  
pinMode(m1a, OUTPUT);  // Digital pin 9 set as output Pin
pinMode(m1b, OUTPUT);  // Digital pin 10 set as output Pin
pinMode(m2a, OUTPUT);  // Digital pin 11 set as output Pin
pinMode(m2b, OUTPUT);  // Digital pin 12 set as output Pin

pinMode(lfled, OUTPUT);  // Digital pin 7 set as output Pin
pinMode(rfled, OUTPUT);  // Digital pin 4 set as output Pin
pinMode(lbled, OUTPUT);  // Digital pin 8 set as output Pin
pinMode(rbled, OUTPUT);  // Digital pin 13 set as output Pin

pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(buzzer, OUTPUT);
pinMode(redPin, OUTPUT); // analog pin A0 set as output Pin
pinMode(greenPin, OUTPUT); // analog pin A1 set as output Pin
pinMode(bluePin, OUTPUT); //analog pin A2 set as output Pin

Serial.begin(9600);
  dht.begin();
}

void loop()
{
  unsigned long currentTime = millis();

  //CAR operation
if (currentTime - previousTime_carControl >= eventTime_carControl) 
{
    carControl();
    previousTime_carControl = currentTime;
}
 
  //dht operation
if (currentTime - previousTime_dhtSensor >= eventTime_dhtSensor) 
{
    dhtSensor();
    previousTime_dhtSensor = currentTime;
}
  //dISTANCE operation
if (currentTime - previousTime_distanceSensor >= eventTime_distanceSensor) 
{
    distanceSensor();
    previousTime_distanceSensor = currentTime;
}

}

void carControl(){
while (Serial.available() > 0)
  {
  val = Serial.read();
  Serial.println(val);
  }

  if( val == 'F') // Forward
    {
      digitalWrite(m1a, HIGH);
      digitalWrite(m1b, LOW);
      digitalWrite(m2a, HIGH);
      digitalWrite(m2b, LOW);

      digitalWrite(lfled, HIGH);
      digitalWrite(rfled, HIGH);

    }
  else if(val == 'B') // Backward
    {
      digitalWrite(m1a, LOW);
      digitalWrite(m1b, HIGH);
      digitalWrite(m2a, LOW);
      digitalWrite(m2b, HIGH); 

      digitalWrite(lbled, HIGH);
      digitalWrite(rbled, HIGH);
    }
  
    else if(val == 'L') //Left
    {
    digitalWrite(m1a, HIGH);
    digitalWrite(m1b, LOW);
    digitalWrite(m2a, LOW);
    digitalWrite(m2b, LOW);

    digitalWrite(lfled, HIGH);

    }
    else if(val == 'R') //Right
    {
    digitalWrite(m1a, LOW);
    digitalWrite(m1b, LOW);
    digitalWrite(m2a, HIGH);
    digitalWrite(m2b, LOW); 

    digitalWrite(rfled, HIGH);

    }
    
  else if(val == 'S') //Stop
    {
    digitalWrite(m1a, LOW);
    digitalWrite(m1b, LOW);
    digitalWrite(m2a, LOW);
    digitalWrite(m2b, LOW); 

    digitalWrite(lfled, LOW);
    digitalWrite(rfled, LOW); 
    digitalWrite(lbled, LOW);
    digitalWrite(rbled, LOW);

    }
  else if(val == 'I') //Forward Right
    {
    digitalWrite(m1a, LOW);
    digitalWrite(m1b, LOW);
    digitalWrite(m2a, HIGH);
    digitalWrite(m2b, LOW);

    digitalWrite(rfled, HIGH);

    }
  else if(val == 'J') //Backward Right
    {
    digitalWrite(m1a, LOW);
    digitalWrite(m1b, LOW);
    digitalWrite(m2a, LOW);
    digitalWrite(m2b, HIGH);

    digitalWrite(rbled, HIGH);
    }
   else if(val == 'G') //Forward Left
    {
    digitalWrite(m1a, HIGH);
    digitalWrite(m1b, LOW);
    digitalWrite(m2a, LOW);  
    digitalWrite(m2b, LOW);

    digitalWrite(lfled, HIGH);

    }
  else if(val == 'H') //Backward Left
    {
    digitalWrite(m1a, LOW);
    digitalWrite(m1b, HIGH);
    digitalWrite(m2a, LOW);
    digitalWrite(m2b, LOW); 
   
    digitalWrite(lbled, HIGH);
    }
    else if(val == 'V') //horn on
    {
      digitalWrite(buzzer, HIGH);
    }
    else if(val == 'v') //horn off
    {
      digitalWrite(buzzer, LOW);
    }
}

void dhtSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
    String str1 = String(t, 2);
    String str2 = String(h, 2);
    String str3 = String(f, 2);
      
    String my_String = "{'data':[{'data1':'"+str1+"','data2':'"+str2+"','data3':'"+str3+"'}]}";
    Serial.println(my_String);  
    
}

void distanceSensor(){
  // Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;

safetyDistance = distance;

if (safetyDistance <= 10 || safetyDistance == 1189){
  digitalWrite(buzzer, HIGH);
  analogWrite(redPin, 0);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 255);

  //stop the car
  digitalWrite(m1a, LOW);
  digitalWrite(m2a, LOW);
}
else if(safetyDistance > 45){
  digitalWrite(buzzer, LOW);
  analogWrite(redPin, 255);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 255);
}
else {
  digitalWrite(buzzer, LOW);
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 255);
}

// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
}

