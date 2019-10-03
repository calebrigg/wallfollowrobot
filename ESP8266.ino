#define WIFI_SSD "*******"
#define WIFI_PASSWORD "*******"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//This is the IP address that is assigned to the ESP device
IPAddress staticIP(192, 168, 0, 69); 
//The gateway address of the network
IPAddress gateway(192, 168, 0, 1); 
//The subnet mask
IPAddress subnet(255, 255, 255, 0); 
//DNS 
IPAddress dns(8, 8, 8, 8);  

//Creates an object of the server that listens on port 80
ESP8266WebServer server(80); 

// handles requests to the webroot and displays a confirmation message
void rootHandler() {
 server.send(200, "text/html", "ESP is up and running. :-)");
}

// API end point that forwards serial Input to the RedBoard
void inputHandler(){
  // if there is nothing to be sent to the Arduino then returns a 400 status error code
   if(!server.hasArg("input") || server.arg("input")==NULL) { 
        server.send(400, "text/plain", "400: Invalid Request");
        return;
    }
  // if there's an input in the body it will be written to the Serial output which will be read by the RedBoard
   else{
      Serial.println(server.arg("input"));
      server.send(200, "text/plain", "Input has been written to the RedBoard!");
      return;
   }
 }

void setup() {
  
  Serial.begin(9600);
  Serial.println();

  //Attempts a connection the Wi-FI
  Serial.printf("Connecting to WiFi: %s\n", WIFI_SSD);
  WiFi.config(staticIP, gateway, subnet);
  WiFi.begin(WIFI_SSD, WIFI_PASSWORD);
  //waits until the connection succeeds
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //prints IP of the ESP8266 device
  Serial.print("\nIP address: ");
  Serial.println(WiFi.localIP());

  //set handlers for different endpoints of the API
  server.on("/", rootHandler);
  server.on("/input", HTTP_POST, inputHander); 
  
  //set serial baud to 115200.
  Serial.end();
  Serial.begin(115200);
  
  Serial.println("WC,");
  
  //start up the server
  server.begin(); 
}

void loop() {
  //handle any requests
  server.handleClient();
}
