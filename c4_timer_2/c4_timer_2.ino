#include <Keypad.h>
#include <Wire.h>
#include "U8glib.h"

#define KEY_ROWS 4  // 薄膜按鍵的列數
#define KEY_COLS 4  // 薄膜按鍵的行數

U8GLIB_ST7920_128X64_1X u8g(15, 14, 16); //宣告繪圖設定

// 設置按鍵模組
char keymap[KEY_ROWS][KEY_COLS] = {
  {'F', 'E', 'D', 'C'},
  {'B', '3', '6', '9'},
  {'A', '2', '5', '8'},
  {'0', '1', '4', '7'}
};

byte rowPins[KEY_ROWS] = {6, 7, 8, 9}; //Rows 0 to 3
byte colPins[KEY_COLS] = {15, 14, 16, 10}; //Columns 0 to 3

Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, KEY_ROWS, KEY_COLS);

String passcode = "1234";     // 預設密碼
String inputCode = "";        // 暫存用戶的按鍵字串
bool acceptKey = true;        // 代表是否接受用戶按鍵輸入的變數，預設為「接受」
String lineA = "",lineB = ""; // 顯示內容

//計時
int timeRange[] = {0,0,0,0,0,0,0,0};
int total = (int)sizeof(timeRange)/2;
int strTime = 0;
int clockTime = 0;
int tempTime = 0;
int lastTime = 0;
String settime = "";

byte beePin = 10;  //蜂鳴器腳位

//蜂鳴器
void bee(){
  digitalWrite(beePin,HIGH);
  delay(500);
  digitalWrite(beePin,LOW);
}

//設定時間寬度
void setTimeRange(){
  settime = "";
  while(acceptKey){
    u8g.firstPage();
    do{
      char key = keypad.getKey();
      if(acceptKey && key != NO_KEY){
        if (key == 'C') {   // 清除畫面
          settime = "";
          Serial.println();
          lineB = "";
        } else if (key == 'D') {  // 確定輸入時間
          Serial.println("\nTime set!");
          strTime = millis()/1000;
          lineA = "Time set!";
          lineB = "PIN:";
          acceptKey = false;
        } else {
          settime += key;  // 儲存時間數字
          Serial.print(key);
          lineB = settime;
        }
      }
      lineA = "set time...";
      draw(lineA,lineB);
    }while( u8g.nextPage() );
    delay(100);
  }
  
  for(int x = 0;x < total;x++){
    if(x == 0){
      timeRange[x] = settime.toInt();
    }else{
      timeRange[x] = timeRange[x-1]/2;  
    }
    Serial.println(timeRange[x]);
  } 
  
  resetLocker();
  
}

//繪圖函式
void draw(String A,String B) {
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0,10);
  u8g.print(A);
  u8g.setPrintPos(0,30);
  u8g.print(B);
}

// 顯示「歡迎光臨」後，重設LCD顯示文字和輸入狀態。
void resetLocker() {
  Serial.print("PIN:");
  lineA = "Time... ";

  acceptKey = true;
  inputCode = "";
}

// 比對用戶輸入的密碼
void checkPinCode() {
  acceptKey = false;  // 暫時不接受用戶按鍵輸入
  // 比對密碼
  if (inputCode == passcode) {
    Serial.println("\nWelcome home!");
    acceptKey = true;
    pass();
  } else {
    Serial.println("\n***WRONG!!***");
    lineB = "***WRONG!!***";    
    resetLocker();
  }    
}

//密碼通過顯示
void pass(){
  while(acceptKey){
    u8g.firstPage();
    do{
      char key = keypad.getKey();
      if(acceptKey && key != NO_KEY){
        if(key == 'D'){
            acceptKey = false;
        }
      }
      lineA = "password correct";
      lineB = "success!!!!!!!!!";
      draw(lineA,lineB);
    }while( u8g.nextPage() );
    delay(100);
  }
  acceptKey = true;
  setPassword();
}

//設定密碼
void setPassword(){
  passcode = "";
  Serial.print("Setting passwoad... ");
  lineA = "Setting passwoad";
  lineB = "PIN:";
  while(acceptKey){
    u8g.firstPage();
    do{
      char key = keypad.getKey();
      if (acceptKey && key != NO_KEY) {
        if (key == 'C') {   // 清除畫面
          passcode = "";
          Serial.println();
          lineB = "PIN:";
        } else if (key == 'D') {  // 確定輸入密碼
          Serial.println("\npassword set!!");
          lineA = "password set!!";
          lineB = "";
          acceptKey = false;
        } else {
          passcode += key;  // 儲存用戶的密碼
          Serial.print(key);
          lineB = "PIN: " + passcode;
        }
      }
      draw(lineA,lineB);
    } while( u8g.nextPage() );
    delay(100);
  } 
  acceptKey = true;
  setTimeRange();
}

 //遊戲結束
void gameOver(){
  while(acceptKey){
    u8g.firstPage();
    do{
      char key = keypad.getKey();
      if(acceptKey && key != NO_KEY){
        if(key == 'D'){
            acceptKey = false;
        }
      }
      lineA = "time out!!!!!";
      lineB = "fail.........";
      draw(lineA,lineB);
    }while( u8g.nextPage() );
    delay(50);
  }
  acceptKey = true;
  setPassword();
}

void setup() {
  Serial.begin(9600);
  u8g.setColorIndex(1);         // pixel on

  pinMode(beePin,OUTPUT);
  
  lineB = "PIN:";
  setPassword();
}

void loop() {

  
  //draw(A,B)
  while(acceptKey){
    
    for(int x = total-1; x>=0; x--){
      if(timeRange[0] - ( millis()/1000 - strTime) > timeRange[x]){
         clockTime = 8-x;
      }
    }
    Serial.println(clockTime);
  
    if((millis()/1000 - tempTime) >= clockTime){
      bee();
      tempTime = millis()/1000;
    }
  
    u8g.firstPage();
    do{
      char key = keypad.getKey();
      // 若目前接受用戶輸入，而且有新的字元輸入…
      if (acceptKey && key != NO_KEY) {
        if (key == 'C') {   // 清除畫面
          inputCode = "";
          Serial.println();
          lineB = "PIN: ";
        } else if (key == 'D') {  // 比對輸入密碼
          checkPinCode();
        } else {
          inputCode += key;  // 儲存用戶的按鍵字元
          Serial.print("*");
          lineB = "PIN: " + inputCode;
        }
      }
      lastTime = timeRange[0] - (millis()/1000 - strTime);
      String str = String(lastTime);
      lineA = "Time... " + str  ;
      draw(lineA,lineB);

      if(lastTime <= 0){
        acceptKey = false;
      }
      
    } while( u8g.nextPage() );
    delay(100);    
  }

  acceptKey = true;
  gameOver();
  
}
