#include "event_manager.h"
#include "intrinsics.h"
#include "msp430fg4617.h"

//P1IFG, P1IN

//how much time passed from the moment a button was down? (short/long press detection)
static unsigned button_timer = 0;
enum states = {CLK, SW};
enum buttons = {BUTTON1-SHORT,BUTTON2-SHORT,BUTTON3-SHORT,BUTTON4-SHORT};
static char state_machine[][] = 
{
/*STATE | BUTTON1-SHORT | BUTTON2-SHORT*/
/*--------------------------------------*/
/*CLK*/  {  SW         ,        CLK},
/*SW */  {  CLK        ,         SW}
};

static char state = CLK;

void em_onPress()
{
  if(bouton_timer >= VIBRATION_DELAY)
  {
    switch(state)
    {
    case CLK:
      break;
    case SW:
      if(P1IFG & 0x4)
      {
        sw_toggle();
      }
      break;
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