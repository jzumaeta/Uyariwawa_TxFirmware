#include <Arduino.h>
#include <ESP8266WiFi.h>
extern "C" {
  #include <espnow.h>
}


class WirelessSender
{
private:
  u8 remoteMac[6];

public:
    WirelessSender(/* args */);
    ~WirelessSender();
    void Init();
    bool SendToPeer(const char* msg, int len);
};