#include <Wire.h>

int timeRange[] = {10,20,40,80,160,320,640,1280,2560,5120};
int timeBee[]   = {125,250,500,1000,2000,3000,5000,8000,13000,21000,34000};
int total = (int)sizeof(timeRange)/4;
int strTime = 0;
int totalTime = 170;
int clockTime = 0;
int tempTime = millis();
int lastTime = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {


  
    
    lastTime = totalTime - (millis()/1000 - strTime);
    
    for(int x = 9 ; x >= 0; x--){
      if(lastTime > timeRange[x]){
        clockTime = timeBee[x+1];
        break;
      }else if(lastTime < timeRange[0]){
        clockTime = timeBee[0];
      }
    }
    
    int x = millis() - tempTime;
    if(x >= clockTime){
      Serial.println("bee!!");
      Serial.print("x: ");
      Serial.println(x);
      Serial.print("lastTime: ");
      Serial.println(lastTime);
      Serial.print("clockTime: ");
      Serial.println(clockTime);
      tempTime = millis();
    }
    
    String str = String(lastTime);
}
