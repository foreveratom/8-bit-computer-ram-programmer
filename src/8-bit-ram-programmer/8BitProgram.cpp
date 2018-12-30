#include <SPI.h>
#include "8BitProgram.h"

Program8B::Program8B() { 
  for (int i = 0; i < length(); i++) {
    _program[i] = 0;
  }
}

Program8B& Program8B::clear() {
   for (int i = 0; i < length(); i++) {
    _program[i] = 0;
   }
   return *this;       
}

const int Program8B::intOf(const int step) {
  if ((step < 0) || (step >= length())) {
    return 0;      
  }
  return _program[step];
}

const String Program8B::instructionOf(const int step) {
  if ((step < 0) || (step >= length())) {
    return "";      
  }

  const int instruction = _program[step] >> 4;
  const int payload = _program[step] - (instruction << 4);

  switch (instruction) {
    case _NOP:  
      return "NOP";
    case _OUT:  
      return "OUT";
    case _HLT:  
      return "HLT";        
    case _LDA:  
      return "LDA " + String(payload);
    case _LDI:  
      return "LDI " + String(payload);
    case _ADD:  
      return "ADD " + String(payload);
    case _SUB:  
      return "SUB " + String(payload);
    case _STA:  
      return "STA " + String(payload);
    case _JMP:  
      return "JMP " + String(payload);
    case _JC:  
      return "JC " + String(payload);
    case _JZ:  
      return "JZ " + String(payload);
    default:
      return "???";          
  }
}

const String Program8B::binaryOf(const int step) {      
  if ((step < 0) || (step >= length())) {
    return "";      
  }
  return binaryOf(_program[step], 8);
}

Program8B& Program8B::set(const int step, const int instruction, const int payload) {
  if ((step < 0) || (step >= length())) {
    return *this;      
  }
  
  _program[step] = (instruction << 4) + (payload);          
  return *this;
}

Program8B& Program8B::set(const int step, const String instruction) {
  if ((step < 0) || (step >= length())) {
    return *this;      
  }

  const int space = instruction.indexOf(" ");
  const String inst = (space == -1) ? instruction : instruction.substring(0, space);
  const int payload = (space == -1) ? 0 : instruction.substring(space).toInt();

  if (inst.equalsIgnoreCase("NOP")) {
    NOP(step);
    return *this;
  }
  
  if (inst.equalsIgnoreCase("HLT")) {
    HLT(step);
    return *this;
  }
  
  if (inst.equalsIgnoreCase("OUT")) {
    OUT(step);
    return *this;
  }
  
  if (inst.equalsIgnoreCase("LDA")) {
    LDA(step, payload);
    return *this;
  }
  
  if (inst.equalsIgnoreCase("LDI")) {
    LDI(step, payload);
    return *this;
  }

  if (inst.equalsIgnoreCase("ADD")) {
    ADD(step, payload);
    return *this;
  }

  if (inst.equalsIgnoreCase("SUB")) {
    SUB(step, payload);
    return *this;
  }

  if (inst.equalsIgnoreCase("STA")) {
    STA(step, payload);
    return *this;
  }
  
  if (inst.equalsIgnoreCase("JMP")) {
    JMP(step, payload);
    return *this;
  }
  
  if (inst.equalsIgnoreCase("JC")) {
    JC(step, payload);
    return *this;
  }

  if (inst.equalsIgnoreCase("JZ")) {
    JZ(step, payload);
    return *this;
  }
  
  return *this;
}


String Program8B::binaryOf(const int value, const int digits) {
  String returned = "";
  
  int mask = 0;
  int current = value;
  
  for (int i = 0; i < digits; i++) {
    mask = (mask << 1) | 0x0001;
  }
  
  current = current & mask;
  int a = digits;
  while (a) {
    returned = returned + ((current & (0x0001 << (a - 1))) ? "1" : "0");
    a--;    
  }
  return returned;
}
