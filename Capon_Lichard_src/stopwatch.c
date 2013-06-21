#include "stopwatch.h"
#include "tools.h"
#include "display_manager.h"

static unsigned var_cc = 0, var_cs = 0, var_cm = 0, var_ch = 0;
static unsigned char running = 0;
static unsigned char LED = 0x0;
static unsigned ntick=0;
static char has_changed = 1;


/* sw_tick()
* - increments the centiseconds and eventualy seconds and minutes
*/
void sw_tick()
{
  ntick++;
  if (running)
  {
      if (ntick%12==0)
      {
        LED = LED << 1;
        if (!(LED & 0xF))
          LED = 1;
      }
  }
  else
  {
    if (ntick % 50 == 0)
      LED ^= 0xF;
  }
     
  if (running)
  {
    has_changed = 1; //set the flag to 1 (indicates a display refresh is needed)
    var_cc++;
  }
  
  if (var_cc >= 100)
  {
    var_cc = 0;
    var_cs++;
  }
  if (var_cs >= 60)
  {
    var_cs = 0;
    var_cm++;
  }
  if (var_cm >= 60)
  {
    var_cm = 0;
    var_ch++;
  }
}


/* sw_getLED()
* - returns the LED mask used by the display manager
*/
unsigned char sw_getLED()
{
  return LED;
}


/* sw_start()
* - starts the stopwatch
*/
void sw_start()
{
  running = 1;
}


/* sw_stop()
* - stops the stopwatch
*/
void sw_stop()
{
  running = 0;
}


/* sw_isRunning()
* - returns the state of the stopwatch
*/
char sw_isRunning()
{
  return running;
}


/* sw_toggle()
* - toggles the state of the stopwatch
*/
char sw_toggle()
{
  running = !running;
  if (running)
  {
    LED = 0x1;
  }
  else
    LED = 0xF;
  return running;
}


/* sw_reset()
* - resets the time value to 0, but does not stop the stopwatch
*/
void sw_reset()
{
  var_cc = 0, var_cs = 0, var_cm = 0, var_ch = 0;
  has_changed = 1;
  LED = 0x0;
}


/* sw_hasChanged(c)
* - did the stopwatch value changed since the last time?
* - c allows the user to force a refresh on the next call
*/
char sw_hasChanged(char c)
{
  char temp = has_changed;
  has_changed = 1;
  return temp;
}


/* sw_tostring()
* - returns the current time converted into an ascii string
*/
char* sw_tostring()
{
  return convert_mmsscc(var_cc, var_cs, var_cm);
}