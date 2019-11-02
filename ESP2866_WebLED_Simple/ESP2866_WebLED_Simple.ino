#include <ESP8266WiFi.h>

//
// ESP8266 Web Server / LED Driver Example : https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/example-sketch-ap-web-server
// Connecting with WPA:  https://www.arduino.cc/en/Tutorial/ConnectWithWPA
//


/////////////////////
// Pin Definitions //
/////////////////////
const int LED_PIN = D7; // Thing's onboard, green LED

int ledState = LOW;


WiFiServer server(80);

void setupWiFi()
{
  WiFi.begin("VaporNet","Get49han");
}

void initHardware()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void setup() 
{
  initHardware();
  setupWiFi();
  server.begin();

}

void loop() 
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Prepare the response. Start with the common header:
  String s = "";
  s += "HTTP/1.1 200 OK\r\n";  
 // s += "Access-Control-Allow-Origin: *\r\n";
 // s += "Content-Length:05\r\n";
 // s += "Connection: close\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n";
  
  s += "<html>\r\n";
  s += processRequest(req);
  s += "</html>\n";
   
  // Send the response to the client
  client.print(s);
  delay(10);  
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed

}


String processRequest(String req)
{
  
  // Match the request
  int val = -1; // We'll use 'val' to keep track of both the
                // request type (read/set) and value if set.
                
  if (req.indexOf("info") != -1)
  {
    String s = "";
    s += "SSID: " +  WiFi.SSID() +"<BR>";
    s += "IP: ";
    s += WiFi.localIP();
    s += "<BR>";
    s += "Mac: " +  WiFi.macAddress() +"<BR>";
    s += "LED is now : ";
    s += ledState ? "ON" : "OFF";
    s += "<BR>";
    
    return s;
  }
  else if (req.indexOf("/led/0") != -1)
  {
    ledState = LOW;
    digitalWrite(LED_PIN, ledState); // Will write LED high
    return "LED is now OFF";
  }  
  else if (req.indexOf("/led/1") != -1)
  {
    ledState = HIGH;
    digitalWrite(LED_PIN, ledState); // Will write LED low
    return "LED is now ON";
  }  
  else 
  {
    return "Invalid Request.<br> Try /led/1, /led/0, or /info.";
  }
  
}    



