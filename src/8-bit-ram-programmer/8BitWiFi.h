#ifndef __8BIT_WIFI__
#define __8BIT_WIFI__

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "8BitLoader.h"
#include "8BitProgram.h"

class WebServer8B {
  public:
    WebServer8B(
      const char* ssid,
      const char* password,
      const int port = 80);

    ~WebServer8B();
    
    bool begin(const int retries = 5);

    void listen();   

  private: 
    const char* _ssid;
    const char* _key;

    int _port;    
    
    //A bad attempt at HTTP concurrency management
    bool _busy = false;

    WiFiServer* _server;    
    Program8B _program;     
    Loader8B _loader;
    
    void accept(WiFiClient& client);
    
    bool handleLoad(const String request);
    
    bool handleWrite(const String request);    
    
    bool handleClear(const String request);

    static void reply(WiFiClient& client, const int result, const String& body);
    
    static String htmlOf(Program8B program);
    
};

#endif
