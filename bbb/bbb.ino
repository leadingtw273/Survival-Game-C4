#include <Keypad.h>
#include <Wire.h>
#include "U8glib.h"

#define KEY_ROWS 4  // 薄膜按鍵的列數
#define KEY_COLS 4   // 薄膜按鍵的行數

U8GLIB_ST7920_128X64_4X u8g(15, 14, 16);

// 設置按鍵模組
char keymap[KEY_ROWS][KEY_COLS] = {
  {'F', 'E', 'D', 'C'},
  {'B', '3', '6', '9'},
  {'A', '2', '5', '8'},
  {'0', '1', '4', '7'}
};

byte rowPins[KEY_ROWS] = {6, 7, 8, 9}; //Rows 0 to 3
byte colPins[KEY_COLS] = {2, 3, 4, 5}; //Columns 0 to 3

Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, KEY_ROWS, KEY_COLS);

String passcode = "1234";   // 預設密碼
String inputCode = "";      // 暫存用戶的按鍵字串
bool acceptKey = true;      // 代表是否接受用戶按鍵輸入的變數，預設為「接受」

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
  do{
    draw("PIN:","");
  } while( u8g.nextPage() );
  acceptKey = true;
  inputCode = "";
  setInput();
}

// 比對用戶輸入的密碼
void checkPinCode() {
  acceptKey = false;  // 暫時不接受用戶按鍵輸入
  // 比對密碼
  if (inputCode == passcode) {
    Serial.println("\nWelcome home!");
    do{
      draw("[success]", "Welcome home!");
    } while( u8g.nextPage() );
    delay(3000);
    acceptKey = true;
    setPassword();
  } else {
    Serial.println("\n***WRONG!!***");
    do{
      draw("PIN:","***WRONG!!***");
    } while( u8g.nextPage() );
    delay(500);
    resetLocker();
  }    
}

void setPassword(){
  passcode = "";
  Serial.print("Input passwoad: ");
  do{
    draw("Input passwoad:","");
  } while( u8g.nextPage() );
  while(acceptKey){
    char key = keypad.getKey();
    if (key != NO_KEY) {
      if (key == 'C') {   // 清除畫面
        passcode = "";
        Serial.println();
        do{
          draw("Input passwoad:","");
        } while( u8g.nextPage() );
      } else if (key == 'D') {  // 確定輸入密碼
        Serial.println("\npassword set!!");
        do{
          draw("password set!!","");  
        } while( u8g.nextPage() );
        acceptKey = false;
      } else {
        passcode += key;  // 儲存用戶的密碼
        Serial.print(key);
        do{
          draw("Input passwoad:",passcode);  
        } while( u8g.nextPage() );
      }
    }
  }  
  resetLocker();
}

void setInput(){
    while(acceptKey){
      
      char key = keypad.getKey();
      if (key != NO_KEY) {
        if (key == 'C') {   // 清除畫面
          inputCode = "";
          Serial.println();
          do{
            draw("PIN:","");
          } while( u8g.nextPage() );
        } else if (key == 'D') {  // 比對輸入密碼
          checkPinCode();
        } else {
          inputCode += key;  // 儲存用戶的按鍵字元
          Serial.print(key);
          do{
            draw("PIN:",inputCode);
          } while( u8g.nextPage() );
        }
      }
      
    }
}

void setup() {
  Serial.begin(9600);
  u8g.getMode() == U8G_MODE_BW;
  u8g.setColorIndex(1);         // pixel on
  u8g.firstPage();  
  
  setPassword();
}

void loop() {
  
}
