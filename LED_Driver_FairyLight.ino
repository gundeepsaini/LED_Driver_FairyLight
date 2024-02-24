/*-----------------------------------------

      LED Driver for Fairy Lights (USB x2)

Original Build Date: May 2023 (v01)
Device: NodeMcU v1.0 (ESP8266)
Block Diagram & Circuit Diagram: OneNote Workbook

Pin Connections:
  LED Strip 1   : D2 (4)
  LED Strip 1   : D1 (5)

Note:
  - Reset ESP after upload, disable IPv6, reset adapter, allow port in firewall
  - Navigate to "ip/OTA" for OTA through web portal
  


------------------------------------------- */


// ------------- LIB ------------------------------
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <Wire.h> 
#include <EEPROM.h>

#include "Secrets.h"



/* ------------- CONFIG VAR ------------------------------ */
unsigned long looptime_Fast = 0;    // in secs
unsigned long looptime_Mid1 = 2;    // in secs
unsigned long looptime_Mid2 = 10;   // in secs
unsigned long looptime_Slow       = 30;            // in secs
unsigned long looptime_VerySlow   = 15 *(60);      // in secs

#define LED_Strip_1 4 
#define LED_Strip_2 5  

#define External_Relay_1_pin 13  
#define External_Relay_2_pin 12

bool External_Relay_1_state=false;
bool External_Relay_2_state=false;



/* ------------- VAR ------------------------------ */
const char* OTA_Password     = SECRET_Device_OTA_PASS; 
const char* ssid             = SECRET_WIFI_SSID3;
const char* pass             = SECRET_WIFI_PASS3;
const char* DeviceHostName   = SECRET_Device_Name14;


unsigned long lastrun_fast, lastrun_Mid1;
unsigned long lastrun_Mid2, lastrun_slow, lastrun_Veryslow;
unsigned long lastrun_30mins,lastrun_OTA;
bool OTA_Mode=0;
String webpage1="";
ESP8266WebServer server(80);

boolean LED_State_1, LED_State_2;


//------------------------------------------------------------


void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.begin(115200);
  Serial.println(DeviceHostName);

  wifi_station_set_hostname(DeviceHostName);
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, pass);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Serial.println("Wifi Error! Rebooting in 30s...");
    delay(30 *1000);
    digitalWrite(LED_BUILTIN, HIGH);
    ESP.restart();
    while(1);
  }
  
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  WebServer_Config();
  OTA_Config();
  MQTT_config();
  LED_Config();
  config_relay();
  
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Ready");
}


void loop() 
{
  // Always
  Always_loop();

  // Fast Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_fast > looptime_Fast) || lastrun_fast ==0))
  {
    lastrun_fast = millis()/1000;
    Fast_Loop();
  }

  // Mid1 Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Mid1 > looptime_Mid1) || lastrun_Mid1 ==0))
  {
    lastrun_Mid1 = millis()/1000;
    Mid1_Loop();
  }

  // Mid2 Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Mid2 > looptime_Mid2) || lastrun_Mid2 ==0))
  {
    lastrun_Mid2 = millis()/1000;
    Mid2_Loop();
  }

  // Slow Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_slow > looptime_Slow) || lastrun_slow ==0))
  {
    lastrun_slow = millis()/1000;
    Slow_Loop();
  }

    // Very Slow Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Veryslow > looptime_VerySlow) || lastrun_Veryslow ==0))
  {
    lastrun_Veryslow = millis()/1000;
    VerySlow_Loop();
  }
}
