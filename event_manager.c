#include "event_manager.h"
#include "intrinsics.h"
#include "msp430fg4617.h"

//P1IFG, P1IN

//how much time passed from the moment a button was down? (short/long press detection)
static unsigned button_timer = 0;


void em_onPress()
{
  if(bouton_timer >= VIBRATION_DELAY)
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
  bouton_timer = 0;
}


void em_tick()
{
  bouton_timer++;
  if(P1IN & 0xF)
    bouton_timer = 0;
}