
#include "LCD.h"
#include "intrinsics.h"
 #include "msp430fg4617.h"
#include "USB.h"
#include "tools.h"
#include "stopwatch.h"
#include "clock.h"
#include "display_manager.h"
#include "event_manager.h"

#pragma vector=TIMERA0_VECTOR
__interrupt void TimerA0_ISR(void)
{ 
 
  em_tick();    //event manager time loop tick
}

#pragma vector=PORT1_VECTOR
 __interrupt void Port1_ISR(void)
{
  em_onPress();
  P1IFG = 0;    //reset interrupt flags
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  //multiply the base quartz frequency (32768) by 100
  SCFQCTL = 100-1;
  CCTL0 = CCIE;
  TACTL = TACLR;
  TACTL = TASSEL_2 + MC_1;
  TACCR0 = 32768-1;
  
  
  int i = 0;
  // wait for xtal to stabilize
  FLL_CTL0 |= XCAP14PF;
  do
  {
    IFG1 &= ~OFIFG;
    for (i = 0x47FF; i > 0; i--);
  }
  while ((IFG1 & OFIFG));
  
  //input output configuration
  P2DIR = 0xF;
  P2OUT = 0;
  P1IES = 0;
  P1IE = 0xF;
  P1IFG = 0;
 
  //init mmodules
  em_init();
  dm_init();
  sw_reset();
  
  __enable_interrupt();	    //globaly enable interrupts
  
  _BIS_SR(LPM0_bits + GIE); //on éteint presque tout pour économiser
  
  for(;;);
}