#include <M5Stack.h>
#include "functionality.h"

class PlotAnalogIn : public Functionality {
  private:
    static const int INPUT_PIN = 2;

    bool isSingleShot = false;
    bool isRunning = true;

    int cycleSleep = 0;
  
    static const int MARGIN_TOP_PX = 20;
    static const int GRAPH_HEIGHT = 200;
    static const int GRAPH_Y_BOTTOM_PX = GRAPH_HEIGHT + MARGIN_TOP_PX;
    static const int TOOLTIPS_Y_PX = MARGIN_TOP_PX + GRAPH_HEIGHT;
    static const int GRID_COLOR = 0b0001100001100011; //R 5bits, G 6bits, B 5bits
    
    int xFrame = 0;
    
    void drawNextFrame(int value) {
      int x = xFrame++;
      int y = MARGIN_TOP_PX + GRAPH_HEIGHT * ((4095 - value) / 4096.0);
      
      M5.Lcd.fillRect(x, MARGIN_TOP_PX, 1, GRAPH_HEIGHT, TFT_BLACK);
      if (!isSingleShot) M5.Lcd.fillRect(x + 1, MARGIN_TOP_PX, 2, GRAPH_HEIGHT, TFT_RED); // faster in single shot
      
      M5.Lcd.drawPixel(x, GRAPH_Y_BOTTOM_PX - GRAPH_HEIGHT/5, GRID_COLOR);
      M5.Lcd.drawPixel(x, GRAPH_Y_BOTTOM_PX - GRAPH_HEIGHT/5*2, GRID_COLOR);
      M5.Lcd.drawPixel(x, GRAPH_Y_BOTTOM_PX - GRAPH_HEIGHT/5*3, GRID_COLOR);
      M5.Lcd.drawPixel(x, GRAPH_Y_BOTTOM_PX - GRAPH_HEIGHT/5*4, GRID_COLOR);
      
      M5.Lcd.drawPixel(x, y, TFT_WHITE);
    }

    void updateTooltips() {
      // button tooltips
      M5.Lcd.fillRect(0, TOOLTIPS_Y_PX, 320, 20, TFT_BLACK);
      M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(20, TOOLTIPS_Y_PX);
      M5.Lcd.printf("Sleep %d", cycleSleep);
      M5.Lcd.setCursor(140, TOOLTIPS_Y_PX);
      M5.Lcd.print("Arm");
      M5.Lcd.setCursor(220, TOOLTIPS_Y_PX);
      M5.Lcd.print(isSingleShot ? "Single" : "Cont");
    }
    
  public:
    PlotAnalogIn() {
      
      pinMode(INPUT_PIN, INPUT);
    }

    void setup() {
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setTextColor(TFT_WHITE, TFT_BLUE);
      M5.Lcd.drawCentreString("Plotting pin 2 (0-5V)", 320/2, 0, 2);
      updateTooltips();
    }

    void loop() {
      if (isRunning) drawNextFrame(analogRead(INPUT_PIN));

      if (isSingleShot && xFrame == 320) {
        isRunning = false;
      }

      if(M5.BtnA.wasPressed()) {
        cycleSleep++;
        if (cycleSleep == 11) cycleSleep = 20;
        if (cycleSleep == 21) cycleSleep = 100;
        if (cycleSleep == 101) cycleSleep = 1000;
        cycleSleep%=1001;
        updateTooltips();
      }

      if(M5.BtnB.wasPressed()) {
        isRunning = true;
      }

      if(M5.BtnC.wasPressed()) {
        isSingleShot = !isSingleShot;
        updateTooltips();
      }
      xFrame %= 320;
      delay(cycleSleep);
    }
};
