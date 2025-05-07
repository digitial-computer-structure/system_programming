#ifndef _halGPIO_H_
#define _halGPIO_H_

#include  "../header/bsp.h"         // private library - BSP layer
#include  "../header/app.h"         // private library - APP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable
extern int value1; //saves the last value on state1
extern int count_dir; //"1" means counting up
extern unsigned char value2; //saves the last value on state 2
extern int value3;
extern enum FSMstate next_state;

extern void sysConfig(void);
extern void print2LEDs(unsigned char);
extern void clrLEDs(void);
extern void toggleLEDs(char);
extern void setLEDs(char);
//extern unsigned char readSWs(void);
extern void delay(unsigned int);
extern void incdecLEDs();
extern void skip_leds();
extern void wave_def();
extern void pwm();
extern void enterLPM(unsigned char);
extern void incLEDs(char);
extern void enable_interrupts();
extern void disable_interrupts();

extern __interrupt void PBs_handler(void);

#endif

