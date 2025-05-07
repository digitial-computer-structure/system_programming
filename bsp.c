#include  "bsp.h"    // private library - BSP layer

//-----------------------------------------------------------------------------
//           GPIO congiguration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage

  WDTCTL = WDTHOLD | WDTPW;     // Stop WDT

  // LEDs 8-bit Array Port configuration
  LEDsArrPortSel &= ~0xFF;            // GPIO capability
  LEDsArrPortDir |= 0xFF;             // output dir
  LEDsArrPort = 0x00;                 // clear all LEDs

  // Switches Setup
  //SWsArrPortSel &= ~0x0F;
  //SWsArrPortDir &= ~0x0F;

  // PushButtons Setup
    PBsArrPortSel &= ~0xFF;
    PBsArrPortDir &= ~0x7F;
    PBsArrPortDir =  0x80;
    P2REN |=  0x07;            // resistor enable on P2.0–P2.2
    pwm1 |=  0x07;            // pull-up (OUT=1)
    PBsArrIntEdgeSel |= 0x07;          // falling-edge on P2.0–P2.2
    PBsArrIntPend   &= ~0x07;          // clear any pending IFGs on P2.0–P2.2
    PBsArrIntEn      |= 0x07;          // enable interrupts on P2.0–P2.2  _BIS_SR(GIE);                     // enable interrupts globally
}
//-------------------------------------------------------------------------------------
//            Timers congiguration
//-------------------------------------------------------------------------------------
void TIMERconfig(void){

    //write here timers congiguration code
}
//-------------------------------------------------------------------------------------
//            ADC congiguration
//-------------------------------------------------------------------------------------
void ADCconfig(void){

    //write here ADC congiguration code
}








