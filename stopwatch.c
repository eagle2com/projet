#include "stopwatch.h"
#include "tools.h"
#include "display_manager.h"

//module variables
//*****************************************************************************
static unsigned var_cc = 0, var_cs = 0, var_cm = 0, var_ch = 0;
static unsigned char running = 0;
static unsigned char LED = 0x0;
static unsigned ntick=0;
static char has_changed = 1;


/*****************************************************************************




******************************************************************************/

void sw_tick()
{
  ntick++;
  if(running)
  {
      if(ntick%12==0)
      {
        LED = LED << 1;
        if(!(LED & 0xF))
          LED = 1;
      }
  }
  else
  {
    if(ntick % 50 == 0)
      LED ^= 0xF;
  }
     
  if(running)
  {
    has_changed = 1;
    var_cc++;
  }
  
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
  {
    var_cm = 0;
    var_ch++;
  }
}

//*****************************************************************************

unsigned char sw_getLED()
{
  return LED;
}

//*****************************************************************************

void sw_start()
{
  running = 1;
}

//*****************************************************************************

void sw_stop()
{
  running = 0;
}

//*****************************************************************************

char sw_isRunning()
{
  return running;
}

//*****************************************************************************

char sw_toggle()
{
  running = !running;
  if(running)
  {
    LED = 0x1;
  }
  else
    LED = 0xF;
  return running;
}

//*****************************************************************************

void sw_reset()
{
  var_cc = 0, var_cs = 0, var_cm = 0, var_ch = 0;
  has_changed = 1;
  LED = 0x0;
}

//*****************************************************************************

char sw_hasChanged(char c)  //do we reset the indicator after the call?
{
  char temp = has_changed;
  has_changed = 1;
  return temp;
}

//*****************************************************************************

char* sw_tostring()
{
  return convert_mmsscc(var_cc, var_cs, var_cm);
}