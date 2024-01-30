#include <SPI.h>
#include <ESP8266WiFi.h>                     // The Basic Function Of The ESP NOD MCU


//------------------------------------------------------------------------------------
// WIFI Module Config
//------------------------------------------------------------------------------------
char ssid[] = "Casa_01";                        // SSID of your ESP Server
char pass[] = "fvml1234";                    // password of your ESP Server
WiFiServer server(80);

  IPAddress ip(192, 168, 10, 40);            // IP address of the server
  IPAddress gateway(192, 168, 10, 1);        // gateway of the server
  IPAddress subnet(255, 255, 255, 0);        // subnet mask of the server

//------------------------------------------------------------------------------------
// Defining I/O Pins
//------------------------------------------------------------------------------------
#define       LedBoard  2                    // WIFI Module LED
#define       BUZZER     16

int s = 300;
int o = 800; 

//====================================================================================
void setup() {
  Serial.begin(115200);                      // Only for debug
  
  WiFi.mode(WIFI_AP_STA);                    // Need both to serve the webpage and take commands via tcp
  
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid, pass);                   // Access point password and identification
  delay(500);
  Serial.print("AP IP address: ");
  Serial.println(ip);
  server.begin();                            // Starts the server

//------------------------------------------------------------------------------------
// Serial Network parameters - Only for debug
//------------------------------------------------------------------------------------  
  Serial.println("ESP Server Initialized - FVML");
  Serial.print("IP: ");       Serial.println(WiFi.softAPIP());
  Serial.print("SSID: ");     Serial.println(WiFi.SSID());
  Serial.print("Signal: ");   Serial.println(WiFi.RSSI());
  pinMode(BUZZER, OUTPUT);
  pinMode(LedBoard, OUTPUT);                // Initiate the Onboard Led Output
  digitalWrite(LedBoard, HIGH);             // Initiate the Onboard Led Off
  digitalWrite(BUZZER, HIGH);
}



void character(int speed) { 
  digitalWrite(BUZZER, HIGH); 
  delay(speed); 
  digitalWrite(BUZZER, LOW); 
  delay(300);
  } 

void sos(){
  for (int x = 1; x <= 3; x++) { 
    character(s); 
    } 
    delay(100); 
   for (int x = 1; x <= 3; x++) { 
    character(o); 
    } 
    delay(100); 
   for (int x = 1; x <= 3; x++) { 
    character(s); 
    } 
    delay(2000);
}


void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  String request = client.readStringUntil('\r');
  client.flush();

  if (request == "I am Transmitter") {
    digitalWrite(LedBoard, !digitalRead(LedBoard));
    sos();
    Serial.print("Data Received: "); Serial.println(request);
    delay(200);
  }

  /*client.println("Receiver\r");      // sends the answer to the client
  delay(100);*/
}