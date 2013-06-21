#include "clock.h"
#include "tools.h"
#include "display_manager.h"

static unsigned var_hs=0, var_hm=0, var_hh=0, var_hc = 0;
static unsigned var_hm_temp = 0, var_hh_temp = 0;

static unsigned LED = 1;
static char has_changed = 1;

/* clk_tick()
* - increments the centiseconds and eventualy seconds, minutes and hours
*/
void clk_tick()
{
  var_hc++;
  //animate the leds
  if (var_hc % 50 == 0)
    LED = LED << 1;
  if (!(LED & 0xF))
    LED = 1;
  
  //update the clock values
  if (var_hc >= 100)
  {
    var_hc = 0;
    var_hs++;
    has_changed = 1;
  }
  if (var_hs >= 60)
  {
    var_hs = 0;
    var_hm++;
  }
  if (var_hm >= 60)
  {
    var_hm = 0;
    var_hh++;
  }
  if (var_hh >= 24)
  {
    var_hh = 0;
  }
}
/* clk_getLED()
* - returns the LED mask used by the display manager
*/
unsigned char clk_getLED()
{
  return LED;
}

/* clk_synchronize()
* - synchronizes the clock with the temporary values received from usb
*/
void clk_synchronize()
{
  var_hh = var_hh_temp;
  var_hm = var_hm_temp;
  var_hs = 0;
  var_hc = 0;
}

/* clk_setTime(clock)
* - converts the ascii string received from the usb link into integer values
* - checks for errors in syntax and bounds
* - returns and error code
*/
char clk_setTime(char* clock)
{ 
  if (strlen(clock) < 5)
  {
    return CLK_ERROR_SYNTAX;
  }
  //get the corresponding value of the digits from ascii
  unsigned n1 = clock[0] - '0';
  unsigned n2 = clock[1] - '0';
  unsigned n3 = clock[3] - '0';
  unsigned n4 = clock[4] - '0';
  
  //syntax and boundaries check
  /******************/
  char err = 0;
  if (n1*10+n2 > 23)
    err = 1;
  
  if (n3*10+n4 > 59)
    err = 1;
  
  if (err)
  {
    return CLK_ERROR_VALUE;
  }
  /******************/
      
  //store the values into temporary variables
  var_hh_temp = 10*n1+n2;
  var_hm_temp = 10*n3+n4;
  
  return CLK_ERROR_OK;
}

/* clk_hasChanged(c)
* - did the clock value changed since the last time?
* - c allows the user to force a refresh on the next call
*/

char clk_hasChanged(char c)
{
  char temp = has_changed;
  has_changed = c;
  return temp;
}

/* clk_tostring()
* - returns the current time converted into an ascii string
*/
char* clk_tostring()
{
  return convert_hhmmss(var_hs, var_hm, var_hh);
}