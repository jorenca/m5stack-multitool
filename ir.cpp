#include <M5Stack.h>
#include "functionality.h"

class IR : public Functionality {
  public:
    IR() {}

    void setup() {
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setTextColor(TFT_WHITE, TFT_BLUE);
      M5.Lcd.drawCentreString("== IR I/O ==", 320/2, 0, 2);
    }

    void loop() {

    }
};
