///Ameba currency display//
/// This program is greatly based on the sketch released at
/// http://blog.boochow.com/article/425016458.html (written in Japanese)
/// Added several description for RTL8710 

#include <ESP_SSD1306.h>    
#include <Adafruit_GFX.h>   
#include <SPI.h>            
#include <Wire.h>
//#include <WiFiUdp.h>           
#include <WiFi.h>
#include <ArduinoJson.h>

#define WIFI_SSID "yourssid"
#define WIFI_PSK "yourpassword"
#define DEST_HOST "api.fixer.io"
#define DEST_PORT 80
#define DEST_URL "/latest?base=USD&symbols=JPY" 
#define DEST_URL2 "/latest?base=CAD&symbols=JPY"

//sample json data used in this sketch
// {"base":"USD","date":"2015-08-28","rates":{"JPY":120.84}}

#define OLED_RESET  16  // Pin 15 -RESET digital signal
ESP_SSD1306 oled(OLED_RESET); // FOR I2C
  WiFiClient client;
  const int BUFFER_SIZE = JSON_OBJECT_SIZE(4) + JSON_ARRAY_SIZE(1);
  
//ST7032 lcd;

void setup() {
  //int count;
  //for(count=1;count<3;count++){
  int count=1;

  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  //if(count==1){
    //oled.begin();
      oled.begin(SSD1306_SWITCHCAPVCC); 
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(WHITE);
    oled.setCursor(0,0);

  delay(100);

  WiFi.begin(WIFI_SSID, WIFI_PSK);
    String line=""; 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    oled.setCursor(0,0);
    oled.print(".");
    //Serial.print(".");
    oled.display();
  }
  oled.setCursor(0,8);
  oled.println("Connected");
  oled.display();

   




  if (!client.connect(DEST_HOST, DEST_PORT)) {
      oled.setCursor(32,32);
  oled.setCursor(0,8);
  oled.println("failed");
  oled.display();
  //  Serial.println("connection failed");
    return;
  }

  if(count==1){
  client.print(String("GET ") + DEST_URL + " HTTP/1.1\r\n" +
               "Host: " + DEST_HOST + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  }else{
   client.print(String("GET ") + DEST_URL2 + " HTTP/1.1\r\n" +
               "Host: " + DEST_HOST + "\r\n" + 
               "Connection: close\r\n\r\n");   
  delay(500);
  }

  //get rid of the HTTP headers
  while(client.available()){
    line = client.readStringUntil('\r');
    //Serial.print(line);
    line.trim();
    if (line.length() == 0) {
      break;
    }
  }

  //get http content
  String buffer="";
  while(client.available()){
    line = client.readStringUntil('\r');
    line.trim();
    buffer.concat(line);
  }

  //parse json data
  char json[buffer.length() + 1];
  buffer.toCharArray(json, sizeof(json));
  //Serial.println(json);
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
  oled.setCursor(0,16);
  oled.println("p-failed");    
    return;
  }
  const char* date = root["date"];
  //Serial.println(date);
  const char* base = root["base"];
  //Serial.println(base);
  JsonObject& rates = root["rates"];
  //rates.printTo(Serial);
  //Serial.println();
  const char* rate = rates["JPY"];
  //Serial.println(rate);
  //Serial.println();
  


  oled.fillRect(0,0,128,32,BLACK);
  //oled.clearDisplay();
  oled.setCursor(0,0);
  //oled.print(date);
  //oled.print(" UTC");
  oled.setTextSize(2);    
  oled.setCursor(0,0);
  oled.print("JPY/");
  oled.print(base);
  oled.print(": ");
  oled.setCursor(0,16);
  oled.setTextSize(3);  
  oled.print(rate);
  oled.display();
  client.stop();
}

void loop() {
}

