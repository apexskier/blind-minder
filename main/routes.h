#include <ESP8266WebServer.h>

// A webserver
ESP8266WebServer server(80);

void routes_setup();
void routes_404();
void routes_root();
