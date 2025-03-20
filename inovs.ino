#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Stepper.h>
const int stepsPerRevolution = 2038;
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);
int servoPin = 6;
Servo servo;
int angle = 0;
LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {                   // initialize the lcd 
  lcd.init();
  lcd.backlight();
  servo.attach(servoPin);
}

void loop() {
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Stani !");
  for(angle = 0; angle < 90; angle++) {
        servo.write(angle);
    }
  delay(10000);
  myStepper.setSpeed(15);
	myStepper.step(stepsPerRevolution);
  lcd.setCursor(3,0);
  lcd.print("kreni !");
  delay(10000);
  myStepper.setSpeed(15);
	myStepper.step(-stepsPerRevolution);
  for(angle = 90; angle > 0; angle--) {
        servo.write(angle);
    }
  lcd.setCursor(3,0);
  lcd.print("       ");
  delay(10000);
}
