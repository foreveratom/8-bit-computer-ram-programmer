#ifndef __8BIT_PROGRAM__
#define __8BIT_PROGRAM__

#include <Arduino.h>

class Program8B {
  public:
    Program8B();    
    
    Program8B& clear();
    
    const int intOf(const int step);
    
    const String instructionOf(const int step);
    
    const String binaryOf(const int step) ;
    
    Program8B& set(const int step, const int instruction, const int payload);

    Program8B& NOP(const int step) {
      return set(step, _NOP, 0);
    }

    Program8B& LDA(const int step, const int value) {
      return set(step, _LDA, value);
    }
    
    Program8B& LDI(const int step, const int value) {      
      return set(step, _LDI, value);
    }
    
    Program8B& ADD(const int step, const int value) {
      return set(step, _ADD, value);
    }
    
    Program8B& SUB(const int step, const int value) {      
      return set(step, _SUB, value);
    }

    Program8B& STA(const int step, const int address) {      
      return set(step, _STA, address);
    }
    
    Program8B& JMP(const int step, const int count) {      
      return set(step, _JMP, count);
    }
    
    Program8B& JC(const int step, const int value) {      
      return set(step, _JC, value);
    }
    
    Program8B& JZ(const int step, const int value) {      
      return set(step, _JZ, value);
    }
    
    Program8B& HLT(const int step) {      
      return set(step, _HLT, 0);
    }
    
    Program8B& OUT(const int step) {      
      return set(step, _OUT, 0);
    }

    const int length() {
      return sizeof(_program) / sizeof(int);
    }

    static String binaryOf(const int value, const int digits);

  private:
    //refers to the 8 Bit computer microcode instruction set 
    static const int _NOP = 0;
    static const int _LDA = 1;
    static const int _ADD = 2;
    static const int _SUB = 3;
    static const int _STA = 4;
    static const int _LDI = 5;
    static const int _JMP = 6;
    static const int _JC = 7;
    static const int _JZ = 8;
    static const int _OUT = 14;
    static const int _HLT = 15;

    int _program[16];
    
};



#endif
