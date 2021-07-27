#include "msp.h"

// ESET 369 TERM PROJECT
// Code Created by: Keith Fritz
// November 16th 2020
extern unsigned char RXdata[], TXdata[], RXcnt;
extern unsigned char LCD_data[];
void BH_EDU_SIM_init(void);
void BH_EDU_SIM_task(void);
void LCD_command(unsigned char);
void LCD_write(unsigned char);
void LCD_init(void);
void LeftKey(void);
void RightKey(void);
void DownKey(void);
void UpKey(void);
unsigned int flag;
unsigned int adc_raw[3]; //array

void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;  // hold watchdog time
    P3->DIR |= BIT0; //output direction

    P1->DIR &= ~0xC0;    //clear for input
    P1->REN |= 0xC0;    //enable internal resistor
    P1->OUT |= 0xC0;     //pull-up resistors
    P3->DIR &= ~0xC0;    //clear for input
    P3->REN |= 0xC0;    //enable internal resistor
    P3->OUT |= 0xC0;     //pull-up resistors
    P1->DIR &= ~BIT1; // clear for input
    P1->REN |= BIT1;  // enable internal resistor
    P1->OUT |= BIT1;  // pull-up resistor

    P1->DIR |= BIT0;  // output direction for LED
    P5->DIR |= BIT6; // output direction
    P5->SEL1 &= ~BIT6;  // TA2.1 selection
    P5->SEL0 |= BIT6;  // TA2.1 selection

    P6->DIR |= 0xC1;  // set for output (Port 6)
    P6->OUT &= ~0xC1;
    P2->DIR |= 0xFF;  // set for output (Port 2)
    P2->OUT &= ~0xFF;

    P4->SEL0 |= 0x07; //alternative function (A11, A12, A13)
    P4->SEL0 |= 0x07; //alternative function (A11, A12, A13)
    ADC14->CTL0 = ADC14_CTL0_SHT0_6 | ADC14_CTL0_SHP | ADC14_CTL0_ON; //ADC CTL0 set up
    ADC14->CTL1 = ADC14_CTL1_RES_2; //ADC CTL1 setup, 12 bit mode
    ADC14->CTL0 |= ADC14_CTL0_MSC | ADC14_CTL0_CONSEQ_1; //multiple sample conversion, sequence of channel mode
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_11;  //multiple channel selection
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_12;
    ADC14->MCTL[2] = ADC14_MCTLN_INCH_13 | ADC14_MCTLN_EOS; // end of sequence

    TIMER_A0->CCR[0] = 3000; //set the value of CCR0
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1 | TIMER_A_CTL_CLR; //TA0CTL setup, up mode

    TIMER_A2->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1 | TIMER_A_CTL_CLR;
    TIMER_A2->CCR[0] = 60000;  // set the value of CCR0
    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;  // output mode selection
    // From BH board: UP / DOWN / LEFT / RIGHT
    //P1.6   P1.7   P3.6   P3.7
    //Yellow / Orange / Red / Brown (wire connections)

    LCD_init();  // LCD init.

    BH_EDU_SIM_init(); //  BH EDU sim init
    // Used: P1.2, P1.3 (UART)
    // used: (Output) P1.1
    __enable_irq(); // keep this line

    LCD_command(0xC0);  //2nd line
    LCD_write('K');
    LCD_write('e');
    LCD_write('i');
    LCD_write('t');
    LCD_write('h');
    LCD_write(' ');
    LCD_write('F');
    LCD_write('r');
    LCD_write('i');
    LCD_write('t');
    LCD_write('z');

    flag = 0;

    while(1) {

        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC; //ADC, start conversion
        while ((ADC14->CTL0 & ADC14_CTL0_BUSY)!=0); //wait until busy flag is cleared

        adc_raw[0] = ADC14->MEM[0]; //read ADC
        adc_raw[1] = ADC14->MEM[1]; //read ADC
        adc_raw[2] = ADC14->MEM[2]; //read ADC

        LCD_data[0]='X';
        LCD_data[1]=':';
        LCD_data[2]='0'+(adc_raw[0] / 1000);
        LCD_data[3]='.';
        LCD_data[4]='0'+((adc_raw[0]/100)%10);
        LCD_data[5]='0'+((adc_raw[0]/10)%10);
        LCD_data[6]='0'+(adc_raw[0]%10);

        LCD_data[8]='Y';
        LCD_data[9]=':';
        LCD_data[10]='0'+(adc_raw[0] / 1000);
        LCD_data[11]='.';
        LCD_data[12]='0'+((adc_raw[0]/100)%10);
        LCD_data[13]='0'+((adc_raw[0]/10)%10);
        LCD_data[14]='0'+(adc_raw[0]%10);

        LCD_data[16]='Z';
        LCD_data[17]=':';
        LCD_data[18]='0'+(adc_raw[0] / 1000);
        LCD_data[19]='.';
        LCD_data[20]='0'+((adc_raw[0]/100)%10);
        LCD_data[21]='0'+((adc_raw[0]/10)%10);
        LCD_data[22]='0'+(adc_raw[0]%10);

        //Button Press Selection:
                if (P1->IN & BIT1) { //S1
                    //not pressed
                    //0 degrees
                    TIMER_A2->CCR[1] = 6000;  // set the value of CCR1 6000 (0 degrees)
                }   else {
                    //pressed
                    TIMER_A2->CCR[1] = 3500;  // set the value of CCR1 3500 (90 degrees)
                }

                if (P3->IN & BIT6) { //LEFT
                    //not pressed
                }   else {
                    //Left
                    if (flag == 0) {
                        LeftKey();
                    }

                        }

                if (P3->IN & BIT7) { //RIGHT
                    //Not Pressed
                }   else {
                    //Right
                    if (flag == 0) {
                        RightKey();
                    }

                        }

                if (P1->IN & BIT7) { //DOWN
                    //Not Pressed
                }   else {
                    //Down
                    if (flag == 0) {
                    DownKey();
                    }
                        }

                if (P1->IN & BIT6)  {//UP
                    //Not Pressed
                }   else {
                    //Up
                    if (flag == 0) {
                        UpKey();
                    }

                        }
                if (P3->IN & BIT6) {
                    if (P3->IN & BIT7) {
                        if (P1->IN & BIT6) {
                            if (P1->IN & BIT7) {
                                if (flag == 1) {
                                    //reset display
                                    flag = 0;
                                    LCD_command(0x01);  //clear display
                                    __delay_cycles(50000);
                                    LCD_command(0xC0);  //2nd line
                                    LCD_write('K');
                                    LCD_write('e');
                                    LCD_write('i');
                                    LCD_write('t');
                                    LCD_write('h');
                                    LCD_write(' ');
                                    LCD_write('F');
                                    LCD_write('r');
                                    LCD_write('i');
                                    LCD_write('t');
                                    LCD_write('z');
                                }
                            }
                        }
                    }
                }

        BH_EDU_SIM_task();  // BH EDU sim task
        __delay_cycles(5000); // keep this line, recommended minimum delay
    }
}

