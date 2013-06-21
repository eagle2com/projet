
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
  em_tick();    //event manager
}

#pragma vector=PORT1_VECTOR
 __interrupt void Port1_ISR(void)
{
  em_onPress();
  P1IFG = 0; //remise à zéro des flags
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P2DIR = 0xF;
  P2OUT = 0;
  
  P1IES = 0;
  P1IE = 0xF;
  P1IFG = 0;

  TACCTL0 = CCIE;       //enable timerA interrupts
  CCR0 = 32768/100 - 1; // f = 32658/(TACCRO+1) -> TACCR0 = 32768/100 - 1;
  TACTL = TASSEL_1 + MC_1;  //TimerA ACLK(32768[Hz]) in up mode
 
  //init mmodules
  em_init();
  dm_init();
  sw_reset();
  
  __enable_interrupt();	    //globaly enable interrupts
  
  _BIS_SR(LPM3_bits + GIE); //on éteint presque tout pour économiser
  
  for(;;);
}