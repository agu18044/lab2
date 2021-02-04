#include <xc.h> 
#include <stdint.h>
#include "7seg.h"

uint8_t display(uint8_t valor){
    switch (valor){
        case 0: // valor = "0"
            return 63; 
            break;
        case 1: // valor = "1"
            return 6; 
            break;
        case 2: // valor = "2"
            return 91; 
            break;
        case 3: // valor = "3"
            return 79; 
            break;
        case 4: // valor = "4"
            return 102; 
            break;
        case 5: // valor = "5"
            return 109; 
            break;
        case 6: // valor = "6"
            return 125; 
            break;
        case 7: // valor = "7"
            return 7; 
            break;
        case 8: // valor = "8"
            return 127; 
            break;
        case 9: // valor = "9"
            return 111; 
            break;
        case 10: // valor = "A"
            return 119; 
            break;
        case 11: // valor = "b"
            return 124; 
            break;
        case 12: // valor = "C"
            return 57; 
            break;
        case 13: // valor = "d"
            return 94; 
            break;
        case 14: // valor = "E"
            return 121; 
            break;
        case 15: // valor = "F"
            return 113; 
            break;
        default: 
            return 0; 
    }
}
