//Code Created by Keith Fritz
//ESET 369 Lab 8 Part A
#include "msp.h"
void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;  // hold watchdog timer
    int i;
    int j;
    //Outputs:
    P3->DIR |= BIT0; //output direction

    //Inputs:
    P1->DIR &= ~0xC0;    //clear for input
    P1->REN |= 0xC0;    //enable internal resistor
    P1->OUT |= 0xC0;     //pull-up resistors
    P3->DIR &= ~0xC0;    //clear for input
    P3->REN |= 0xC0;    //enable internal resistor
    P3->OUT |= 0xC0;     //pull-up resistors

    TIMER_A0->CCR[0] = 3000; //set the value of CCR0
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1 | TIMER_A_CTL_CLR; //TA0CTL setup, up mode
    // From BH board: UP / DOWN / LEFT / RIGHT
    //P1.6   P1.7   P3.6   P3.7
    //Yellow / Orange / Red / Brown (wire connections)
    while(1) {
    //LED2 Color Select:
            if (P3->IN & BIT6) { //LEFT
                //not pressed
            }   else {
                //Nothing
            }
            if (P3->IN & BIT7) { //RIGHT
                //Not Pressed
            }   else {
                //Nothing
            }
            if (P1->IN & BIT7) { //DOWN
                //Not Pressed
            }   else {
                //5 Beeps
                for(j=1; j < 6; j++) {
                    for(i=1; i < 50000; i++) {
                        if((TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)!=0) {
                        P3->OUT ^= BIT0; //Toggle P3.0
                        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; //clear CCIFG flag
                        }
                    }
                    for(i=1; i < 50000; i++) {
                        //do nothing
                    }
                }
            }

            if (P1->IN & BIT6)  {//UP
                //Not Pressed
            }   else {
                //Single Beep
                for(j=1; j < 70000; j++) {
                    if((TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)!=0) {
                    P3->OUT ^= BIT0; //Toggle P3.0
                    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; //clear CCIFG flag
                    }
                }
            }

        }
    }
