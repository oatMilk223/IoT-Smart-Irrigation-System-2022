// program: A simple water irrigation system that leaks water when the interval time is up, if it is not currently raining. Sensors used
// include DHT11 temp and humidity, rain sensor, and soil moisture sensor. Additionally, the system uses 3 leds for visual indication of watering intervals,
// as well as tubing attached to a servomotor which is triggered and tilted for water leaking.
//LEDS
int plantHealth = 0;

// RAIN SENSOR:
const int cap_D = 2;
const int cap_A = A4;
int rainAnalog;

// DHT TEMP AND HUMID:
#include "DHT.h"
#define DHTPIN A1     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// SOIL MOISTURE AND SERVO:
int moisture_signal = A0;
const int dryValue = 996;
int timer = 0;

int waterTime = 10; // adjustable interval time of which the plant is watered.
// is a minute (60secs) at default.
//int waterTime2 = 3600; //an alt value for the water interval. = an hour

//SERVO MOTOR:
#include <Servo.h>
Servo myservo;  
int pos = 0;    

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //LEDS
  pinMode(7, OUTPUT); //green - if plant is healthy and good condition
  pinMode(6, OUTPUT); // yellow - if plant is in ok conditions
  pinMode(5, OUTPUT); // red - if plant is in a bad state
  
  //SERVO
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object  // for dht module

  //DHT
  dht.begin();

  //RAIN SENSOR
  pinMode(cap_D, INPUT);
  pinMode(cap_A, INPUT);
}

void loop() {
  // LED - CHECK: - builds up via % of the water interval time specified, is then reset when watered
  if (plantHealth > waterTime - (waterTime/3)){
    digitalWrite(7, LOW);  //green turn off
    digitalWrite(6, LOW);  //yellow turn off
    digitalWrite(5, HIGH);  //red
    delay(1000);
  }
  else if (plantHealth < waterTime && plantHealth > waterTime/3){
    digitalWrite(5, LOW);  //red turn off
    digitalWrite(7, LOW);  //green turn off
    digitalWrite(6, HIGH);  //yellow
    delay(1000);
  }
  else {
    digitalWrite(5, LOW);  //red turn off
    digitalWrite(6, LOW);  //yellow turn off
    digitalWrite(7, HIGH);  //green
    delay(1000);
  }
  
  // DHT - writes humidity, temp and heat index in C and F (not humidity):
  float h = dht.readHumidity();
  float t = dht.readTemperature();  
  float f = dht.readTemperature(true); 
  float hif = dht.computeHeatIndex(f, h);  
  float hic = dht.computeHeatIndex(t, h, false);  
  Serial.println(h);
  Serial.println(t);
  Serial.println(f);
  Serial.println(hic);
  Serial.println(hif);

  // RAIN SENSOR: - checks for rain, prints analog value, and changes isRaining bool for water condition
  bool isRaining;
  if(digitalRead(cap_D) == LOW) {
      Serial.println("Raining");
      isRaining = true; 
      delay(10); 
    } else{
      Serial.println("Not raining");
      //isRaining = false; 
      isRaining = true; 
      //- change this and uncomment the other if statement to see water condition
      delay(10); 
    }
  rainAnalog=analogRead(cap_A); 
   Serial.println(rainAnalog); 

  //SOIL MOISTURE AND SERVO MOTOR - IF MOISTURE IS DRY, ITS NOT RAINING, & THE WATER INTERVAL TIME IS MET, PLANT WILL BE WATERED VIA SERVO MOTOR
  int Moisture = analogRead(moisture_signal);
  //Serial.print(" Moisture level: ");
  Serial.println(Moisture);
  if (Moisture < 1000){
  //if (Moisture >= dryValue){
    Serial.println("Soil_is_dry");
    if (timer >= waterTime && isRaining == false){
      // WATERING:
      Serial.println("Watering");
      plantHealth++; // adds health by watering
      for (pos = 180; pos >=1; pos--) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(10);                       
      }
      for (pos = 0; pos <180; pos ++) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(10);                       
      }
      plantHealth = 0;
      timer = 0;
    } else {
      Serial.println("notWatering");
    }
  } 
  else if (Moisture <= 700){
      Serial.println("Soil_too_wet");
      Serial.println("notWatering");
  } else {
    Serial.println("Soil_is_normal");
    Serial.println("notWatering");
  }
  timer ++;
  plantHealth++;
  //Serial.print(" Seconds until next water: ");
 int val = waterTime - timer;
  Serial.println(val);
}
