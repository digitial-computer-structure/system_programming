#include  "api.h"         // private library - API layer
#include  "app.h"         // private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;
int value1=0; //saves the last value on state1
int count_dir=1; //"1" means counting up
unsigned char value2=1;//saves the last value on state2
int value3=1;//saves the mode of state 3


void main(void){

  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();

  while(1){
    switch(state){
      case state0:
        clrLEDs();
        enterLPM(lpm_mode);
        break;

      case state1:
        disable_interrupts();
        incdecLEDs();
       // delay(LEDs_SHOW_RATE);  // delay of 62.5 [ms]
        state=state0;
        enable_interrupts();
        break;

      case state2:
        disable_interrupts();
        skip_leds();
        //delay(LEDs_SHOW_RATE);      // delay of 62.5 [ms]
        state=state0;
        enable_interrupts();
        break;

      case state3:
        enable_interrupts();
        wave_def ();
        break;

    }
  }
}





