#include "event_manager.h"
#include "intrinsics.h"
#include "msp430fg4617.h"
#include "display_manager.h"
#include "stopwatch.h"
#include "clock.h"

//P1IFG, P1IN

//how much time passed from the moment a button was down? (short/long press detection)
static unsigned button_timer = 0;
#define CLK 0
#define SW 1
#define USB 2

#define BUTTON1_SHORT 0
#define BUTTON2_SHORT 1
#define BUTTON3_SHORT 2
#define BUTTON4_SHORT 3

static const char state_machine[3][4] = 
{
/*STATE | BUTTON1-SHORT | BUTTON2-SHORT | BUTTON3-SHORT | BUTTON4-SHORT*/
/*--------------------------------------*/
/*CLK*/  {  SW         ,        CLK         ,CLK           , CLK},
/*SW */  {  CLK        ,        SW          ,SW            , SW},
/*USB*/  {  CLK        ,        USB         , USB          , USB}
};

static char state = CLK;

void em_onPress()
{
  if(button_timer >= VIBRATION_DELAY)
  {
    char button = -1;
    switch(P1IFG)
    {
    case 0x1:
      button = BUTTON1_SHORT;
      break;
    case 0x2:
      button = BUTTON2_SHORT;
      break;
    case 0x4:
      button = BUTTON3_SHORT;
      break;
    case 0x8:
      button = BUTTON4_SHORT;
      dm_displayMessage("cacaprout",500);
      break;
    }
    
    switch(state)
    {
    case CLK:
      switch(button)
      {
      case BUTTON1_SHORT:
        dm_setDisplayMode(STOPWATCH);
        break;
      case BUTTON2_SHORT:
        break;
      case BUTTON3_SHORT:
        break;
      case BUTTON4_SHORT:
        break;
      }
      break;
    case SW:
      switch(button)
      {
      case BUTTON1_SHORT:
        dm_setDisplayMode(CLOCK);
        break;
      case BUTTON2_SHORT:
        sw_toggle();
        break;
      case BUTTON3_SHORT:
        sw_reset();
        break;
      case BUTTON4_SHORT:
        break;
      }
      break;
    case USB:
      char com[9] = {0};
       while (USB_readyToRead() && USB_readyToWrite())
      {  
        usb_gets (com);
        LCD_print(com);
      }

      break;
    }
    state = state_machine[state][button];
  }
  button_timer = 0;
}


void em_tick()
{
  button_timer++;
  if(P1IN & 0xF)
    button_timer = 0;
}