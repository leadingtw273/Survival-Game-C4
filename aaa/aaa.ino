#include <Keypad.h>
#include <Wire.h>

#define KEY_ROWS 4  // 薄膜按鍵的列數
#define KEY_COLS 4   // 薄膜按鍵的行數

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


// 顯示「歡迎光臨」後，重設LCD顯示文字和輸入狀態。
void resetLocker() {
  Serial.print("PIN:");

  acceptKey = true;
  inputCode = "";
}

// 比對用戶輸入的密碼
void checkPinCode() {
  acceptKey = false;  // 暫時不接受用戶按鍵輸入
  // 比對密碼
  if (inputCode == passcode) {
    Serial.println("\nWelcome home!");
    delay(3000);
    acceptKey = true;
    setPassword();
  } else {
    Serial.println("\n***WRONG!!***");
    resetLocker();
  }    
}

void setPassword(){
  passcode = "";
  Serial.print("Input passwoad: ");
  while(acceptKey){
    char key = keypad.getKey();
    if (acceptKey && key != NO_KEY) {
      if (key == 'C') {   // 清除畫面
        passcode = "";
        Serial.println();
      } else if (key == 'D') {  // 確定輸入密碼
        Serial.println("\npassword set!!");
        acceptKey = false;
      } else {
        passcode += key;  // 儲存用戶的密碼
        Serial.print(key);
      }
    }
  }  
  resetLocker();
}

void setup() {
  Serial.begin(9600);
  setPassword();
}

void loop() {
  char key = keypad.getKey();

  // 若目前接受用戶輸入，而且有新的字元輸入…
  if (acceptKey && key != NO_KEY) {
    if (key == 'C') {   // 清除畫面
      inputCode = "";
      Serial.println();
    } else if (key == 'D') {  // 比對輸入密碼
      checkPinCode();
    } else {
      inputCode += key;  // 儲存用戶的按鍵字元
      Serial.print("*");
    }
  }
}
