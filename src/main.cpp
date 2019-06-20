#include <Arduino.h>
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"
#include <ESP8266WiFi.h>
#include <Ticker.h>
extern "C" {
  #include <espnow.h>
}

#define WIFI_CHANNEL 1

//MAC ADDRESS OF THE DEVICE YOU ARE SENDING TO
byte remoteMac[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x33};


#define SYSTEM_LED        D0    //GREEN_LED
#define STATUS_LED        D5    //BLUE LED
#define USER_SWITCH       D6

/**        
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/
VR myVR(D2, D1);    // 2:RX 3:TX, you can choose your favourite pins.
uint8_t buf[64];



#define auxilio       (0)
#define mantente      (1) 
#define tranquilo     (2)
#define detente       (3)
#define cuidado       (4)
#define malestar      (5)
#define mirame        (6)
/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized. 
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/
void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

Ticker tk_sys;

void systemled()
{
  if (digitalRead(SYSTEM_LED) == 0) {
    digitalWrite(SYSTEM_LED, HIGH);
    tk_sys.attach_ms(200, systemled);
  }
  else {
    digitalWrite(SYSTEM_LED, LOW);
    tk_sys.attach_ms(1000, systemled);
  }
}

void setup()
{
  /** initialize */
  myVR.begin(9600);
  
  Serial.begin(115200);
  Serial.println("Reconocimiento de Voz - Proyecto UYARIWAWA");
    
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  } else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    //while(1);
  }
  
  if(myVR.load((uint8_t)auxilio) >= 0){
    Serial.println("Auxilio-activado");
  }
  if(myVR.load((uint8_t)mantente) >= 0){
    Serial.println("Mantente-activado");
  }
  if(myVR.load((uint8_t)tranquilo) >= 0){
    Serial.println("Tranquilo-activado");
  }
  if(myVR.load((uint8_t)detente) >= 0){
    Serial.println("Detente-activado");
  }
  if(myVR.load((uint8_t)cuidado) >= 0){
    Serial.println("Cuidado-activado");
  }
  if(myVR.load((uint8_t)malestar) >= 0){
    Serial.println("Malestar-activado");
  }
  if(myVR.load((uint8_t)mirame) >= 0){
    Serial.println("Mirame-activado");
  }

  //Enable ESP-NOW
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  Serial.print("\r\n\r\nDevice MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.println("\r\nESP_Now Controller.");
  esp_now_init();
  delay(10);
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(remoteMac, ESP_NOW_ROLE_COMBO, WIFI_CHANNEL, NULL, 0);
  //GPIOs
  pinMode(SYSTEM_LED, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  //sys
  tk_sys.attach_ms(1000, systemled);
}

Ticker tk;
void turnoffcallback()
{
  digitalWrite(STATUS_LED, LOW);
}

void send_to_peer(const char* msg)
{
  byte result = 0x00;
  for (int i=0; i<3; i++) {
    result = esp_now_send(remoteMac, (u8*)msg, strlen(msg));
    if (result==0) {
      break;
    }
  }

  //blink led
  if (result == 0) digitalWrite(STATUS_LED, HIGH);
  tk.once_ms(250, turnoffcallback);
  //print message
  Serial.print("\nTx: "); Serial.println(msg);
  Serial.print("Status: "); Serial.println(result);
}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);

  if(ret > 0) {
    switch(buf[1]){
      case auxilio:
        send_to_peer("auxilio");
        break;
      case mantente:
        send_to_peer("mantente");
        break;
      case tranquilo:
        send_to_peer("tranquilo");
        break;
      case detente:
        send_to_peer("detente");
        break;
      case cuidado:
        send_to_peer("cuidado");
        break;
      case malestar:
        send_to_peer("malestar");
        break;
      case mirame:
        send_to_peer("mirame");
        break;
      default:
        Serial.println("Funcion no Grabada");
        break;
    }
    /** voice recognized */
    printVR(buf);
  }
}