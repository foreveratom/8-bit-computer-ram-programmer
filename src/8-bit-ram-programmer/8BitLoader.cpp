#include <SPI.h>
#include "8BitLoader.h"

Loader8B::Loader8B() {  
}

void Loader8B::execute(Program8B program, const int interval) {
  Serial.println("Loader execute()");
  SPI.begin();
  
  this->begin();
  delay(100);  

  for (int i = 0; i < program.length(); i++) {         
    //PRG | HLT | REG i
    const int low = _OUT_PRG | _OUT_HLT | (i << 4);    
    const int high = program.intOf(i);
    
    digitalWrite(_PIN_SS, LOW);
    SPI.transfer(high);   
    SPI.transfer(low);
    digitalWrite(_PIN_SS, HIGH);
    delay(interval);

    Serial.print(i);
    Serial.print(": ");
    Serial.print(Program8B::binaryOf(high, 8));   
    Serial.print(" ");
    Serial.print(Program8B::binaryOf(low, 8));   
    Serial.println("");      
  }

  this->reset();
  delay(interval);
      
  this->clear();
  delay(10);  

  this->end();      

  SPI.end();
}

void Loader8B::begin() {  
  Serial.println("send BEGIN");

  pinMode(_PIN_PRG, OUTPUT);
  pinMode(_PIN_HLT, OUTPUT);
  pinMode(_PIN_SS, OUTPUT); 
  pinMode(LED_BUILTIN, OUTPUT);     
  delay(10);

  digitalWrite(LED_BUILTIN, LOW);  
  delay(10);

  digitalWrite(_PIN_HLT, HIGH);  
  delay(10);

  digitalWrite(_PIN_PRG, HIGH);
  delay(10);
    
}

void Loader8B::end() {
  Serial.println("send END");
  
  digitalWrite(_PIN_PRG, LOW);
  delay(10);  
  
  digitalWrite(_PIN_HLT, LOW);
  delay(10);
  
  digitalWrite(LED_BUILTIN, HIGH);     
}

void Loader8B::clear() {
  Serial.println("send CLEAR");  
  
  digitalWrite(_PIN_SS, LOW);
  SPI.transfer(0);
  SPI.transfer(0);
  digitalWrite(_PIN_SS, HIGH);
}

void Loader8B::reset() {
  Serial.println("send RESET");  
  digitalWrite(_PIN_SS, LOW);
  SPI.transfer(0);
  SPI.transfer(_OUT_RST | _OUT_HLT | _OUT_PRG);
  digitalWrite(_PIN_SS, HIGH);  
}
