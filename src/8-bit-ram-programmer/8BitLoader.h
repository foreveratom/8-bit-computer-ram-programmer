#ifndef __8BIT_LOADER__
#define __8BIT_LOADER__

#include <Arduino.h>
#include "8BitProgram.h"

class Loader8B {
  public:
    Loader8B();    
    
    //Write the program into the 8 bit computer memory with the given delay interval between each step (in milliseconds).
    //a low value (100) is best but if you want to see things happening on the computer, you will want a higher value (1000 for 1 second between each step)
     
    //What happens during write is the following:    
    //1 - set PRG and HLT pins up top stop the computer and enter programming mode.
    //
    //2 - step through each instruction and activate the shift register bits accordingly to set the computer memory.
    //_OUT_HLT and _OUT_PRG will be set to 1 in each step
    //
    //3 - set _OUT_RST (RESET) on the shift register bits for <interval> milliseconds to initialize the computer.
    //
    //4 - set PRG and HLT pins down
    void write(Program8B program, const int interval = 100) {
      execute(program, (interval > 100) ? interval : 100);
    }

  private:
    //HALT flag - should be wired to HLT on the clock. 
    //This flag is set to 1 when the program is being loaded in the computer.
    static const int _OUT_HLT = 0b00001000;

    //PROGRAM flag - should be wired to the LS157 of the RAM programer and/or in replacement of the reset button.
    //This flag is set to 1 when the program is being loaded in the computer.
    static const int _OUT_PRG = 0b00000100;

    //RESET flag - should be wired to the RESET button IN.
    //This flag is set to 1 for a short while (execution interval) once the program is fully loaded.
    static const int _OUT_RST = 0b00000010;
        
    //There is one unused bit left for custom usage
    //static const int _OUT_UNUSED = 0b00000001; 
    
    //_PIN_PRG and _PIN_HLT are up when the program is being loaded in the computer.    
    static const int _PIN_PRG = D2;//04
    static const int _PIN_HLT = D1;//05
    
    //I wanted to use D6 to have a RESET pin but it is not usable:
    //D6 is up during boot and when pressing the reset button of the ESP12.
    //static const int _PIN_RST = D6;

    //Master/Slave I2C pin
    static const int _PIN_SS = D8;//15

    //Additional pins used for I2C wiring
    //static const int _PIN_CLK = D5;//14
    //static const int _PIN_MOSI = D7;//13
  
    void execute(Program8B program, const int interval);

    void begin();

    void end();

    void clear();

    void reset();
};



#endif
