
#include <xc.h>
#include "adc.h"


#define delay_4xus(us)  { char x=us;  while(x--);}

unsigned int Temp = 0;

void AD_set(void)
{
    TRISAbits.TRISA0=0;

    ADCON0bits.CHS0 = 0; //  Analog Channel Select Bits
    ADCON0bits.CHS1 = 0; //  Analog Channel Select Bits
    ADCON0bits.CHS2 = 0; //  Analog Channel Select Bits

//    ADCON1bits.ADFM = 1; //Right Justified
    ADCON0bits.ADON = 1; // AD Converter Module is powered up


}

void ADC_init(void)
{

    AD_set();

}

unsigned int AD_read(void)
{

    TRISAbits.TRISA0 = 1;
//    ADCON1bits.ADFM = 1; //Right Justified
    
    delay_4xus(5);
    ADCON0bits.ADON = 1; // AD Converter Module is powered up
    ADCON0bits.GO = 1; // AD Go ...Start Conversion

    while (ADCON0 & 0x02) //Wait GO/Done Bits.........
        continue;

    Temp = ((int) ADRESH << 8) | ADRESL;
    return Temp;
}

