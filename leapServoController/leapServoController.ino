#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>
#include "restArdueasy.h"


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177);
EthernetServer server(80);

const int SERVO_PIN = 9;
Servo myservo;



void setup() {
  myservo.attach(SERVO_PIN);

  beginSerial();
  beginEthernet(mac, ip, server);

  bind ("GET", "/servo", &setServo);
}


void loop() {
  restLoop(server);
}


void setServo(EthernetClient client, RequestParameter p) {
  char charBuf[10];
  int pos;
  p.value.toCharArray(charBuf, 10);
  pos = atoi(charBuf);
  if (pos >= 0 || pos <= 180) {
    myservo.write(pos);
  }
  httpOK(client);
}


void httpOK(EthernetClient client){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Access-Control-Allow-Origin: *");  
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println();
}
