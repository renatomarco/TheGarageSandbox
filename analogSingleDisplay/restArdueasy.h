#include "structs.h"

void bind ( String method, String uri, void (*f)(EthernetClient, RequestParameter) );

void parseHeader(EthernetClient client);

void beginSerial();

void beginEthernet(byte mac[], IPAddress ip, EthernetServer svr);

void restLoop(EthernetServer server);
