#include "Fiware.h"
// Cloud data
#define SERVER         "test.ttcloud.net"     // replace with your server
#define PORT           8082                 // replace with your port
    
//################ FIWARE VARIABLES ################
char FIWARE_APIKEY[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char FIWARE_DEVICE[] = "myArduino";
char FIWARE_SERVER[] = "test.ttcloud.net";
int FIWARE_PORT = 8082;

WiFiClient client;
  
void postMeasures(String body)
{
    Serial.println("\nStarting connection to server...");
    
    // If you get a connection, report back via serial:
    if (client.connect(FIWARE_SERVER, FIWARE_PORT)){
      Serial.println("connected to server");
    }
      
    // Make a HTTP request:
    client.println("POST /iot/d?i="+String(FIWARE_DEVICE)+"&k="+String(FIWARE_APIKEY)+"&getCmd=1 HTTP/1.1");    
    client.println("Host:"+String(FIWARE_SERVER)+":"+String(FIWARE_PORT));
    client.println("Content-Length: "+String(body.length()));
    client.println("Connection: close");
    client.println();
    client.println(body);
    Serial.println(body);
    
    client.stop();
    
    }

