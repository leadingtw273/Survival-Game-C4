#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define KEY_ROWS 4  // 薄膜按鍵的列數
#define KEY_COLS 4  // 薄膜按鍵的行數

LiquidCrystal_I2C lcd(0x3F,16,2);

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
  lineA = "set time...";
  lineB = "SEC:";
  while(acceptKey){
    char key = keypad.getKey();
    if(acceptKey && key != NO_KEY){
      if (key == 'C') {   // 清除畫面
        settime = "";
        lineB = "SEC:";
      } else if (key == 'D') {  // 確定輸入時間
        strTime = millis()/1000;
        lineA = "Time set!";
        lineB = "";
        acceptKey = false;
      } else {
        settime += key;  // 儲存時間數字
        lineB = "SEC: " + settime;
      }
    }
    show_LCD();
    delay(2000);
  }
  
  for(int x = 0;x < total;x++){
    if(x == 0){
      timeRange[x] = settime.toInt();
    }else{
      timeRange[x] = timeRange[x-1]/2;  
    }
  } 
  
  resetLocker();
  
}

// 顯示「歡迎光臨」後，重設LCD顯示文字和輸入狀態。
void resetLocker() {
  lineA = "Time... ";
  lineB = "PIN: ";
  show_LCD();
  acceptKey = true;
  inputCode = "";
}

// 比對用戶輸入的密碼
void checkPinCode() {
  acceptKey = false;  // 暫時不接受用戶按鍵輸入
  // 比對密碼
  if (inputCode == passcode) {
    acceptKey = true;
    pass();
  } else {
    lineB = "***WRONG!!***";    
    show_LCD();
    delay(2000);
    resetLocker();
  }    
}

//密碼通過顯示
void pass(){
  lineA = "password correct";
  lineB = "success!!!!!!!!!";
  while(acceptKey){
    char key = keypad.getKey();
    if(acceptKey && key != NO_KEY){
      if(key == 'D'){
          acceptKey = false;
      }
    }
    show_LCD();
    delay(100);
  }
  acceptKey = true;
  setPassword();
}

//設定密碼
void setPassword(){
  char key = '';
  passcode = "";
  lineA = "Setting passwoad";
  lineB = "PIN:";
  while(acceptKey){
    key = keypad.getKey();
    Serial.println(key);
    if (acceptKey && key != NO_KEY) {
      if (key == 'C') {   // 清除畫面
        passcode = "";
        lineB = "PIN:";
      } else if (key == 'D') {  // 確定輸入密碼
        lineA = "password set!!";
        lineB = "";
        acceptKey = false;
      } else {
        passcode += key;  // 儲存用戶的密碼
        lineB = "PIN: " + passcode;
      }
    }
    show_LCD();
    delay(2000);
  } 
  acceptKey = true;
  setTimeRange();
}

 //遊戲結束
void gameOver(){
  lineA = "time out!!!!!";
  lineB = "fail.........";
  while(acceptKey){
    char key = keypad.getKey();
    if(acceptKey && key != NO_KEY){
      if(key == 'D'){
          acceptKey = false;
      }
    }
    show_LCD();
    delay(50);
  }
  acceptKey = true;
  setPassword();
}

void show_LCD(){
  lcd.setCursor(0, 0); // 設定游標位置在第一行行首
  lcd.print(lineA);
  delay(1000);
  lcd.setCursor(0, 1); // 設定游標位置在第二行行首
  lcd.print(lineB);
  delay(1000);
}

void setup() {
  Serial.begin(9600);

  pinMode(beePin,OUTPUT);
  
  lcd.init();// initialize the lcd 
   
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

    if((millis()/1000 - tempTime) >= clockTime){
      bee();
      tempTime = millis()/1000;
    }
  
    char key = keypad.getKey();
    // 若目前接受用戶輸入，而且有新的字元輸入…
    if (acceptKey && key != NO_KEY) {
      if (key == 'C') {   // 清除畫面
        inputCode = "";
        lineB = "PIN: ";
      } else if (key == 'D') {  // 比對輸入密碼
        acceptKey = false;
        checkPinCode();
      } else {
        inputCode += key;  // 儲存用戶的按鍵字元
        lineB = "PIN: " + inputCode;
      }
    }
      
    lastTime = timeRange[0] - (millis()/1000 - strTime);
    String str = String(lastTime);
    lineA = "Time... " + str  ;
      
    show_LCD();

    if(lastTime <= 0){
      acceptKey = false;
    }
       
  }

  gameOver();
  
}
