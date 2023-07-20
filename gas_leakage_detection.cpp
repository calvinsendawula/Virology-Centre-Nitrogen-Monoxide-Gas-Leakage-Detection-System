// 136047 Sendawula Calvin Jonathan ICS 4A

#include<LiquidCrystal.h>
LiquidCrystal lcd(3,5,6,9,10,11);

// Sensor Pins
const int ledPin = 2;
const int buzzerPin = 4;
const int buttonPin = 7;

// Variables
int gasRead = 0;
int buttonState = 0;

// Functions
void runMotor(int speed) {
  int maxAnalogValue = 255;
  int analogValue = map(speed, 0, 60, 0, maxAnalogValue);
  analogWrite(A1, analogValue);
}

void openDoor() {
  runMotor(-3);
  delay(1000);
  runMotor(0);
}

void closeDoor() {
  runMotor(3);
  delay(1000);
  runMotor(0);
}

void buzz(int freq, int del){
  tone(buzzerPin,freq);
  delay(del);
  noTone(buzzerPin);
  delay(del);
}

void ledOn(int del){
  digitalWrite(ledPin, HIGH);
  delay(del);
}

void ledOff(int del){
  digitalWrite(ledPin, LOW);
  delay(del);
}

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
}

void loop()
{
  buttonState = digitalRead(buttonPin);
  gasRead = analogRead(A0);
  int gasConcentration = map(gasRead, 85, 380, 0, 100);
  
  lcd.setCursor(0,1);
  lcd.print("NO(ppm): ");
  lcd.setCursor(9,1);
  lcd.print(gasConcentration);
  lcd.setCursor(11,1);
  lcd.print("e4ppm");
  delay(10);
  
  if(buttonState == HIGH){
  	lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Opening door");
    for(int i=0; i<4; i++){
      ledOn(200);
      buzz(500, 500);
      ledOff(200);
      buzz(500, 500);
    }
    openDoor();
    lcd.clear();
  }

  if(gasConcentration >= 30 && gasConcentration < 50){
    ledOn(1000);
    ledOff(1000);
  } else if(gasConcentration >= 50 && gasConcentration < 70){
    ledOn(500);
    buzz(200, 500);
    ledOff(500);
    buzz(200, 500);
  } else if(gasConcentration >= 70){
    closeDoor();
    ledOn(200);
    buzz(500, 200);
    ledOff(200);
    buzz(500, 200);
  }

  if(Serial.available()>0){
    String data = Serial.readString();
    if(data == "stop"){
      noTone(buzzerPin);
      lcd.setCursor(0,0);
      for(int i=60; i>0; i--){
      	lcd.clear();        
        lcd.print("Time left: ");
        lcd.print(i);
        lcd.print(" s");
        delay(1000);
      }
      loop();
    }
  }
}
