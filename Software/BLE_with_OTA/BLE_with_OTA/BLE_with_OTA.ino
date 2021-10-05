/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/

#include <BleKeyboard.h>

BleKeyboard bleKeyboard("theBrutzlerBLE", "theBrutzler", 100);

const int Col[] = {0, 2, 3, 4, 5, 9, 10, 12, 13, 14, 15, 18, 19, 21, 22, 23, 27, 32, 33};
const int Row[] = {25, 26, 34, 35, 36, 37, 38, 39};
int Key[sizeof(Col)*sizeof(Row)] = {};
int Key_Kommand[sizeof(Col)*sizeof(Row)] = {};//tbd};

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "secret";
const char* password = "topsecret";

bool OTA = false;
bool OTA_INIT = false;
bool OTA_STAGE_1 = false;
unsigned long first_try_millis;
unsigned long seacond_try_millis=0;
unsigned long firmware_update_timeout =0;
unsigned long timeout=2000;

void setup() {
   
  for (int i = 0; i < sizeof(Col)*sizeof(Row); i++) {
    Key_Kommand[i] = 0;
  }
 
  Key_Kommand[0] = KEY_UP_ARROW;
  Key_Kommand[1] = '?';
  Key_Kommand[3] = 187;// 0xF6;//'ö';
  Key_Kommand[4] = 236 ;//0x3C;//'<';
  Key_Kommand[5] = '1'; 
  Key_Kommand[6] = '4';
  Key_Kommand[7] = '7';
  Key_Kommand[19] = '?';
  Key_Kommand[21] = 'f';
  Key_Kommand[22] = KEY_F3;
  Key_Kommand[23] = '?';//KEY_MEDIA_EMAIL_READER;
  Key_Kommand[24] = KEY_LEFT_CTRL;
  Key_Kommand[40] = 'g';
  Key_Kommand[42] = 'm';
  Key_Kommand[43] = 'b';
  Key_Kommand[57] = ' '; 
  Key_Kommand[58] = KEY_LEFT_GUI;
  Key_Kommand[60] = '4';
  Key_Kommand[64] = '?';//KEY_MEDIA_LOCAL_MACHINE_BROWSER;
  Key_Kommand[76] = 'w';
  Key_Kommand[77] = '?';   
  Key_Kommand[78] = KEY_F11;
  Key_Kommand[79] = KEY_F1; 
  Key_Kommand[80] = '.';
  Key_Kommand[81] = KEY_INSERT; 
  Key_Kommand[82] = KEY_DELETE;
  Key_Kommand[83] = KEY_HOME;
  Key_Kommand[95] = 'e';
  Key_Kommand[96] = '?';
  Key_Kommand[97] = KEY_F12;
  Key_Kommand[98] = 'l';
  Key_Kommand[99] = 192;// 0x2D;//'-';
  Key_Kommand[100] = KEY_PAGE_DOWN; 
  Key_Kommand[101] = KEY_PAGE_UP; 
  Key_Kommand[102] = KEY_END;
  Key_Kommand[114] = KEY_TAB;
  Key_Kommand[115] = 'u';
  Key_Kommand[116] = KEY_F8;
  Key_Kommand[117] = 181;//0x2D;//'-';  
  Key_Kommand[118] = 'n';
  Key_Kommand[119] = 'h';
  Key_Kommand[120] = '8';
  Key_Kommand[121] = 183;// 0xFC;//'ü';
  Key_Kommand[133] = KEY_RIGHT_ARROW;
  Key_Kommand[134] = 192;//0x2D;//'-'; 
  Key_Kommand[135] = 't';
  Key_Kommand[136] = KEY_BACKSPACE;
  Key_Kommand[137] = KEY_CAPS_LOCK;
  Key_Kommand[138] = KEY_RETURN;
  Key_Kommand[140] = 184;// 0x2B;//'+';
  Key_Kommand[152] = KEY_DOWN_ARROW; 
  Key_Kommand[153] = 125 ;//0x2A;//'*';
  Key_Kommand[154] = 'r';
  Key_Kommand[155] = 'j';
  Key_Kommand[157] = '3';
  Key_Kommand[158] = '6';
  Key_Kommand[159] = '9';
  Key_Kommand[171] = 'a';
  Key_Kommand[174] = KEY_F4; 
  Key_Kommand[175] = KEY_RIGHT_GUI;
  Key_Kommand[190] = KEY_LEFT_ARROW; 
  Key_Kommand[191] = 38;//0x2F;//'/';
  Key_Kommand[192] = KEY_ESC;
  Key_Kommand[193] = 'k';
  Key_Kommand[194] = '0'; 
  Key_Kommand[195] = '2';
  Key_Kommand[196] = '5';  
  Key_Kommand[197] = '8'; 
  Key_Kommand[209] = 'q';
  Key_Kommand[210] = 'o'; 
  Key_Kommand[211] = KEY_F10;
  Key_Kommand[212] = KEY_F2;
  Key_Kommand[213] = ',';
  Key_Kommand[215] = '0';
  Key_Kommand[216] = 185 ;//0x23;//'#';
  Key_Kommand[230] = 'y';
  Key_Kommand[231] = '1';
  Key_Kommand[235] = KEY_RIGHT_ALT;
  Key_Kommand[249] = 'c'; 
  Key_Kommand[250] = '3';
  Key_Kommand[251] = '?';//KEY_MEDIA_PLAY_PAUSE; 
  Key_Kommand[252] = KEY_RIGHT_CTRL;
  Key_Kommand[268] = 'x';
  Key_Kommand[269] = '2';
  Key_Kommand[272] = KEY_RIGHT_SHIFT; 
  Key_Kommand[285] = 'd';
  Key_Kommand[286] = 'i'; 
  Key_Kommand[287] = KEY_F9;  
  Key_Kommand[288] = 61;//  0x60;//'´';---------------
  Key_Kommand[289] = KEY_RIGHT_GUI; 
  Key_Kommand[290] = 188;// 0xE4;//'ä';
  Key_Kommand[291] = '9';
  Key_Kommand[292] = 184;//0x2B;//'~';
  Key_Kommand[304] = 's';
  Key_Kommand[305] = 'z';
  Key_Kommand[306] = KEY_F7;
  Key_Kommand[307] = 96 ;//0x5E;//'^';---------------
  Key_Kommand[310] = '7';
  Key_Kommand[311] = 'p';
  Key_Kommand[323] = KEY_F5;
  Key_Kommand[325] = 'v'; 
  Key_Kommand[326] = '5';
  Key_Kommand[327] = '?';//KEY_MEDIA_WWW_SEARCH;
  Key_Kommand[330] = KEY_LEFT_ALT;
  Key_Kommand[342] = KEY_F6;
  Key_Kommand[344] = KEY_RETURN;
  Key_Kommand[345] = '6';
  Key_Kommand[348] = KEY_LEFT_SHIFT;

  bleKeyboard.begin();
    ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else // U_SPIFFS
          type = "filesystem";
  
        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        //Serial.println("Start updating " + type);
      })
      .onEnd([]() {
        //Serial.println("\nEnd");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        //Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) ;//Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) ;//Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) ;//Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) ;//Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) ;//Serial.println("End Failed");
        ESP.restart();
      });

  for (int i = 0; i < 19; i++) {
    pinMode(Col[i], OUTPUT);
    digitalWrite(Col[i], LOW);
  }
  for (int i = 0; i < 8; i++) {
    pinMode(Row[i], INPUT);
  }
}

