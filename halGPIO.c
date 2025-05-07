#include  "halGPIO.h"     // private library - HAL layer

//--------------------------------------------------------------------
//             System Configuration
//--------------------------------------------------------------------
void sysConfig(void){
    GPIOconfig();
    TIMERconfig();
    ADCconfig();
}
//--------------------------------------------------------------------
//              Print Byte to 8-bit LEDs array
//--------------------------------------------------------------------
void print2LEDs(unsigned char ch){
    LEDsArrPort = ch;
}
//--------------------------------------------------------------------
//              Clear 8-bit LEDs array
//--------------------------------------------------------------------
void clrLEDs(void){
    LEDsArrPort = 0x000;
}
//--------------------------------------------------------------------
//              Toggle 8-bit LEDs array
//--------------------------------------------------------------------
void toggleLEDs(char ch){
    LEDsArrPort ^= ch;
}
//--------------------------------------------------------------------
//              Set 8-bit LEDs array
//--------------------------------------------------------------------
void setLEDs(char ch){
    LEDsArrPort |= ch;
}
//--------------------------------------------------------------------
//              Read value of 4-bit SWs array
//--------------------------------------------------------------------
//unsigned char readSWs(void){
  //  unsigned char ch;

 //   ch = PBsArrPort;
//ch &= SWmask;     // mask the least 4-bit
 //   return ch;
//}
//---------------------------------------------------------------------
//             Increment / decrement LEDs shown value
//---------------------------------------------------------------------
void incLEDs(char val){
    LEDsArrPort += val;
}
//---------------------------------------------------------------------
//             function for state 1
//---------------------------------------------------------------------
void incdecLEDs(){
    int i;
    LEDsArrPort = value1;
    if (count_dir) {
        for (i=0; i<20; i++) {
            delay(LEDs_SHOW_RATE);
            incLEDs(1);
            value1++;
        }
        count_dir=0;
    }
    else {
        for (i=0; i<20; i++) {
            delay(LEDs_SHOW_RATE);
            incLEDs(-1);
            value1--;
        }
        count_dir=1;
    }
}

//---------------------------------------------------------------------
//             function for state 2
//---------------------------------------------------------------------

void skip_leds(){
    LEDsArrPort = value2;
    int i;
    unsigned char last_value2;
    for (i=0; i<14; i++) {
                delay(LEDs_SHOW_RATE);
                int last_value2=((value2<<1)|(value2>>7));
                LEDsArrPort=last_value2;
                value2=last_value2;

            }

    }


//---------------------------------------------------------------------
//             function for state 3
//---------------------------------------------------------------------
void wave_def(void)
{
    LEDsArrPort = 0x00;                /* turn LEDs off as soon as PWM starts */
    unsigned int period, duty;
    volatile unsigned int i;

    P2DIR |= BIT7;                      /* P2.7 output */

    while (state == state3) {           /* stay until PB0/PB1 steals control */
        /* pick the current profile */
        if (value3) {                   /* profile A: 4 kHz @ 75 % */
            period = 250;               /* 1 MHz / 4 kHz */
            duty   = 187;               /* 75 %  */
        } else {                        /* profile B: 2 kHz @ 25 % */
            period = 500;               /* 1 MHz / 2 kHz */
            duty   = 125;               /* 25 %  */
        }

        /* HIGH portion */
        P2OUT |= BIT7;
        for (i = 0; i < duty; ++i) ;

        /* LOW portion */
        P2OUT &= ~BIT7;
        for (i = 0; i < (period - duty); ++i) ;
    }

    P2OUT &= ~BIT7;                     /* pin low on exit */
}
void pwm (int Nperiod, int Nduty, int Ncycles){
    //const unsigned int upTime=Nduty;
    //const unsigned int downTime=1-Nduty;
    int i;
    for (i=0; i<Ncycles; i++) {
        P2OUT |= 0x80;
        __delay_cycles(500000);
        P2OUT &= ~0x80;
        __delay_cycles(500000);

    }

}





//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  // t[msec]
    volatile unsigned int i;

    for(i=t; i>0; i--);
}
//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
    enable_interrupts();
    if (LPM_level == 0x00)
      _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01)
      _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02)
      _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
    else if(LPM_level == 0x03)
      _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04)
      _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
}
//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts(){
  _BIS_SR(GIE);
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
  _BIC_SR(GIE);
}
//*********************************************************************
//            Port2 Interrupt Service Rotine
//*********************************************************************
#pragma vector=PORT2_VECTOR
  __interrupt void PBs_handler(void){

    delay(debounceVal);
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
    if(PBsArrIntPend & PB0){
      state = state1;
      PBsArrIntPend &= ~PB0;
        }
        else if(PBsArrIntPend & PB1){
      state = state2;
      PBsArrIntPend &= ~PB1;
        }
        else if (PBsArrIntPend & PB2) {     /* PB2  → stay in state 3 but flip profile */
            value3 ^= 1;                    /* 0 ↔ 1 */
            state   = state3;               /* (already there) */
            PBsArrIntPend &= ~PB2;
        }
//---------------------------------------------------------------------
//            Exit from a given LPM
//---------------------------------------------------------------------
        switch(lpm_mode){
        case mode0:
         LPM0_EXIT; // must be called from ISR only
         break;

        case mode1:
         LPM1_EXIT; // must be called from ISR only
         break;

        case mode2:
         LPM2_EXIT; // must be called from ISR only
         break;

         case mode3:
         LPM3_EXIT; // must be called from ISR only
         break;

         case mode4:
         LPM4_EXIT; // must be called from ISR only
         break;
    }

}
