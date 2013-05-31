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

static unsigned USB_CONNECTED = 0;
static char S_USB_ON[] = "USB_ON  ";
static char S_USB_OFF[] = "USB_OFF ";

void em_onPress()
{
  if(button_timer >= VIBRATION_DELAY)
  {
    char button = 0;
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
      dm_displayMessage("SYNC",100);
      if(USB_readyToWrite())
      {
        usb_puts("\033[2J\033[;H");
        usb_puts("Write the correct time now: \n");
        char usb_buffer[5] = {0};
        while(!USB_readyToRead());
        usb_gets(usb_buffer);
        while(!USB_readyToWrite());
        usb_puts("\033[2J\033[;H");
        dm_displayMessage(usb_buffer,100);
        clk_synchronize(usb_buffer);
      }
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

      break;
    }
    state = state_machine[state][button];
  }
  button_timer = 0;
}


void em_tick()
{
  if(USB_isConnected() != USB_CONNECTED)
  {
    dm_displayMessage(USB_CONNECTED?S_USB_OFF:S_USB_ON,100);
    if(USB_isConnected())
    {
      USB_init();
      dm_displayMessage("INIT OK",100);
    }
  }
  USB_CONNECTED = USB_isConnected();
  
  button_timer++;
  if(P1IN & 0xF)
    button_timer = 0;
  
  //P2OUT |= USB_readyToWrite()<<1;
}