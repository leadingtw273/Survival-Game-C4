#include "U8glib.h"

U8GLIB_ST7920_128X64_4X u8g(15, 14, 16);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
void draw() {
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0,10);
  u8g.print("Hello World!");
}

void setup(void) {
  u8g.getMode() == U8G_MODE_BW;
  u8g.setColorIndex(1);         // pixel on
}

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  delay(50);
}

