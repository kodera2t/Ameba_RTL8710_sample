/*

 This example  prints the Wifi shield's MAC address, and
 scans for available Wifi networks using the Wifi shield.
 Every ten seconds, it scans again. It doesn't actually
 connect to any network, so no encryption scheme is specified.

 Circuit:
 * WiFi shield attached

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 21 Junn 2012
 by Tom Igoe and Jaymes Dec
 */

#include <ESP_SSD1306.h>    
#include <Adafruit_GFX.h>   
#include <SPI.h>            
#include <Wire.h>           
#include <WiFi.h>

int screenMem[46]; //the implementation of frame buffer is referenced from Ben Heck's
int cursorX = 0;    //Retro BASIC computer's source
int checkChar = 0;

#define OLED_RESET  16  // Pin 15 -RESET digital signal
ESP_SSD1306 oled(OLED_RESET); // FOR I2C

// ASCII Characters
#define CR  '\r'
#define NL  '\n'
#define LF      0x0a
#define TAB '\t'
#define BELL  '\b'
#define SPACE   ' '
#define SQUOTE  '\''
#define DQUOTE  '\"'
#define CTRLC 0x1B  // Changed to ESC key (27 - 0x1B)
#define CTRLH 0x08
#define CTRLS 0x13
#define CTRLX 0x18

static char initmsg[]          PROGMEM = "Morse!";
static char wpmsg[]              PROGMEM = "Current wpm is ";
static char errormsg[]         PROGMEM = "WiFi shield not present";
static char searching[]        PROGMEM = "Scanning available networks...";
static char notfound[]         PROGMEM = "Couldn't get a wifi connection";
static char numnet[]           PROGMEM = "number of available networks:";

void setup() {
  //Initialize serial and wait for port to open:
 // Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
      oled.begin();
    //   init done
 //   oled.display();
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(WHITE);
    oled.setCursor(0,0);
  for (int xg = 0 ; xg < 46 ; xg++) {
    screenMem[xg] = 32;
  }

  doFrame(46);
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    printmsg(errormsg);
   // Serial.println("WiFi shield not present");
    
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") {
   // Serial.println("Please upgrade the firmware");
  }

  // Print WiFi MAC address:
  printMacAddress();
}

void loop() {
  // scan for existing networks:
  //Serial.println("Scanning available networks...");
  printmsg(searching);
  listNetworks();
  delay(10000);
}

void printMacAddress() {
  // the MAC address of your Wifi shield
  byte mac[6];

  // print your MAC address:
  WiFi.macAddress(mac);
 // Serial.print("MAC: ");
 // printmsg(maca);
//  lcdchar(mac[5], HEX);
//  Serial.print(":");
//  Serial.print(mac[4], HEX);
//  Serial.print(":");
//  Serial.print(mac[3], HEX);
//  Serial.print(":");
//  Serial.print(mac[2], HEX);
//  Serial.print(":");
//  Serial.print(mac[1], HEX);
//  Serial.print(":");
//  Serial.println(mac[0], HEX);
}

void listNetworks() {
  // scan for nearby networks:
  //Serial.println("** Scan Networks **");
  printmsg(searching);
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    printmsg(notfound);
   // Serial.println("Couldn't get a wifi connection");
    while (true);
  }

  // print the list of networks seen:
//  Serial.print("number of available networks:");
  printmsg(numnet);
  lcdChar(char(numSsid));

  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    //Serial.print(thisNet);
    //Serial.print(") ");
    //Serial.print(WiFi.SSID(thisNet));
    printmsg(WiFi.SSID(thisNet));
    //printmsg(WiFi.SSID(thisNet));
//    printmsg("\tSignal: ");
//    //Serial.print("\tSignal: ");
//    lcdChar((WiFi.RSSI(thisNet)));
//    printmsg(" dBm");
 //   printmsg("\tEncryptionRaw: ");
 //   printEncryptionTypeEx(WiFi.encryptionTypeEx(thisNet));
    printmsg("\tEncryption: ");
    printEncryptionType(WiFi.encryptionType(thisNet));
  }
}

