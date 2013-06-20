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

static const char state_machine[2][4] = 
{
/*STATE | BUTTON1-SHORT | BUTTON2-SHORT | BUTTON3-SHORT | BUTTON4-SHORT*/
/*--------------------------------------*/
/*CLK*/  {  SW         ,        CLK         ,CLK           , CLK},
/*SW */  {  CLK        ,        SW          ,SW            , SW}};
/*USB*/ // {  CLK        ,        USB         , USB          , USB}
//};

static char state = CLK;

static unsigned USB_CONNECTED = 0;
static char S_USB_ON[] = "USB_ON  ";
static char S_USB_OFF[] = "USB_OFF ";

/* em_onPress(void)
* is called on every button interrupt on port1 
* - detects which button was pressed
* - handles transitions between different states using a state machine
* - can be easily extended in order be able to distinguish between long and 
*       short button presses
* - handles the button anti-bounce, it is allowed to activate only once in the
*       defined interval VIBRATION_DELAY ( 1/100's of a second )
*/
void em_onPress()
{
  if (button_timer >= VIBRATION_DELAY)
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
      if(USB_isConnected())
      {
        char usb_buffer[6] = {0};
        char SET_OK = 0;
        while(!SET_OK)
        {
          dm_displayMessage("SYNC",100);
          if(USB_readyToWrite())
          {
            usb_puts("\033[2J\033[;H");     //clear the terminal
            usb_puts("Write the correct time now:(HH:MM) \n");
            while(!USB_readyToRead());
            usb_gets(usb_buffer);
            while(!USB_readyToWrite());
            usb_puts("\033[2J\033[;H");
            dm_displayMessage(usb_buffer,100);
            
            char error = clk_setTime(usb_buffer);
            if(error == CLK_ERROR_OK)
              SET_OK = 1;
            else if(error == CLK_ERROR_SYNTAX)
            {
              dm_displayMessage("SYNT ERR",100);
            }
            else if(error == CLK_ERROR_VALUE)
            {
              dm_displayMessage("VAL ERR",100);
            }
          }
        }
        //now we have to wait for the sync command: I
        char SYNC_OK = 0;
        while(!SYNC_OK)
        {
           while(!USB_readyToWrite());
            usb_puts("now enter I when ready to synchronize");
          while(!USB_readyToRead());
          usb_gets(usb_buffer);
          if(usb_buffer[0] == 'I')
          {
            clk_synchronize();
          }
        }
      }
      else
      {
        dm_displayMessage("NO USB",100);
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

/* em_tick(void)
* this is the method called on every tick of the update loop
* - displays information about usb state change
* - handles button timer incrementation and reset
* - calls all the other modules that are dependant on the update loop
*/
void em_tick()
{
  if(USB_isConnected() != USB_CONNECTED)
  {
    // display message about USB connectivity
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
  
  
  sw_tick();    //stopwatch
  clk_tick();   //clock
  dm_tick();    //display manager
}