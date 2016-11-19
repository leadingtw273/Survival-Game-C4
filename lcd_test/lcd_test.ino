#include <Wire.h>
#include <LiquidCrystal_SR.h>

LiquidCrystal_SR lcd(2,3,TWO_WIRE);
//                   | |
//                   | \-- Clock Pin
//                   \---- Data/Enable Pin

void setup(){

  lcd.begin(16,2);               // initialize the lcd

  lcd.home ();                   // go home
  lcd.write("LiquidCrystal_SR");
}

void loop(){
}

