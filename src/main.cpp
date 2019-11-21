#include <Arduino.h>
#include <SoftwareSerial.h>
#include "VRLib/VoiceRecognitionV3.h"
#include <ESP8266WiFi.h>
#include <Ticker.h>
extern "C" {
  #include <espnow.h>
}
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


/**        
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/
VR myVR(D7, D6);    // 2:RX 3:TX, you can choose your favourite pins.
uint8_t buf[64];

enum words_t {
  auxilio = 0,
  mantente = 1,
  tranquilo = 2,
  detente = 3,
  cuidado = 4,
  malestar = 5,
  mirame = 6
};

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
  for(int i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.printf("[%02X]", buf[i]);
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

void setup()
{ 
  delay(500);
  Serial.begin(115200);
  Serial.println("\n\nProyecto UYARIWAWA");
  Serial.println(    "------------------\n");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //default -> 0x3C OR 0x3D
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Uyariwawa Device"));
  display.println(F("-----------------"));
  display.println(
      "\nCompilation:\n"
      "\nDate: " __DATE__
      "\nTime: " __TIME__);
  display.display();
  delay(3000);
    
  //Init Module
  myVR.begin(9600);
  myVR.flush();
  delay(100);
  if (myVR.clear() == 0) {
    Serial.println("Recognizer cleared!");
  } else {
    Serial.println("Not find VoiceRecognitionModule!");
    Serial.println("Please check connection and restart Arduino.");
    display.clearDisplay();
    display.setTextSize(1); 
    display.setCursor(0,0); 
    display.println(F("Not find VoiceRecognitionModule!"));
    display.display();
    delay(5000);
    ESP.restart(); delay(1);
  }
  
  //Activate commands
  if (myVR.load((uint8_t)auxilio) >= 0) Serial.println("Auxilio-activado");
  if (myVR.load((uint8_t)mantente) >= 0) Serial.println("Mantente-activado");
  if (myVR.load((uint8_t)tranquilo) >= 0) Serial.println("Tranquilo-activado");
  if (myVR.load((uint8_t)detente) >= 0) Serial.println("Detente-activado");
  if (myVR.load((uint8_t)cuidado) >= 0) Serial.println("Cuidado-activado");
  if (myVR.load((uint8_t)malestar) >= 0) Serial.println("Malestar-activado");
  if (myVR.load((uint8_t)mirame) >= 0) Serial.println("Mirame-activado");

  display.clearDisplay();
  display.setTextSize(2); 
  display.setCursor(0,0); 
  display.println(F("Ready to\nreceive\ncommands...."));
  display.display();
}

String inputString = "";

void loop()
{
  if (myVR.recognize(buf) > 0) {
    display.clearDisplay();
    display.setTextSize(1); 
    display.setCursor(0,0); 

    switch(buf[1]){
      case auxilio:
        display.println(F("Message: auxilio"));
        send_to_peer("auxilio");
        break;
      case mantente:
        display.println(F("Message: mantente"));
        send_to_peer("mantente");
        break;
      case tranquilo:
        display.println(F("Message: tranquilo"));
        send_to_peer("tranquilo");
        break;
      case detente:
        display.println(F("Message: detente"));
        send_to_peer("detente");
        break;
      case cuidado:
        display.println(F("Message: cuidado"));
        send_to_peer("cuidado");
        break;
      case malestar:
        display.println(F("Message: malestar"));
        send_to_peer("malestar");
        break;
      case mirame:
        display.println(F("Message: mirame"));
        send_to_peer("mirame");
        break;
      default:
        display.println(F("Message: Invalido!"));
        Serial.println("Funcion no Grabada");
        break;
    }
    /** voice recognized */
    printVR(buf);
    display.display();
  }
}