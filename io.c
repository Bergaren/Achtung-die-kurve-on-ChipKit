#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "io.h"

int getSw ( void ){
    int input = (PORTD >> 8) & 0xF;
    return input;
}

int getBtns( void ){
    int input = (PORTD >> 5) & 0x7;
    return input;
}

int getJyStckX(int plrNr){
      int input = (PORTE >> (plrNr + plrNr - 2) ) & 0x1;
  return input;
}

int getJyStckY(int plrNr){
  int input = (PORTE >> ((2*plrNr) - 1)) & 0x1;
  return input;
}
