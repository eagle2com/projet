#include "display_manager.h"
#include "LCD.h"
#include "intrinsics.h"
#include "msp430fg4617.h"
#include "clock.h"
#include "stopwatch.h"

static char sw_changed = 0;
static char clk_changed = 0;
static char display_mode = CLOCK;
static unsigned lcd_timer = 0;
static const lcd_delay = 25;

void dm_init()
{
  LCD_init();
}

void dm_clkChanged()
{
  clk_changed = 1;
}

void dm_swChanged()
{
  sw_changed = 1;
}

void dm_tick()
{
  lcd_timer++;
  if(lcd_timer >= lcd_delay)
  {
    switch(display_mode)
    {
    case CLOCK:
      if(clk_changed)
      {
        LCD_print(clk_tostring());
        clk_changed = 0;
        lcd_timer = 0;
      }
      break;
    case STOPWATCH:
      if(sw_changed)
      {
        LCD_print(sw_tostring());
        sw_changed = 0;
        lcd_timer = 0;
      }
      break;
    }
  }
}