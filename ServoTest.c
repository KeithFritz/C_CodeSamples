#include "msp.h"
int main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;  // hold watchdog timer
    P1->DIR |= BIT0;  // output direction for LED
    P5->DIR |= BIT6; // output direction
    P5->SEL1 &= ~BIT6;  // TA2.1 selection
    P5->SEL0 |= BIT6;  // TA2.1 selection
    TIMER_A2->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1 | TIMER_A_CTL_CLR;
    TIMER_A2->CCR[0] = 60000;  // set the value of CCR0
    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;  // output mode selection
    TIMER_A2->CCR[1] = 3500;  // set the value of CCR1 (for TP use range 6000 to 3500
    while(1){
        P1->OUT ^= BIT0;  // Toggle LED
        __delay_cycles(500000);  // delay
    }
}
