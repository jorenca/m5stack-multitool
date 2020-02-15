#include <M5Stack.h>
#include "functionality.h"

class Tone : public Functionality {
  private:
    bool isActive = false;
    int frequency = 1000; // Hz
    int pressingUpSinceCycles = 0;
    int pressingDownSinceCycles = 0;

    void printStatus() {
      M5.Lcd.fillRect(0, 100, 320, 100, TFT_BLACK);
      M5.Lcd.setCursor(0, 100);

      M5.Lcd.printf("Active? %s\n", isActive ? "ON" : "OFF");
      M5.Lcd.printf("Frequency %d Hz\n", frequency);
    }

    void updateTone() {
      if (isActive) M5.Speaker.tone(frequency);
      else M5.Speaker.mute();
    }

  public:
    Tone() {}

    void setup() {
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setTextColor(TFT_WHITE, TFT_BLUE);
      M5.Lcd.drawCentreString("== Tone ==", 320/2, 0, 2);

      // button tooltips
      M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(55, 220);
      M5.Lcd.print("F+");
      M5.Lcd.setCursor(125, 220);
      M5.Lcd.print("On/Off");
      M5.Lcd.setCursor(240, 220);
      M5.Lcd.print("F-");

      // set normal text properties
      M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
      M5.Lcd.setTextSize(2);
      
      printStatus();
    }

    void loop() {
      if(M5.BtnA.isPressed() && frequency < 12000) {
        pressingUpSinceCycles++;  
        delay(10);
      
        frequency += pressingUpSinceCycles;
        updateTone();
        printStatus();
      } else pressingUpSinceCycles = 0;

      if(M5.BtnB.wasPressed()) {
        isActive = !isActive;
        updateTone();
        printStatus();
      }

      if(M5.BtnC.isPressed() && frequency > 1) {
        pressingDownSinceCycles++;
        delay(10);
        
        frequency -= pressingDownSinceCycles;
        if (frequency < 1) frequency = 1;
        updateTone();
        printStatus();
      } else pressingDownSinceCycles = 0;
    }
};
