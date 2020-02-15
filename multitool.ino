#include <M5Stack.h>
#include "functionality.h"
#include "ir.cpp"
#include "wifi_hotspot.cpp"
#include "tone.cpp"

#define TEXT_HEIGHT 16

uint8_t selected = 0;
bool inSelection = false;

void setup() {
  M5.begin();
  
  M5.Lcd.setBrightness(200);    // BRIGHTNESS MAX 255
  M5.Lcd.fillScreen(BLACK);
  
  showMenu(selected);
}

Functionality* functions[] = { new WiFiHotspot(), new IR(), new Tone() };
const char* MENU_ITEMS[] = {"WiFi Hotspot", "IR I/O", "Tone"};
const int MENU_ITEMS_COUNT = 3;

void loop() {
  M5.update();
  if(inSelection) {
    functions[selected]->loop();
    return;
  }

  
  if(M5.BtnA.wasPressed() && selected > 0) {
    selected--;
    showMenu(selected);
  }

  if(M5.BtnC.wasPressed() && selected < MENU_ITEMS_COUNT-1) {
    selected++;
    showMenu(selected);
  }

  if(M5.BtnB.wasPressed()) {
    inSelection = true;
    functions[selected]->setup();
  }
}

void showMenu(uint8_t selected) {
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLUE);
  M5.Lcd.drawCentreString("== Menu ==", 320/2, 0, 2);

  //M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, TEXT_HEIGHT * 2);
  for(uint8_t i=0; i<MENU_ITEMS_COUNT; i++) {
    M5.Lcd.setTextColor(TFT_WHITE, i == selected ? TFT_NAVY : TFT_BLACK);
    M5.Lcd.printf("%i. %s\n", i, MENU_ITEMS[i]);
  }
}
