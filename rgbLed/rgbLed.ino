#include <SPI.h>
#include <Ethernet.h>
#include "restArdueasy.h"


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177);
EthernetServer server(80);

const int R_PIN = 3;
const int G_PIN = 5;
const int B_PIN = 6;


void setup() {
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  beginSerial();
  beginEthernet(mac, ip, server);

  bind ("GET", "/rgb", &setRGBValue);
}


void loop() {
  restLoop(server);
}


void setRGBValue(EthernetClient client, RequestParameter p) {
  Serial.println(map(p.value[0],48,57,0,255));
  if(p.name=="R") analogWrite(R_PIN, map(p.value[0],48,57,0,255));
  if(p.name=="G") analogWrite(G_PIN, map(p.value[0],48,57,0,255));
  if(p.name=="B") analogWrite(B_PIN, map(p.value[0],48,57,0,255));
  httpOK(client);
}


void httpOK(EthernetClient client){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println();
}
