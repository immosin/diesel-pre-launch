#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

typedef std::function<void(void)> PFunc;

const char* ssid = "NodeMCU";
const char* password = "12345678";

#define RELAY_COUNT 4

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

uint8_t relayPins           [RELAY_COUNT] = {D5, D6, D7, D8};
bool    relayStates         [RELAY_COUNT] = {HIGH, HIGH, HIGH, HIGH};
String  relayOnFuncsNames   [RELAY_COUNT] = {"/relay1On",
                                             "/relay2On", 
                                             "/relay3On",
                                             "/relay4On"};
String  relayOffFuncsNames  [RELAY_COUNT] = {"/relay1Off",
                                             "/relay2Off", 
                                             "/relay3Off",
                                             "/relay4Off"};
void changeRelayState()
{
    for (size_t i = 0; i < RELAY_COUNT; i++)
  {
    digitalWrite(relayPins[i], relayStates[i]);
  }
  server.send(200, "text/html", SendHTML()); 
}

void HR1On(){relayStates[0]=LOW;changeRelayState();}
void HR2On(){relayStates[1]=LOW;changeRelayState();}
void HR3On(){relayStates[2]=LOW;changeRelayState();}
void HR4On(){relayStates[3]=LOW;changeRelayState();}

void HR1Off(){relayStates[0]=HIGH;changeRelayState();}
void HR2Off(){relayStates[1]=HIGH;changeRelayState();}
void HR3Off(){relayStates[2]=HIGH;changeRelayState();}
void HR4Off(){relayStates[3]=HIGH;changeRelayState();}


PFunc   relaysOnFuncs       [RELAY_COUNT] = {HR1On,
                                             HR2On,
                                             HR3On,
                                             HR4On};
PFunc   relaysOffFuncs      [RELAY_COUNT] = {HR1Off,
                                             HR2Off,
                                             HR3Off,
                                             HR4Off};

void setup()
{

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  server.on("/",        HConnect);
  server.onNotFound(handle_NotFound);
  
  for (size_t i = 0; i < RELAY_COUNT; i++)
  {
    pinMode(     relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], relayStates[i]);

    server.on(relayOnFuncsNames[i],  relaysOnFuncs[i]);
    server.on(relayOffFuncsNames[i], relaysOffFuncs[i]);
  }
  
  server.begin();
}

void loop() {
  server.handleClient();
}

void HConnect()
{
  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML()
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Relay Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 18px;color: #888;margin-bottom: 4px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  //ptr +="<h1>SsangYong Actyon</h1>\n";
  ptr +="<h3>Prelaunch Heaters Control Unit</h3>\n";
  
   if(!relayStates[0])
  {ptr +="<p>Glow plugs: ON</p><a class=\"button button-off\" href=\"/relay1Off\">OFF</a>\n";}
  else
  {ptr +="<p>Glow plugs: OFF</p><a class=\"button button-on\" href=\"/relay1On\">ON</a>\n";}

  if(!relayStates[1])
  {ptr +="<p>Turbine: ON</p><a class=\"button button-off\" href=\"/relay2Off\">OFF</a>\n";}
  else
  {ptr +="<p>Turbine: OFF</p><a class=\"button button-on\" href=\"/relay2On\">ON</a>\n";}

    if(!relayStates[2])
  {ptr +="<p>Fuel: ON</p><a class=\"button button-off\" href=\"/relay3Off\">OFF</a>\n";}
  else
  {ptr +="<p>Fuel: OFF</p><a class=\"button button-on\" href=\"/relay3On\">ON</a>\n";}

    if(!relayStates[3])
  {ptr +="<p>Pump: ON</p><a class=\"button button-off\" href=\"/relay4Off\">OFF</a>\n";}
  else
  {ptr +="<p>Pump: OFF</p><a class=\"button button-on\" href=\"/relay4On\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
