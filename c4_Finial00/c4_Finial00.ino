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

byte rowPins[KEY_ROWS] = {2, 3, 4, 5}; //Rows 0 to 3
byte colPins[KEY_COLS] = {6, 7, 8, 9}; //Columns 0 to 3

Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, KEY_ROWS, KEY_COLS);

String passcode = "1234";     // 預設密碼
String inputCode = "";        // 暫存用戶的按鍵字串
bool acceptKey = true;        // 代表是否接受用戶按鍵輸入的變數，預設為「接受」
String lineA = "",lineB = ""; // 顯示內容

//計時
//1,2,3,5,8,13,21,34,55,89
int timeRange[] = {10,20,40,80,160,320,640,1280,2560,5120};
int timeBee[]   = {125,250,500,1000,2000,3000,5000,8000,13000,21000,34000};
int total = (int)sizeof(timeRange)/4;
int strTime = 0;
int totalTime = 0;
int clockTime = 0;
int tempTime = 0;
int lastTime = 0; 
String settime = "";

byte beePin = 13;  //蜂鳴器腳位

//蜂鳴器
void bee(){
  digitalWrite(beePin,HIGH);
  myDelay(500);
  digitalWrite(beePin,LOW);
}
void bee(int d){
  digitalWrite(beePin,d);
}

//設定密碼
void setPassword(){
  char key = '\0';
  passcode = "";
  lineA = "Setting passwoad";
  lineB = "PIN:";
  while(acceptKey){
    key = keypad.getKey();
    Serial.println(key);
    if (acceptKey && key != NO_KEY) {
      clear_LCD();
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
    delay(50);
  } 
  delay(1000);
  acceptKey = true;
  setTimeRange();
}

//設定時間寬度
void setTimeRange(){
  settime = "";
  lineA = "set time...      ";
  lineB = "SEC:";
  while(acceptKey){
    char key = keypad.getKey();
    if(acceptKey && key != NO_KEY){
      clear_LCD();
      if (key == 'C') {   // 清除畫面
        settime = "";
        lineB = "SEC:";
      } else if (key == 'D') {  // 確定輸入時間
        tempTime = millis();
        strTime = millis()/1000;
        totalTime = settime.toInt();
        lineA = "Time set!";
        lineB = "";
        acceptKey = false;
      } else {
        settime += key;  // 儲存時間數字
        lineB = "SEC: " + settime;
      }
    }
    show_LCD();
    delay(50);
  }
  delay(1000);
  
  resetLocker();
  
}

void keyINPass(){
  
    char key = keypad.getKey();
    // 若目前接受用戶輸入，而且有新的字元輸入…
    if (acceptKey && key != NO_KEY) {
      clear_LCD();
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
    
    lastTime = totalTime - (millis()/1000 - strTime);
        
    String str = String(lastTime);
    lineA = "Time... " + str + " "  ;
    
    show_LCD();
    
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
    delay(250); 
    clear_LCD();
    resetLocker();
  }    
}

//密碼通過顯示
void pass(){
  digitalWrite(beePin,LOW);
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
    delay(50);
  }
  clear_LCD();
  acceptKey = true;
  setPassword();
}

 //遊戲結束
void gameOver(){
  lineA = "time out!!!!!";
  lineB = "fail.........";
  bee(1);
  while(acceptKey){
    char key = keypad.getKey();
    if(acceptKey && key != NO_KEY){
      if(key == 'D'){
          bee(0);
          acceptKey = false;
      }
    }
    show_LCD();
    delay(50);
  }
  clear_LCD();
  acceptKey = true;
  setPassword();
}

void clear_LCD(){
  lcd.setCursor(0, 0); // 設定游標位置在第一行行首
  lcd.print("                               ");
  lcd.setCursor(0, 1); // 設定游標位置在第二行行首
  lcd.print("                               ");
}

void show_LCD(){
  lcd.setCursor(0, 0); // 設定游標位置在第一行行首
  lcd.print(lineA);
  lcd.setCursor(0, 1); // 設定游標位置在第二行行首
  lcd.print(lineB);
}

void myDelay(unsigned long duration){
  unsigned long start = millis();
  
  while(((millis() - start) <= duration) && acceptKey){
    keyINPass();
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(beePin,OUTPUT);
  
  lcd.init();// initialize the lcd 
   
  setPassword();
}

void loop() {

  Serial.println("lastTime: " + lastTime);
  Serial.println("clockTime: " + clockTime);
    
  while(acceptKey){
  
    char key = keypad.getKey();
    // 若目前接受用戶輸入，而且有新的字元輸入…
    if (acceptKey && key != NO_KEY) {
      clear_LCD();
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
      
    lastTime = totalTime - (millis()/1000 - strTime);
    
    for(int x = 9 ; x >= 0; x--){
      if(lastTime > timeRange[x]){
        clockTime = timeBee[x+1];
        break;
      }else if(lastTime < timeRange[0]){
        clockTime = timeBee[0];
      }
    }
    
    int timeDiff = millis() - tempTime ;
    if(timeDiff >= clockTime){
      bee();
      tempTime = millis();
    }
    
    String str = String(lastTime);
    lineA = "Time... " + str + " "  ;
      
    show_LCD();

    if(lastTime <= 0){
      acceptKey = false;
    }
       
  }
  acceptKey = true;
  gameOver();
  
}