void printEncryptionTypeEx(uint32_t thisType) {
  /*  Arduino wifi api use encryption type to mapping to security type.
   *  This function demonstrate how to get more richful information of security type.
   */
  switch (thisType) {
    case SECURITY_OPEN:
      printmsg("Open");
      break;
    case SECURITY_WEP_PSK:
      printmsg("WEP");
      break;
    case SECURITY_WPA_TKIP_PSK:
      printmsg("WPA TKIP");
      break;
    case SECURITY_WPA_AES_PSK:
      printmsg("WPA AES");
      break;
    case SECURITY_WPA2_AES_PSK:
      printmsg("WPA2 AES");
      break;
    case SECURITY_WPA2_TKIP_PSK:
      printmsg("WPA2 TKIP");
      break;
    case SECURITY_WPA2_MIXED_PSK:
      printmsg("WPA2 Mixed");
      break;
    case SECURITY_WPA_WPA2_MIXED:
      printmsg("WPA/WPA2 AES");
      break;
  }
}

void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      printmsg("WEP");
      break;
    case ENC_TYPE_TKIP:
      printmsg("WPA");
      break;
    case ENC_TYPE_CCMP:
      printmsg("WPA2");
      break;
    case ENC_TYPE_NONE:
      printmsg("None");
      break;
    case ENC_TYPE_AUTO:
      printmsg("Auto");
      break;
  }
}


static void outchar(unsigned char c)
{
    lcdChar(c);
}

static void lcdChar(unsigned int c) {
if(c <= 128){
  if (c != 13 and c != 10 and c != 8) { 
    screenMem[22 + cursorX] = c;
    cursorX += 1;
    if (cursorX < 11) {
    oled.setCursor(32+cursorX*6,48);   
    oled.write(c);
  //  filesave(c);  
    oled.display();
    }
    
  }
  
  if (cursorX == 11 or c == 10) {     
  
    for (int xg = 11 ; xg > 0 ; xg--) {

      screenMem[0 + xg] = screenMem[11 + xg];
      screenMem[11 + xg] = screenMem[22 + xg];    
//      screenMem[22 + xg] = screenMem[33 + xg];
//      screenMem[63 + xg] = screenMem[84 + xg];
//      screenMem[84 + xg] = screenMem[125 + xg]; 
        
//      screenMem[0 + xg] = screenMem[21 + xg];
//      screenMem[21 + xg] = screenMem[42 + xg];    
//      screenMem[42 + xg] = screenMem[63 + xg];
//      screenMem[63 + xg] = screenMem[84 + xg];
//      screenMem[84 + xg] = screenMem[125 + xg];   
//      screenMem[125 + xg] = screenMem[146 + xg];



//      screenMem[146 + xg] = 32;
    
            screenMem[22 + xg] = 32;
    }
  
    cursorX = 0;
    
    doFrame(22);  
    //            printwpm(wpm);

  }


}
}

static void doFrame(byte amount) {
        int xposi,yposi,yshift;
        oled.clearDisplay();
  for (int xg = 0 ; xg < amount ; xg++) {
                yshift=int(xg/11.0);
                yposi=yshift*8;
                xposi=(xg-yshift*10)*6;
          oled.setCursor(xposi+32,yposi+32);
    oled.write(screenMem[xg]);
  }
        oled.display();
 // printwpm(wpm);
}  

/***************************************************************************/
void printmsgNoNL(char *msg)
{
  while( pgm_read_byte( msg ) != 0 ) {
    outchar( pgm_read_byte( msg++ ) );
  };
}

/***************************************************************************/
void printmsg(char *msg)
{
  printmsgNoNL(msg);
  line_terminator();
}

static void line_terminator(void)
{
  outchar(NL);
  outchar(CR);
}

