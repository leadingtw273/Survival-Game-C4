unsigned long getTime;
int timeRange[] = {0,0,0,0,0,0,0,0};
int total = (int)sizeof(timeRange)/2;
int strTime = 0;
int clockTime = 0;

void setTimeRange(int setTime){
  
  for(int x = 0;x < total;x++){
    if(x == 0){
      timeRange[x] = setTime;
    }else{
      timeRange[x] = timeRange[x-1]/2;  
    }
    Serial.println(timeRange[x]);
  }    
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  setTimeRange(600);
}

void loop() {
  // put your main code here, to run repeatedly:
  

  getTime = millis();
  for(int x = total-1; x>=0; x--){
    if(timeRange[0] - ((getTime/1000) - strTime) > timeRange[x]){
       clockTime = 8-x;
    }
  }
  
  Serial.println(clockTime);

  
  Serial.print("pass Time: ");  
  Serial.println(getTime/1000);
  Serial.print("last Time: ");  
  Serial.println(timeRange[0] - (getTime/1000));
  //Serial.println(timeRange[(getTime/1000)%8]);
  delay(1000);
}
