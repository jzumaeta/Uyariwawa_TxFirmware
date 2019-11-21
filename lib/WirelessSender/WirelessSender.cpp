#include "WirelessSender.h"


#define WIFI_CHANNEL    1

WirelessSender::WirelessSender(/* args */)
{
}

WirelessSender::~WirelessSender()
{
}

void WirelessSender::Init()
{
    //Init ESP-NOW protocol
    if (esp_now_init() != 0) {
        Serial.println("[ERROR] Can't init ESP-Now! Restarting....\n\n");
        delay(3000);
        ESP.restart(); delay(1);
    }

    //MAC info
    Serial.print("[WS_INFO] Access Point MAC: ");
    Serial.println(WiFi.softAPmacAddress());
    Serial.print("[WS_INFO] Station MAC: "); 
    Serial.println(WiFi.macAddress());

    //Set remote peer MAC
    memcpy(remoteMac, "\x36\x33\x33\x33\x33\x33", 6);
    //Setting roles
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_add_peer(remoteMac, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0);

    //Sender callback
    esp_now_register_send_cb([](uint8_t* mac, uint8_t status) {
        uint8_t m[6];
        memcpy(m, mac, sizeof(m));
        Serial.printf("[WS_INFO] Slave MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
            m[0], m[1], m[2], m[3], m[4], m[5]);
        Serial.print("[STATUS] (0=0K - 1=ERROR): "); Serial.println(status);
    });
}

bool WirelessSender::SendToPeer(const char* msg, int len)
{
    int count=0, result=0;

    do {
        if (count>0) delay(5);
        result = esp_now_send(remoteMac, (u8*)msg, len);
        Serial.println("[WS_INFO] Send message to peer!\n");
    }
    while(result!=0 && ++count<3);

    //print message
    Serial.printf("Tx: %s - Status: %d\n", msg, result);
}