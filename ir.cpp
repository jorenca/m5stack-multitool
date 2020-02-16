#include <M5Stack.h>
#include "functionality.h"

class IR : public Functionality {
  private:
    static const int INPUT_PIN = 2; // normal high

    int cc = 0;
    
    bool bitPieces[100];
    int bitpI = 0;

    void readNext() {
      
      bitpI = 0;
      while(digitalRead(INPUT_PIN)); // wait for fall
      bitPieces[bitpI++] = 0; // always starts with 0
      
      while(bitpI < 95) {
        // experimentally, up and down times are around 4000-5800 for shorts and 10000-11000 for longs
        int rc = 0;
        for(rc=0; (rc < 200 || digitalRead(INPUT_PIN) == 0) && rc < 100000; rc++);
        if (rc <= 3600) { break; }
        if (rc > 3800 && rc < 7500) bitPieces[bitpI++] = 1;
        else if (rc > 7500) { bitPieces[bitpI++] = 1; bitPieces[bitpI++] = 1; }
        
        int dc = 0;
        for(dc=0; (dc < 200 || digitalRead(INPUT_PIN) == 1) && dc < 100000; dc++);
        if (dc <= 3800) { break; }
        if (dc > 3800 && dc < 7500) bitPieces[bitpI++] = 0;
        else if (dc >= 7500 && dc < 13000) { bitPieces[bitpI++] = 0; bitPieces[bitpI++] = 0; }
        else if (dc >= 13000) {
          bitPieces[bitpI++] = 0;
          break;
        }
      }

      if (bitpI != 28 && bitpI != 29) {
        M5.Lcd.printf("Failed reading, bitpI %d\n", bitpI);
        
        for(int i=0; i<bitpI; i++) M5.Lcd.printf("%d ", bitPieces[i]);
        return;
      }
        
      int res = 0;
      for(int i=0; i<bitpI; i+=2) {
        if (bitPieces[i] == bitPieces[i+1]) {
          M5.Lcd.print("Failed decoding bitpieces, same pair.\n");
          return;
         }
         res |= (bitPieces[i+1])<<(i/2);
      }

      if (res & 0b11 != 0b11) {
        M5.Lcd.printf("Failed checking first 2 bits. %d\n", res);
        for(int i=0; i<bitpI; i++) M5.Lcd.printf("%d ", bitPieces[i]);
        return;
      }
      res &= 0b11111111111000; // erase starting bits and toggle bit
      M5.Lcd.printf("SUCCESS! %d\n", res);
    }
   
  public:
    IR() {
      pinMode(INPUT_PIN, INPUT);
    }

    void setup() {
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setTextColor(TFT_WHITE, TFT_BLUE);
      M5.Lcd.drawCentreString("== IR I/O ==", 320/2, 0, 2);
    }

    void loop() {
      if (cc%20 == 0) {
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(0, 0);
      }
      readNext();
      cc++;
    }
};
