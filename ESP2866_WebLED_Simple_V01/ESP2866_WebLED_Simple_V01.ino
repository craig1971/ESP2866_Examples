
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//
// ESP8266 Web Server / LED Driver Example : https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/example-sketch-ap-web-server
// Connecting with WPA:  https://www.arduino.cc/en/Tutorial/ConnectWithWPA
//


/////////////////////
// Pin Definitions //
/////////////////////
const int LED_PIN = D7;

int ledState = LOW;


ESP8266WebServer server(80);

//
// Setup WiFi Configuration
//
void setupWiFi()
{
  WiFi.begin("VaporNet","Get49han");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected! Open http://");
  Serial.print(WiFi.localIP());
  Serial.println(" in your browser");
}

//
// Setup Hardware Configurations
//
void setupHardware()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

//
// Setup HTTP Server
//
void setupHttpServer()
{

  server.begin();
  
  //
  // LED OFF Methods
  //
  server.on("/ledoff", HTTP_GET, []() {
    setLedOff();
  });
  
  server.on("/ledoff", HTTP_OPTIONS, []() {
    server.sendHeader("access-control-allow-credentials", "false");
    server.sendHeader("access-control-allow-headers", "x-requested-with");
    server.sendHeader("access-control-allow-methods", "GET,OPTIONS");

    server.send(204);
  });

  //
  // LED ON Methods
  //
  server.on("/ledon", HTTP_GET, []() {
    setLedOn();
  });
  
  server.on("/ledon", HTTP_OPTIONS, []() {
    server.sendHeader("access-control-allow-credentials", "false");
    server.sendHeader("access-control-allow-headers", "x-requested-with");
    server.sendHeader("access-control-allow-methods", "GET,OPTIONS");

    server.send(204);
  });
  
  //
  // Info Methods
  //
  server.on("/info", HTTP_GET, []() {
    sendInfo();
  });
  
  server.on("/info", HTTP_OPTIONS, []() {
    server.sendHeader("access-control-allow-credentials", "false");
    server.sendHeader("access-control-allow-headers", "x-requested-with");
    server.sendHeader("access-control-allow-methods", "GET,OPTIONS");

    server.send(204);
  });
}


//
// Main Setup
//
void setup() 
{
  setupHardware();
  setupWiFi();
  setupHttpServer();
}


//
// Generates response for LED OFF Request
//
void setLedOff()
{
    ledState = LOW;
    digitalWrite(LED_PIN, ledState);
  
    String json = "{";
    json += "\"ledstate\":" + String(ledState ? "ON" : "OFF");
    json += "}";
    
    server.send(200, "text/json", json);
    json = String();   
}

//
// Generates response for LED ON Request
//
void setLedOn()
{
    ledState = HIGH;
    digitalWrite(LED_PIN, ledState);
    
    String json = "{";
    json += "\"ledstate\":" + String(ledState ? "ON" : "OFF");
    json += "}";
    
    server.send(200, "text/json", json);
    json = String();    
}

//
// Generates response for Info Request
//
void sendInfo()
{    
    String json = "{";
    
    json += "\"ssid\":" + String(WiFi.SSID()) + ",";
    json += "\"ip\":" ;
    json += WiFi.localIP().toString();
    json += ",";
    json += "\"mac\":" + String(WiFi.macAddress()) + ",";
    json += "\"ledstate\":" + String(ledState ? "ON" : "OFF");
    json += "}";
    
    server.send(200, "text/json", json);
    json = String();  
}

//
// Main Loop
//
void loop() 
{
  server.handleClient();
}




