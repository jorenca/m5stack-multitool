#include <M5Stack.h>
#include "functionality.h"

class IRrc5Read : public Functionality {
  private:
    static const int INPUT_PIN = 2; // normal high

    static const int CODE_DISPLAY_TOP_PX = 50;
    static const int CODE_DISPLAY_HEIGHT_PX = 100;

    static const int ERROR_DISPLAY_TOP_PX = CODE_DISPLAY_TOP_PX + CODE_DISPLAY_HEIGHT_PX;
    static const int ERROR_DISPLAY_HEIGHT_PX = 70;

    void printSuccessfulRead(int code) {
      M5.Lcd.fillRect(0, CODE_DISPLAY_TOP_PX, 320, CODE_DISPLAY_HEIGHT_PX, TFT_BLACK);
      M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(0, CODE_DISPLAY_TOP_PX);
      
      for(int i=0; i<14; i++) {
        if (i==3) M5.Lcd.setTextColor(TFT_BLACK, TFT_YELLOW);
        if (i==8) M5.Lcd.setTextColor(TFT_GREEN, TFT_RED);
        M5.Lcd.printf("%d", (code & (1<<i)) != 0);
      }

      int address = 0;
      for(int bitPos=3; bitPos<8; bitPos++) { // code bit 4 is the most significant bit in the address, bit 8 is LSB
        address<<=1;
        address |= ((code & (1<<bitPos)) != 0);
      }
      M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
      M5.Lcd.print("\nAddress:");
      M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
      M5.Lcd.printf("%d\n", address);

      int command = 0;
      for(int bitPos=8; bitPos<14; bitPos++) { // code bit 9 is the most significant bit in the command, bit 14 is LSB
        command<<=1;
        command |= ((code & (1<<bitPos)) != 0);
      }
      M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
      M5.Lcd.printf("Command:");
      M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
      M5.Lcd.printf("%d", command);
    }

    void printError(const char* msg) {
      M5.Lcd.fillRect(0, ERROR_DISPLAY_TOP_PX, 320, ERROR_DISPLAY_HEIGHT_PX, TFT_BLACK);
      M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(0, ERROR_DISPLAY_TOP_PX);
      M5.Lcd.printf("%s\n", msg, bitpI);
      
      for(int i=0; i<bitpI; i++) M5.Lcd.printf("%d ", bitPieces[i]);
    }
    
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
        printError("Incorrect number of bitpieces");
        return;
      }
        
      int res = 0;
      for(int i=0; i<bitpI; i+=2) {
        if (bitPieces[i] == bitPieces[i+1]) {
          printError("Failed decoding bitpieces, same pair.");
          return;
         }
         res |= (bitPieces[i+1])<<(i/2);
      }

      if (res & 0b11 != 0b11) {
        printError("Failed checking first 2 bits");
        return;
      }

      printSuccessfulRead(res);
    }
   
  public:
    IRrc5Read() {
      pinMode(INPUT_PIN, INPUT);
    }

    void setup() {
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setTextColor(TFT_WHITE, TFT_BLUE);
      M5.Lcd.drawCentreString("== IR RC-5 Reader (pin 2 input active low) ==", 320/2, 0, 2);
    }

    void loop() {
      readNext();
    }
};
