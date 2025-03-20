#include <Servo.h>
#include <Stepper.h>
#include <Ticker.h>
#include <Wire.h> 
//#include <LiquidCrystal_I2C.h>
#include <MobaTools.h>
#include "LCD-SOLDERED.h"

LCD lcd(16,2);

#define VRX_PIN  A2
#define VRY_PIN  A3

const int buttonPin = 13;     
//LiquidCrystal_I2C lcd(0x27,20,4);
int counter = 0;
int counter2 = 0;
const byte pin1 = 4;
const byte pin2 = 5;
const byte pin3 = 6;
const byte pin4 = 7;
const unsigned int motorStepsPerRev = 2048;
MoToStepper stepper( motorStepsPerRev, HALFSTEP);
int servoPin = 8;
Servo rampa;
int broj = 1;
int prvi_senzor1 = 0;
int drugi_senzor1 = 0;
int prvi_senzor2 = 100000;
int drugi_senzor2 = 100000;


void setup() {
//  lcd.init();
  lcd.begin();
  lcd.backlight();
  stepper.attach(pin1, pin2, pin3, pin4 );
  stepper.setSpeed(250);
  stepper.setRampLen(10);
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  rampa.write(0);
  pinMode(buttonPin, INPUT_PULLUP);
}

void podesavanje () {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("podesavanje");
  delay(1000);
  stepper.write(-10);
  delay(1000);
  lcd.clear();
}


void podizanje(char prvi[],char drugi[]) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MOST SE PODIZE");
  rampa.attach(servoPin);    
  delay(1000);
  rampa.write(90);
  delay(2000);
  stepper.write(140);
  delay(1000);
  rampa.detach();
  lcd.clear();
  lcd.print("BROD MOZE PROCI");
  int broj = 1;
  delay(1000);
  prvi_senzor1 = analogRead(A0);
  drugi_senzor1 = analogRead(A1);
  prvi_senzor2 = prvi_senzor1 - 20;
  drugi_senzor2 = drugi_senzor1 - 20;
  delay(1000);
  while (broj > 0) {
    prvi_senzor1 = analogRead(A0);
    drugi_senzor1 = analogRead(A1);
    if (prvi == "prvi_senzor1") {
      Serial.println(broj);
      if (drugi_senzor1 < drugi_senzor2) {
        broj --;
        lcd.clear();
      }
    }
    else if (prvi == "drugi_senzor1") {
      Serial.println(broj);

      if (prvi_senzor1 < prvi_senzor2) {
        broj --;
        lcd.clear();
      }
    }
  prvi_senzor2 = prvi_senzor1 - 20;
  drugi_senzor2 = drugi_senzor1 - 20;

  }
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MOST SE SPUSTA");
  rampa.attach(servoPin);
  delay(1000);
  stepper.write(-90);
  delay(1000);
  rampa.write(0);
  delay(5000);
  rampa.detach();
  prvi_senzor1 = analogRead(A0);
  drugi_senzor1 = analogRead(A1);
  lcd.clear();
}

void pisi() {
  int buttonValue = digitalRead(buttonPin);
  prvi_senzor1 = analogRead(A0);
  drugi_senzor1 = analogRead(A1);
  lcd.setCursor(0,0);
  lcd.print("MOZE PROCI");
  lcd.setCursor(0,1);
  lcd.print(counter);
  
  Serial.print("gleda");
  Serial.print(prvi_senzor1);
  Serial.print(" ");
  Serial.print(drugi_senzor1);
  Serial.print(" ");
  Serial.println(buttonValue);
  if (prvi_senzor1 < prvi_senzor2) {
    podizanje("prvi_senzor1", "drugi_senzor1");
  }

  if (drugi_senzor1 < drugi_senzor2) {
    podizanje("drugi_senzor1", "prvi_senzor1");
  }

  if (buttonValue == 1){
    podesavanje();
  }
  prvi_senzor2 = prvi_senzor1 - 50;
  drugi_senzor2 = drugi_senzor1 - 50;
}

void nista() {
  Serial.println("Brod nemoze proc");
  lcd.setCursor(0,0);
  lcd.print("NEMOZE PROCI");
  lcd.setCursor(0,1);
  lcd.print(counter2);
  }

Ticker timer1(pisi, 1, 9000);
Ticker timer2(nista, 1000, 60);



void loop() {
  digitalWrite(12, 1);
  delay(1000);
  timer1.start();
  counter = 9000;
  while (timer1.counter() < 9000) {
    timer1.update();
    counter --;
  }
  lcd.clear();
  digitalWrite(12, 0);
  timer1.stop();
  timer2.start();
  counter2 = 60;
  while (timer2.counter() < 60){
    timer2.update();
    counter2 --;
  }
  lcd.clear();
  timer2.stop();
}
