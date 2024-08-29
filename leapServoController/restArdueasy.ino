#include "structs.h"



void beginSerial() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
}


void restLoop(EthernetServer server) {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    parseHeader(client);
    delay(1);
    client.stop();
  }
}

void beginEthernet(byte mac[], IPAddress ip, EthernetServer svr){
  // start the Ethernet connection and the server:
  Ethernet.begin(mac);
  svr.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

boolean EOL(char buffer[]) {
  return (buffer[0] == '\r' && buffer[1] == '\n');
}

boolean EOH(char buffer[]) {
  return (buffer[0] == '\r' && buffer[1] == '\n' && buffer[2] == '\r' && buffer[3] == '\n');
}

String hashUri(String method, String uri) {
  return method + ':' + uri; 
}

void (*bindedFunction[16])(EthernetClient, RequestParameter);
String bindedURI[16];
int bindCount = 0;


void bind ( String method, String uri, void (*f)(EthernetClient, RequestParameter) ) {
  Serial.println("Binding: " + method + " - " + uri);
  bindedFunction[bindCount] = f;
  bindedURI[bindCount] = hashUri(method, uri);
  bindCount = bindCount + 1;
}

void updateBuffer(char b[], char c) {
  b[0] = b[1];
  b[1] = b[2];
  b[2] = b[3];
  b[3] = c;
}

String parseHTTPField(EthernetClient client, char delimiter) {
  char buffer[4];
  String parameter = String();
  while (client.connected() && client.available() && !EOL(buffer)) {
    char c = client.read();
    if(c == delimiter) break;
    updateBuffer(buffer, c);
    parameter += c;
  }
  return parameter;
}

String parseHTTPMethod(EthernetClient client) {
  return parseHTTPField(client, ' ');
  
}

String parseRequestURI(EthernetClient client, RequestParameter &parameter) {
  String completeURI = parseHTTPField(client, ' ');
  if(completeURI.indexOf('?') != -1){
      parameter.name = completeURI.substring(completeURI.indexOf('?') + 1);
      parameter.value = parameter.name.substring(parameter.name.indexOf('=') + 1);
      parameter.name = parameter.name.substring(0,parameter.name.indexOf('='));
  }
  return completeURI.substring(0,completeURI.indexOf('?'));
}


void parseHeader(EthernetClient client) {
  String method = parseHTTPMethod(client);
  RequestParameter *p = new RequestParameter;
  String uri = parseRequestURI(client, *p);
  Serial.println("New Request => " + method + ':' + uri + " => " + p->name + " = " + p->value);

  for(int i = 0; i < bindCount; i++){
    if (bindedURI[i] == hashUri(method, uri)) {
      bindedFunction[i](client, *p);
    }
  }
  
  delete p;
}

