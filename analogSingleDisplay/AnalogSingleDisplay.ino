#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>
#include "restArdueasy.h"

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xA8, 0x38 };
IPAddress ip(192,168,147,19);
EthernetServer server(80);

char pinP = A0;
char pinA = 2;
char pinB = 3;
char pinC = 5;
char pinD = 6;
char pinE = 7;
char pinF = 8;
char pinG = 9;

char pins[8] = {pinP, pinA, pinB, pinC, pinD, pinE, pinF, pinG};

int NUMBERS[10][8] = {{0,1,1,1,1,1,1,0}, //0
                      {0,0,1,1,0,0,0,0}, //1
                      {0,1,1,0,1,1,0,1}, //2
                      {0,1,1,1,1,0,0,1}, //3
                      {0,0,1,1,0,0,1,1}, //4
                      {0,1,0,1,1,0,1,1}, //5
                      {0,1,0,1,1,1,1,1}, //6
                      {0,1,1,1,0,0,0,0}, //7
                      {0,1,1,1,1,1,1,1}, //8
                      {0,1,1,1,1,0,1,1} //9
                      };

int LETTERS[10][8] = {{0,1,1,1,0,1,1,1}, //A
                      {0,0,0,1,1,1,1,1}, //B
                      {0,1,0,0,1,1,1,0}, //C
                      {0,0,1,1,1,1,0,1}, //D
                      {0,1,0,0,1,1,1,1}, //E
                      {0,1,0,0,0,1,1,1}, //F
                      };


void setup() {
 
  beginDisplay();
  
  beginSerial();
  beginEthernet(mac, ip, server);

  bind ("GET", "/display", &setDisplay);

}

void beginDisplay() {
  for (int i=0; i < 8; i++) {
    pinMode(pins[i], OUTPUT);
  }
  
  int initializeHIGH[8] = {1,1,1,1,1,1,1};
  
  showInitialization();   
}

void loop() {
  restLoop(server);

  waiting();

/*  for (int i = 0; i < 10; i++) {
    showDisplay(NUMBERS[i], 250);    
  }
  for (int i = 0; i < 10; i++) {
    showDisplay(LETTERS[i], 250);
  }*/
}

void prepareChange() {
  for (int i = 0; i < 3; i++) {
    delay(100);
    changeSegment(0, 0);
    delay(100);
    changeSegment(0, 1);   
  }
}

void setDisplay(EthernetClient client, RequestParameter p) {
  char charBuf[10];
  int number;
  p.value.toCharArray(charBuf, 10);
  number = atoi(charBuf);
  
  if (number < 0) {
     resetDisplay(); 
  } else {
    showDisplay(NUMBERS[number], 50);
  }
  
  Serial.print("Number sent: ");
  Serial.println(number);
  httpOK(client);
}


void httpOK(EthernetClient client){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Access-Control-Allow-Origin: *");  
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println();
}

void waiting() {
  delay(300);
  changeSegment(0, 1);
  delay(300);
  changeSegment(0, 0);   
}

void showInitialization() {
  
  //Set all pins to HIGH
  for (int i=0; i < 8; i++) {
    digitalWrite(pins[i], 1);
  }
  
  delay(1000);

  resetDisplay();
  
  //round and round
  changeSegment(7, 0);
  changeSegment(0, 1);
  
  for (int r = 0; r < 3; r++) {
    
    for (int i=1; i < 7; i++) {
      if (i == 1) {
        changeSegment(i, 1);
      } else {
        changeSegment(i-1, 0);
        changeSegment(i, 1);
      }
      
      if (i % 2) {
        changeSegment(0, 1);
      } else {        
        changeSegment(0, 0);        
      }
    }  
    
    changeSegment(6, 0);
  }
}

void resetDisplay() {
  //Set all pins to LOW
  for (int i=0; i < 8; i++) {
    digitalWrite(pins[i], 0);
  }  
}

void showDisplay(int display[], int d) {
//  long beginTime = millis();
   
  //Set all pins to ZERO
  for (int i=0; i < 8; i++) {
    digitalWrite(pins[i], 0);
  }
  
  delay(d);
  
  if (display != 0) {
    //Write the given values
    for (int i=0; i < 8; i++) {
      Serial.println(display[i]);
      digitalWrite(pins[i], display[i]);
    }
  }
//  delayMicroseconds(500); 
  delay(d);
  
//  while((millis() - beginTime) < 10) ; 
}

void changeSegment(int seg, int value) {
  digitalWrite(pins[seg], value);
  delay(50);
}