void PORT1_IRQHandler(void) {
    if(P1->IFG & BIT1) {
        P1->OUT ^= BIT0; //toggle LED
    }
    __delay_cycles(1000); //switch debounce
    P1->IFG &= ~BIT1; //clear interrupt flag
}

void LCD_command(unsigned char in) {
    P2OUT = in;
    P6OUT &= ~BIT7; //clear RS
    P6OUT &= ~BIT6; //clear R/W
    P6OUT |= BIT0; //set E
    __delay_cycles(200);
    P6OUT &= ~BIT0; //clear E
}

void LCD_write(unsigned char in) {
    P2OUT = in;
    P6OUT |= BIT7; //set RS
    P6OUT &= ~BIT6; //clear R/W
    P6OUT |= BIT0; //set E
    __delay_cycles(200);
    P6OUT &= ~BIT0; //clear E
}

void LCD_init() {
    P6OUT &= ~BIT0; //clear E
    __delay_cycles(3000);
    LCD_command(0x30); //wake up
    __delay_cycles(400);
    LCD_command(0x30);
    __delay_cycles(400);
    LCD_command(0x30);
    __delay_cycles(400);
    LCD_command(0x38);  //Function set: 8 bit/2-line
    LCD_command(0x10);  //set cursor
    LCD_command(0x0F);  //Display on, cursor on
    LCD_command(0x06);  //Entry mode set
    LCD_command(0x01);  //clear display
    __delay_cycles(5000);
}

void LeftKey() {
    int j;
    flag = 1;
    LCD_command(0x01);  //clear display
    __delay_cycles(50000);
    LCD_write('L');
    LCD_write('e');
    LCD_write('f');
    LCD_write('t');
    LCD_command(0xC0);  //2nd line
    LCD_write('K');
    LCD_write('e');
    LCD_write('i');
    LCD_write('t');
    LCD_write('h');
    LCD_write(' ');
    LCD_write('F');
    LCD_write('r');
    LCD_write('i');
    LCD_write('t');
    LCD_write('z');
    //Beep:
    for(j=1; j < 50000; j++) {
        if((TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)!=0) {
        P3->OUT ^= BIT0; //Toggle P3.0
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; //clear CCIFG flag
        }
    }
}

void RightKey() {
    int j;
    flag = 1;
    LCD_command(0x01);  //clear display
    __delay_cycles(50000);
    LCD_write('R');
    LCD_write('i');
    LCD_write('g');
    LCD_write('h');
    LCD_write('t');
    LCD_command(0xC0);  //2nd line
    LCD_write('K');
    LCD_write('e');
    LCD_write('i');
    LCD_write('t');
    LCD_write('h');
    LCD_write(' ');
    LCD_write('F');
    LCD_write('r');
    LCD_write('i');
    LCD_write('t');
    LCD_write('z');
    //Beep:
    for(j=1; j < 50000; j++) {
        if((TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)!=0) {
        P3->OUT ^= BIT0; //Toggle P3.0
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; //clear CCIFG flag
        }
    }
}

void DownKey() {
    int j;
    flag = 1;
    LCD_command(0x01);  //clear display
    __delay_cycles(50000);
    LCD_write('D');
    LCD_write('o');
    LCD_write('w');
    LCD_write('n');
    LCD_command(0xC0);  //2nd line
    LCD_write('K');
    LCD_write('e');
    LCD_write('i');
    LCD_write('t');
    LCD_write('h');
    LCD_write(' ');
    LCD_write('F');
    LCD_write('r');
    LCD_write('i');
    LCD_write('t');
    LCD_write('z');
    //Beep:
    for(j=1; j < 50000; j++) {
        if((TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)!=0) {
        P3->OUT ^= BIT0; //Toggle P3.0
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; //clear CCIFG flag
        }
    }
}

void UpKey() {
    int j;
    flag = 1;
    LCD_command(0x01);  //clear display
    __delay_cycles(50000);
    LCD_write('U');
    LCD_write('p');
    LCD_command(0xC0);  //2nd line
    LCD_write('K');
    LCD_write('e');
    LCD_write('i');
    LCD_write('t');
    LCD_write('h');
    LCD_write(' ');
    LCD_write('F');
    LCD_write('r');
    LCD_write('i');
    LCD_write('t');
    LCD_write('z');
    //Beep:
    for(j=1; j < 50000; j++) {
        if((TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)!=0) {
        P3->OUT ^= BIT0; //Toggle P3.0
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; //clear CCIFG flag
        }
    }

}
