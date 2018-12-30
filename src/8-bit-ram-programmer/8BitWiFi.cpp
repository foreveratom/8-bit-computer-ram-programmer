#include "8BitWiFi.h"

WebServer8B::WebServer8B(
    const char* ssid,
    const char* key,
    const int port) {
      this->_ssid = ssid;
      this->_key = key;
      this->_port = port;
     
      this->_server = new WiFiServer(port);           

  //Fibonacci sequence
  //(see https://theshamblog.com/programs-and-more-commands-for-the-ben-eater-8-bit-breadboard-computer/)
   _program
    .LDI(0, 1)
    .STA(1, 14)
    .LDI(2, 0)
    .STA(3, 15)
    .OUT(4)
    .LDA(5, 14)
    .ADD(6, 15)
    .STA(7, 14)
    .OUT(8)
    .LDA(9, 15)
    .ADD(10, 14)
    .JC(11, 13)
    .JMP(12, 3);
}

WebServer8B::~WebServer8B() {
  delete this->_server;  
}

bool WebServer8B::begin(const int retries) {      
  WiFi.mode(WIFI_STA);   
  WiFi.begin(this->_ssid, this->_key);
  
  int count = 0;
  while ((WiFi.status() != WL_CONNECTED) && (count < retries)) {
      delay(1000);  
      count = count + 1;
  }  
  
  if (WiFi.status() != WL_CONNECTED) {     
    Serial.println("Unable to connect to WiFi.");
    return false;
  }
          
  _server->begin();
  
  Serial.print("WiFi server started on ");
  Serial.print(WiFi.localIP());
  Serial.println("");
  return true;
}

void WebServer8B::listen() {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }
    
  WiFiClient client = _server->available();
  if (client) {
    client.setTimeout(1000 * 60);  
    this->accept(client);
    client.stop();
  }
}

/*GET /?program=LDA%205;%20HLT HTTP/1.1*/
void WebServer8B::accept(WiFiClient& client) {  
  String request = client.readStringUntil('\r');
  request.remove(request.lastIndexOf(" HTTP"), request.length());
  request.trim();
  
  while (client.available()) {
    client.read();
  }

  Serial.print(client.remoteIP());
  Serial.print(":");
  Serial.print(client.remotePort());   
  Serial.print(":");
  Serial.print(request);
  Serial.println("");

  if (!request.startsWith("GET")) {
    this->reply(client, 405, "");//method not supported
  }
  
  if (this->_busy) {
    this->reply(client, 503, "");//unavailable
    return;
  }

  this->_busy = true;
 
  request.replace("GET /", "");  
  request.replace("%20", " ");
  request.replace("?", "&");
  request.trim();

  if (request.length() == 0) {
    this->reply(client, 200, htmlOf(this->_program));
    this->_busy = false;
    return;
  }

  bool success = false;
  if (request.indexOf("&clear") != -1) {
    this->handleClear(request);
    success = true;
  }
    
  if (request.indexOf("&load=") != -1) {       
    this->handleLoad(request);
    success = true;
  }
  
  if (request.indexOf("&write") != -1) {
    this->handleWrite(request);
    success = true;  
  }
  
  if (success) {
    reply(client, 200, htmlOf(this->_program));
  }
  else {
    reply(client, 404, "");
  }
  this->_busy = false;
}

bool WebServer8B::handleLoad(const String request) {
    this->_program.clear();    
    String p = request.substring(request.indexOf("&load="));
    p.replace("&load=", "");
    p = p.substring(0, p.indexOf("&"));
    
    //p is expected to be a CSV of instructions in assembly form (e.g: "LDA 5; OUT; HLT");    
    
    Serial.println("Program loaded");
    for (int i = 0; i < this->_program.length(); i++) {
        Serial.println(this->_program.instructionOf(i));
    }   
    Serial.println("");
    
    return true;
}

bool WebServer8B::handleWrite(const String request) {        
    int iindex = request.indexOf("&interval=");
    if (iindex == -1) {
      this->_loader.write(this->_program, 100);
    }
    else {
      String p = request.substring(iindex);
      p.replace("&interval=", "");
      p = p.substring(0, p.indexOf("&"));
      
      const long pi = p.toInt();
      this->_loader.write(this->_program, (pi > 100) ? pi : 100);      
    }
        
    Serial.println("Program written");
    return true;
}

bool WebServer8B::handleClear(const String request) {
    this->_program.clear();
    Serial.println("Program cleared");
    return true;
}

void WebServer8B::reply(WiFiClient& client, const int result, const String& body) {
  client.print("HTTP/1.1 ");
  client.print(result);
  client.print("\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><body>");
  client.print(body);
  client.print("</body></html>");
}


String WebServer8B::htmlOf(Program8B program) {
  String html = String("<div><table>");
   for (int i = 0; i < program.length(); i++) {
    html = html + "<tr>";
    html = html + "<td>" + i + "</td>";
    html = html + "<td>:</td>";
    html = html + "<td>" + program.instructionOf(i) + "</td>";
    html = html + "<td>:</td>";
    html = html + "<td>" + program.binaryOf(i) + "</td>";
    html = html + "</tr>";    
  }
  html = html + "</table></div>";
  return html;
}
