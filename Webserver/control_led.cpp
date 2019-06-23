#include <WiFi.h>

const char* ssid = "FRITZ!Box 7560 AU";
const char* password = "90311318072085471415";
String header;
WiFiServer server(80);

int LED_BUILTIN = 2;
String led = "off";

void setup(){
  Serial.begin(115200);
  pinMode (LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //Conect to WiFi
  Serial.println("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    }

   Serial.println("");
   Serial.println("Connection Successful");
   Serial.print(WiFi.localIP());
   server.begin();   
  }


void loop(){
  WiFiClient client = server.available();

  if (client){
    Serial.println("new Client");
    String currentLine = "";
    while(client.connected()){
      if (client.available()){
        char c = client.read();
        Serial.print(c);
        header += c;
        if (c == '\n'){
          if (currentLine.length() == 0){  


            // turns the LED on/off
            if(header.indexOf("GET /led/on") >= 0){
              led = "on";
              digitalWrite(LED_BUILTIN, HIGH);
              }
            else if(header.indexOf("GET /led/off") >= 0){
              led = "off";
              digitalWrite(LED_BUILTIN, LOW);
              }
            
            client.print("<title>ESP32 Webserver</title>");
            client.print("<body><h1>Hello IoT World</h1></body>");

            if(led == "off"){
            client.println("<p><a href=\"/led/on\"><button class=\"button\">ON</button></a></p>");
            }else{
            client.println("<p><a href=\"/led/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            
            client.println();
            break;
            }
          else{
            currentLine = "";
            }  
          }
        else if (c != '\r'){
          currentLine += c;
          }  
        }
      }
      header = "";
      client.stop();
      Serial.println("Client Disconected");
      Serial.println("");
    }
      delay(500);
}
