
#include "LCD.h"
#include "intrinsics.h"
 #include "msp430fg4617.h"
#include "USB.h"

#define CHRONO 0
#define HEURE 1

unsigned char buffer[9] = {0};

unsigned var_cc = 0,var_cs = 0, var_cm = 0;
unsigned var_hs=0,var_hm=0,var_hh=0,var_hc = 0;

unsigned lcd_timer = 0;
const unsigned lcd_delay = 10;

unsigned mode = HEURE;
unsigned chrono_on = 1;

unsigned bouton_timer = 9;

unsigned char itoa(unsigned i)
{
  return '0'+i;
}

char* convert_mmsscc()
{ 
  buffer[8] = 0;
  buffer[7] = itoa(var_cc%10);
  buffer[6] = itoa(var_cc/10);
  buffer[5] = '.';
  buffer[4] = itoa(var_cs%10);
  buffer[3] = itoa(var_cs/10);
  buffer[2] = '.';
  buffer[1] = itoa(var_cm%10);
  buffer[0] = itoa(var_cm/10);
 
  return buffer;
}

char* convert_hhmmss()
{ 
  buffer[8] = 0;
  buffer[7] = itoa(var_hs%10);
  buffer[6] = itoa(var_hs/10);
  buffer[5] = '.';
  buffer[4] = itoa(var_hm%10);
  buffer[3] = itoa(var_hm/10);
  buffer[2] = '.';
  buffer[1] = itoa(var_hh%10);
  buffer[0] = itoa(var_hh/10);
 
  return buffer;
}

void update_timer()
{
  if(chrono_on)
    var_cc++;
  if(var_cc >= 100)
  {
    var_cc = 0;
    var_cs++;
  }
  if(var_cs >= 60)
  {
    var_cs = 0;
    var_cm++;
  }
  if(var_cm >= 60)
    var_cm = 0;
  
  var_hc++;
   if(var_hc >= 100)
  {
    var_hc = 0;
    var_hs++;
  }
  if(var_hs >= 60)
  {
    var_hs = 0;
    var_hm++;
  }
  if(var_hm >= 60)
  {
    var_hm = 0;
    var_hh++;
  }
  if(var_hh >= 99)
  {
    var_hh = 0;
  }
  
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
       LCD_print(convert_mmsscc());
      break;
    case HEURE:
       LCD_print(convert_hhmmss());
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
      chrono_on = !chrono_on;
    }
    else if(P1IFG & 0x8) //r�z du chrono
    {
      var_cc = 0,var_cs = 0, var_cm = 0;
    }
  }
  
  P1IFG = 0; //remise � z�ro des flags
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P2DIR = 0xF;
  P2OUT = 0;
  
  LCD_init();
  
  P1IES = 0;
  P1IE = 0xF;
  P1IFG = 0;

  TACCTL0 = CCIE;     //autoriser les interruptions du timer
  CCR0 = 32768/100 - 1; //freq_quartz/freqsouhait�e -> CCRO+1 = FREQQuartz/f -> f = FREQSouhait�e
  //on veut 100Hz vu que le compteur doit montrer les centi�mes de seconde
  TACTL = TASSEL_1 + MC_1;  //on veut utiliser le quartz
  
  P2OUT = 0; 	//on �teint les LEDs
  
  __enable_interrupt();	//autoriser le interruptions
  _BIS_SR(LPM3_bits + GIE); //on �teint presque tout pour �conomiser
  
  for(;;);
  
  

  return 0;
}