// the loop routine runs over and over again forever:
void loop() {
  
  if (bleKeyboard.isConnected()) {
    for (int i = 0; i < 19; i++) {
      digitalWrite(Col[i], HIGH);
        delay(1);
        for (int y = 0; y < 8; y++) {
          if (digitalRead(Row[y])) {
           // Serial.print(Col[i]);
           // Serial.print(" + ");
           // Serial.print(Row[y]);
           // Serial.print(":True ");
            if (Key[i * 19 + y] == false) {
              bleKeyboard.press(Key_Kommand[i * 19 + y]);
              //Serial.print("Press: ");
              //Serial.println(Key_Kommand[i * 19 + y]);
              Key[i * 19 + y] = true;
            }
          }
          else {
            if (Key[i * 19 + y] == true) {
              bleKeyboard.release(Key_Kommand[i * 19 + y]);
              //Serial.print("Release: ");
              //Serial.println(Key_Kommand[i * 19 + y]);
              Key[i * 19 + y] = false;
            }
          }
          if(i==15&&y==4&&Key[i * 19 + y] == true){
            //Serial.println("KEY_RIGHT_GUI_Pressed");
            if(OTA_STAGE_1 == false){
              first_try_millis=millis();
              OTA_STAGE_1=true;
              //Serial.println("OTA_STAGE_1 SET");
            }
            seacond_try_millis=millis();
            if(OTA_STAGE_1==true&&((seacond_try_millis-first_try_millis)>=timeout)){
              //Serial.println("OTA SET");
              OTA=true;
            }
          }
          if(i==15&&y==4&&(Key[i * 19 + y] == false)&&(OTA_STAGE_1==true)){
            OTA_STAGE_1=false;
            //Serial.println("OTA_STAGE_1 RESET");
          }
        }
        digitalWrite(Col[i], LOW);
      }
  } 
  if(OTA==true){
    unsigned long timeout_if_no_update=60000; // 60 seconds
    firmware_update_timeout=millis();
    if((firmware_update_timeout-seacond_try_millis)>=timeout_if_no_update)
        ESP.restart();
    if(OTA_INIT==false){ 
      bleKeyboard.end();
      delay(1000);
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      delay(1000);
      while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        //Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
      }
      ArduinoOTA.begin();
      
      //Serial.println("Ready");
      //Serial.print("IP address: ");
      //Serial.println(WiFi.localIP());
      OTA_INIT=true;
    }
    ArduinoOTA.handle();
  }
}
