#include "msp.h"
unsigned int adc_raw;
void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;  // hold watchdog time
    P1->DIR |= 0x01;  // output direction for P1
    P4->SEL1 |= BIT2;  // alternative function, A11
    P4->SEL0 |= BIT2;  // alternative function, A11
    ADC14->CTL0 = ADC14_CTL0_SHT0_6 | ADC14_CTL0_SHP | ADC14_CTL0_ON; //ADC CTL0 setup
    ADC14->CTL1 = ADC14_CTL1_RES_2;  // ADC CTL1 set up, 12 bit resolution
    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_11;  // channel selection
    while(1) {
        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;  // Start conversion
        while ((ADC14->CTL0 & ADC14_CTL0_BUSY)!=0);  // busy flag check
        adc_raw=ADC14->MEM[0];  // read ADC
        P1->OUT ^= 0x01;  // toggle a LED
        __delay_cycles(50000);
    }
}
