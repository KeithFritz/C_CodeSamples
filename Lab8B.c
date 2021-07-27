//Code Created by Keith Fritz
//ESET 369 Lab 8 Part B
#include "msp.h"
unsigned int adc_raw;
void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; //hold watchdog timer
    int ccr1;
    int ccr2;
    int direction;
    unsigned int setSpeed;
    ccr1 = 0;
    ccr2 = 0;
    direction = 0;
    P1->DIR |= BIT0; //output direction for LED
    P5->DIR |= BIT0; //output direction for P5.0
    P5->OUT |= BIT0; //set output of P5.0
    P2->DIR |= BIT4 | BIT5; //output direction
    P2->SEL1 &= ~(BIT4 | BIT5); //TA0.1 & TA0.2 selection
    P2->SEL0 |= (BIT4 | BIT5); //TA0.1 & TA0.2 selection
    P4->SEL1 |= BIT2;  // alternative function, A11
    P4->SEL0 |= BIT2;  // alternative function, A11
    ADC14->CTL0 = ADC14_CTL0_SHT0_6 | ADC14_CTL0_SHP | ADC14_CTL0_ON; //ADC CTL0 setup
    ADC14->CTL1 = ADC14_CTL1_RES_2;  // ADC CTL1 set up, 12 bit resolution
    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_11;  // channel selection
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1 | TIMER_A_CTL_CLR; //TA0CTL setup
    TIMER_A0->CCR[0] = 5000; //set value of CCR0
    while(1) {
        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;  // Start conversion
        while ((ADC14->CTL0 & ADC14_CTL0_BUSY)!=0);  // busy flag check
        adc_raw=ADC14->MEM[0];  // read ADC
        setSpeed = adc_raw;

        if (P3->IN & BIT6) { //LEFT
            //not pressed
        }   else {
            //Forward Motor Rotation
            direction = 1;
        }
        if (P3->IN & BIT7) { //RIGHT
            //Not Pressed
        }   else {
            //Reverse Motor Rotation
            direction = 2;
        }
        if (P1->IN & BIT7) { //DOWN
            //not pressed
        }   else {
            //Slow Decay
            ccr1 = 0;
            ccr2= 0;
            direction = 0;
        }
        if (P1->IN & BIT6) { //UP
            //Not Pressed
        }   else {
            //Fast Decay
            ccr1 = 5000;
            ccr2= 5000;
            direction = 0;
        }
        if (direction == 1) {
            ccr1 = 5000;
            ccr2 = setSpeed;
        }
        if (direction == 2) {
            ccr1 = setSpeed;
            ccr2 = 5000;
        }
        TIMER_A0->CCR[1] = ccr1; //set value of CCR1
        TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7; //output mode selection
        TIMER_A0->CCR[2] = ccr2; //set value of CCR2
        TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7; //output mode selection
    }
}
