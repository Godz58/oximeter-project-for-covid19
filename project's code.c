const int leftForward=A0;
const int leftBackward=A1;
const int rightForward=A2;
const int rightBackward=A3;
int buzzPin=8;
#include <LiquidCrystal.h>
#include <IRremote.h> 
//LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     1000
PulseOximeter pox;
uint32_t tsLastReport = 0;
 
void onBeatDetected()
{
    Serial.println("Beat!");
}
 IRrecv irrcev (7);
decode_results results;
void setup()
{
 lcd.begin(16, 2);
    irrcev.enableIRIn();
    if (!pox.begin()) {
        lcd.println("FAILED");
        for(;;);
    } else {
        lcd.println("SUCCESS");
    }
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
}
 
void loop()
{    
    // Make sure to call update as fast as possible
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
      
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Heart rate:");
        lcd.print(pox.getHeartRate());
        lcd.setCursor(0, 1);
        lcd.print("SpO2: ");
        lcd.print(pox.getSpO2());
        lcd.print("%");
        if(pox.getSpO2()<=93||pox.getHeartRate()<=60){digitalWrite(buzzPin,HIGH);}

        tsLastReport = millis();
    }    if(irrcev.decode(&results)){  
 if(results.value==0xFF18E7){
  digitalWrite(leftForward,HIGH);
  digitalWrite(rightForward,HIGH);delay(1000);
  irrcev.resume();}
 ///////////////////////////////////////////////////
 
   else if(results.value==0xFF4AB5)
  {digitalWrite(leftBackward,HIGH);
  digitalWrite(rightBackward,HIGH);delay(1000);
  irrcev.resume();}
 ////////////////////////////////////////////////////
 
   else if(results.value==0xFF5AA5){
   analogWrite(leftForward,254);
   analogWrite(rightForward,125);delay(1000);
   irrcev.resume();}
/////////////////////////////////////////////////////

   else if(results.value==0xFF10EF){
   analogWrite(leftForward,125);
   analogWrite(rightForward,254);
   delay(1000);
   irrcev.resume();}
/////////////////////////////////////////////////////

 else if(results.value==0xFF38C7){
   analogWrite(leftForward,0);
   analogWrite(rightForward,254);delay(1000);
   irrcev.resume();}
   
 else if(results.value==0xFF6897){
  digitalWrite(leftForward,LOW);
   digitalWrite(rightForward,LOW);
    digitalWrite(leftBackward,LOW);
   digitalWrite(rightBackward,LOW);delay(1000);
   irrcev.resume();}}}
        
  