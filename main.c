
#include "LCD.h"
#include "intrinsics.h"
 #include "msp430fg4617.h"
#include "USB.h"
#include "tools.h"
#include "stopwatch.h"
#include "clock.h"
#include "display_manager.h"



unsigned lcd_timer = 0;
const unsigned lcd_delay = 10;

unsigned mode = HEURE;

unsigned bouton_timer = 9;


void update_timer()
{
  sw_tick();
  clk_tick();
}


#pragma vector=TIMERA0_VECTOR
__interrupt void TimerA0_ISR(void)
{
  bouton_timer++;
  if(P1IN & 0xF)
    bouton_timer = 0;
  
  update_timer();
  lcd_timer++;
  
  //on ne veut reafficher la valeur sur
  if(lcd_timer > lcd_delay)
  {
    lcd_timer = 0;
    switch(mode)
    {
    case CHRONO:
        LCD_print(sw_tostring());
      break;
    case HEURE:
       LCD_print(clk_tostring());
      break;
    }
  }
  
  P2OUT |= USB_isConnected();
  
}

#pragma vector=PORT1_VECTOR
 __interrupt void Port1_ISR(void)
{
  if(bouton_timer >= 9)
  {
    if(P1IFG & 0x1)
    {
      mode = HEURE;
    }
    else if(P1IFG & 0x2)
    {
      mode = CHRONO;
    }
    else if(P1IFG & 0x4) //pause du chrono
    {
      sw_toggle();
    }
    else if(P1IFG & 0x8) //ràz du chrono
    {
      sw_reset();
    }
  }
  
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

  TACCTL0 = CCIE;     //autoriser les interruptions du timer
  CCR0 = 32768/100 - 1; //freq_quartz/freqsouhaitée -> CCRO+1 = FREQQuartz/f -> f = FREQSouhaitée
  //on veut 100Hz vu que le compteur doit montrer les centièmes de seconde
  TACTL = TASSEL_1 + MC_1;  //on veut utiliser le quartz
  
  dm_init();
  
  
  
  sw_reset();
  clk_synchronize(0,0,0,0); ///use the usb communication to synchronize it corretly lately
  
  __enable_interrupt();	//autoriser le interruptions
  _BIS_SR(LPM3_bits + GIE); //on éteint presque tout pour économiser
  
  for(;;);
}