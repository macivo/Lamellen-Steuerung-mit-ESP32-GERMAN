// Webserver_LED.ino
// IDE 1.8.9
// ESP32

#include <WiFi.h>
#include "my_secrets.h"
WiFiServer server(80);

#define output_A 26  // select pin A 
#define output_B 27  // select pin B 

String header;
String output_A_State = "off";
String output_B_State = "off";

void setup() 
{ Serial.begin(115200);
  pinMode(output_A, OUTPUT); pinMode(output_B, OUTPUT);
  digitalWrite(output_A, LOW); digitalWrite(output_B, LOW);
  Serial.print("Connecting to ");
  Serial.println(SECRET_SSID);
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  while (WiFi.status() != WL_CONNECTED) 
  { delay(500);  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WLAN connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop()
{ WiFiClient client = server.available();   
  if (client) 
  { Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected()) 
    { if (client.available()) 
      { char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') 
        { if (currentLine.length() == 0) 
          { client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            if (header.indexOf("GET /output_A/on") >= 0) 
            { output_A_State = "on"; digitalWrite(output_A, HIGH);
            } 
            else if (header.indexOf("GET /output_A/off") >= 0) 
            { output_A_State = "off"; digitalWrite(output_A, LOW);
            } 
            else if (header.indexOf("GET /output_B/on") >= 0) 
            { output_B_State = "on"; digitalWrite(output_B, HIGH);
            } 
            else if (header.indexOf("GET /output_B/off") >= 0) 
            { output_B_State = "off"; digitalWrite(output_B, LOW);
            }
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button1 {background-color: green;}");
            client.println(".button2 {background-color: red;}</style></head>");
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            client.println("<p>Channel A: " + output_A_State + "</p>");
                          
            if (output_A_State=="off") 
            { client.println("<p><a href=\"/output_A/on\"><button class=\"button1\">ON</button></a></p>");
            } 
            else 
            { client.println("<p><a href=\"/output_A/off\"><button class=\"button2\">OFF</button></a></p>");
            } 
            
            client.println("<p>Channel B: " + output_B_State + "</p>");
  
            if (output_B_State=="off") 
            { client.println("<p><a href=\"/output_B/on\"><button class=\"button1\">ON</button></a></p>");
            } 
            else 
            { client.println("<p><a href=\"/output_B/off\"><button class=\"button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            client.println();
            break;
          } 
          else 
          { currentLine = "";
          }
    } 
    else if (c != '\r') 
    { currentLine += c;
    }
   }
 }
 header = "";
 client.stop();
 Serial.println("Client disconnected.");
 Serial.println("");
 }
}
