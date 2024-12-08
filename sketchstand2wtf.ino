#include "HX711.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,4,5,6,7);
const int buttonPin = 12;//start sequence button
const int ledPin = 13;//LED indicator and/or buzzer
const int igniterPin = 11;//igniter transistor circuit
int buttonState = 0;
//#define DOUT 3
//#define CLK 2
HX711 scale();
//(3,2);
float calibration_factor = -560;// calibration factor for loadcell may need to be changed
void setup(){

  pinMode(buttonPin, INPUT);
  pinMode(igniterPin,OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("HX711 Rocket Motor Dynamometer,V.5");
  Serial.println("Affix motor nozzle up. Place igniter in nozzle. Move away from test stand.");
  Serial.println("Press start button to initialize ignition sequence.");
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" MODEL ROCKET ");
  lcd.setCursor(0,1);
  lcd.print(" DYNAMOMETER");
  delay(2000);
 scale.set_scale();
  scale.tare();//Reset the scale to 0
  long zero_factor = scale.read_average();//Get a baseline reading
  Serial.print("Zero factor:");// This can be used to remove the need to tare the scale 
  Serial.println(zero_factor);
  Serial.println("");
}

void loop(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Rocket Dyno ");
  lcd.setCursor(0,1);
  lcd.print(" STDBY ");
  scale.set_scale(calibration_factor);
  lcd.print("g");
  delay(500);
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Rocket Dyno ");
    lcd.setCursor(0,1);
    lcd.print(" STAND CLEAR!");
    Serial.println("IGNITION SEQUENCE ACTIVATED!");
    
    for (int i=0; i<=50; i++){
      digitalWrite(ledPin,HIGH);
      delay(100);
      digitalWrite (ledPin,LOW);
      delay (100);
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Rocket Dyno");
    lcd.setCursor(1,1);
    lcd.print(" AQUIRING DATA");
    digitalWrite(igniterPin,HIGH);
    Serial.print("Start time, ms:");
    Serial.print(millis());
    Serial.println("");
    Serial.println();
    for (int i=0; i<= 800; i++){    //800 samples at 80sa/sec = 10 seconds theoretical
      scale.set_scale(calibration_factor);//Adjust to the calibration factor
      Serial.print(scale.get_units(), 1);
      Serial.println();
    }
Serial.println();
Serial.print("Stop Time, ms:");
Serial.print(millis());
digitalWrite (ledPin,LOW);
digitalWrite (igniterPin,LOW);
Serial.println();

  }
}