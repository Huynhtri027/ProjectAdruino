#include <cactus_io_AM2315.h>
#include <Wire.h>
const int ledPin = 13;
const int AOUTPin = 0;
const int DOUTPin = 2;
String valdata;
int value, serialValue;

AM2315 am2315;
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
 
  value = analogRead(AOUTPin);
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
  }else if(value <= serialValue){
    digitalWrite(ledPin, LOW);
    Serial.println("Led OFF");
  }else{
    Serial.println("Invalid!");
  }
}


