#include "display_manager.h"
#include "LCD.h"
#include "intrinsics.h"
#include "msp430fg4617.h"
#include "clock.h"
#include "stopwatch.h"


static char display_mode = CLOCK;
static unsigned lcd_timer = 0;
static const unsigned lcd_delay = 5;   //in 1/100 of a second

static unsigned message_delay = 0;
static unsigned char state_before_message = CLOCK;


/* dm_init()
* - initializes the display components (here the LCD and LEDs)
* - prints greeting message
*/
void dm_init()
{
  P2DIR = 0xF;
  P2OUT = 0x0;
  LCD_init();
 
  dm_setDisplayMode(CLOCK);
  dm_displayMessage("BONJOUR",100);
}

/* dm_setDisplayMode(dmode)
* - tells the display manager to show either the clock or stopwatch
*/
void dm_setDisplayMode(char dmode)
{
  display_mode = dmode;
  switch(display_mode)
  {
  case CLOCK:
    P2OUT = 1;
    clk_hasChanged(1);  //a refresh of the screen is needed
    break;
  case STOPWATCH:    
    sw_hasChanged(1);   //a refresh of the screen is needed
    break;
  }
}

/* dm_tick()
* - handles the the refresh of all the display elements
* - changes the values on the LCD only when needed
*/

void dm_tick()
{ 
  lcd_timer++;
  if(lcd_timer >= lcd_delay)
  {
    switch(display_mode)
    {
    case CLOCK:
      P2OUT = clk_getLED();
      if(clk_hasChanged(0))
      {
        LCD_print(clk_tostring());
        LCD_dot(2);
        LCD_dot(4);
        LCD_dot(6);
        lcd_timer = 0;
      }
      break;
    case STOPWATCH:
      P2OUT = sw_getLED();
      if(sw_hasChanged(0))
      {
        LCD_print(sw_tostring());
        LCD_dot(2);
        LCD_dot(4);
        LCD_dot(6);
        lcd_timer = 0;
      }
      break;
      
    case MESSAGE:
      message_delay--;
      if(message_delay <= 0)
      {
        display_mode = state_before_message;
        sw_hasChanged(1);
        clk_hasChanged(1);
      }
      break;
  }
  }
}

/* dm_displayMessage(message,delay)
* - displays a message for "delay" centiseconds
* - returns to the previous display mode after delay has passed
*/

void dm_displayMessage(char* message, unsigned delay)
{
  message_delay = delay;
  if(display_mode != MESSAGE)
    state_before_message = display_mode;
  display_mode = MESSAGE;
  LCD_clear();
  LCD_print(message);
}