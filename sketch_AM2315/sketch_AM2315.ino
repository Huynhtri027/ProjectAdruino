#include <cactus_io_AM2315.h>
#include <Wire.h>
const int ledPin = 13;
const int AOUTPin = 0;
const int DOUTPin = 8;
String valdata;
int value, serialValue;

boolean statustime = false;
long lowV = 60000;
long highV = 90000;;
unsigned long time = 0;
unsigned long sensorTime = 0;


AM2315 am2315;
void readCovalue();
void HandleSetLimitCO();
void HandleLimitCO();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  Serial.print("\t\t\t\t");
  
  Serial.println("AM2315 Humidity - Temperature Sensor - MQ7 Sensor");
  Serial.print("\n");
  Serial.println("RH\t\tTemp (C)\tTemp (F)\tTemp (PPM)\tSetValueCO (PPM)\tWarning!!!");
  pinMode(DOUTPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  if(!am2315.begin()){
    Serial.println("AM2315 Humidity - Temperature Sensor");
    Serial.println("Sensor not found, check wiring & pullups!!!");
    while(1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
 
  readCovalue();
  HandleSetLimitCO();
  am2315.readSensor();

  Serial.print(am2315.getHumidity()); Serial.print(" %\t\t");
  Serial.print(am2315.getTemperature_C()); Serial.print(" *C\t");
  Serial.print(am2315.getTemperature_F()); Serial.print(" *F\t");
  Serial.print(value); Serial.print(" ppm\t\t");
  Serial.print(serialValue); Serial.print(" ppm\t\t\t");
  HandleLimitCO();

  // Add a 2 second
  delay(2000);
}

void readCovalue(){
  value = analogRead(AOUTPin);
  unsigned long currentTime = millis();

  if(statustime == false){
    if(currentTime - time > lowV){
      time = currentTime;
      statustime = true;
    }
  }else if(statustime == true){
    if(currentTime - time < highV){
      time = currentTime;
      analogWrite(DOUTPin, 255);
      statustime = false;
    }
  }
  if(currentTime - sensorTime > value){
    sensorTime = currentTime;
  }
}
void HandleSetLimitCO(){
  while(Serial.available()){
    char chdata = Serial.read();
    if(isDigit(chdata)){
      valdata += chdata;
    }

    if(chdata == '\n'){
      serialValue = valdata.toInt();
      valdata = "";
    }
  }
  delay(10);
  while(Serial.available() > 0){
  Serial.read();
  if (Serial.available() == 0)
    delay(10);
  }
}

void HandleLimitCO(){
  if(value >= serialValue){
    digitalWrite(ledPin, HIGH);
    Serial.println("Led ON");
    digitalWrite(DOUTPin, LOW);
  }else if(value <= serialValue){
    digitalWrite(ledPin, LOW);
    Serial.println("Led OFF");
    digitalWrite(DOUTPin, HIGH);
  }else{
    Serial.println("Invalid!");
  }
}


