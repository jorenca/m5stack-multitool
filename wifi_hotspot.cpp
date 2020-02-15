#include <M5Stack.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include "functionality.h"


class WiFiHotspot : public Functionality {
  private:
  
    IPAddress apIP;
    static constexpr char* apSSID = "TP-LINK_D9D9A0";
    static constexpr char* apPwd = "hello123";

    void softAPPrintStations() {
      wifi_sta_list_t wifi_sta_list;
      tcpip_adapter_sta_list_t adapter_sta_list;
      ESP_ERROR_CHECK(esp_wifi_ap_get_sta_list(&wifi_sta_list));
      tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);
    
      M5.Lcd.printf("Clients: %d\n\n", adapter_sta_list.num);
      for(int i = 0; i < adapter_sta_list.num; i++) {
        tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
        M5.Lcd.printf("%d - IP: %s\n", i + 1, ip4addr_ntoa(&(station.ip)));
      }
    }
  
  public:
    WiFiHotspot() {
      apIP = IPAddress(192, 168, 1, 1);
    }

    void setup() {
      M5.Lcd.print("Starting AP...");

      WiFi.mode(WIFI_MODE_STA);
      WiFi.disconnect();
      delay(100);
      WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(apSSID, apPwd, 3, false);
      WiFi.mode(WIFI_MODE_AP);
      // WiFi.softAPConfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet);
      // WiFi.softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0);
      // dnsServer.start(53, "*", apIP);
      
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setTextColor(TFT_WHITE, TFT_BLUE);
      M5.Lcd.drawCentreString("== WiFi Hotspot ==", 320/2, 0, 2);
    }

    void loop() {
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setTextColor(TFT_WHITE, TFT_BLUE);
      M5.Lcd.drawCentreString("== WiFi Hotspot ==", 320/2, 0, 2);
      
      M5.Lcd.setCursor(0, 20);
      M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
      M5.Lcd.printf("AP Name: %s\n", apSSID);
      M5.Lcd.printf("AP Password: %s\n", apPwd);
      softAPPrintStations();
      delay(2000);
    }
};
