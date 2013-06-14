#include "clock.h"
#include "tools.h"
#include "display_manager.h"

static unsigned var_hs=0,var_hm=0,var_hh=0,var_hc = 0;
static unsigned LED = 1;
static char has_changed = 1;

void clk_tick()
{
  var_hc++;
  //animate the leds
  if(var_hc % 50 == 0)
    LED = LED << 1;
  if(!(LED & 0xF))
    LED = 1;
  
  //update the clock values
   if(var_hc >= 100)
  {
    var_hc = 0;
    var_hs++;
    has_changed = 1;
  }
  if(var_hs >= 60)
  {
    var_hs = 0;
    var_hm++;
  }
  if(var_hm >= 60)
  {
    var_hm = 0;
    var_hh++;
  }
  if(var_hh >= 24)
  {
    var_hh = 0;
  }
}

unsigned char clk_getLED()
{
  return LED;
}

void clk_synchronize()
{
  
}

char clk_setTime(char* clock)
{ 
  if(strlen(clock) < 4)
  {
    dm_displayMessage("SYNT ERR",100);
    return CLK_ERROR_SYNTAX;
  }
  unsigned n1 = clock[0] - '0';
  unsigned n2 = clock[1] - '0';
  unsigned n3 = clock[2] - '0';
  unsigned n4 = clock[3] - '0';
  char err = 0;
  
  if(n1*10+n2 > 23)
    err = 1;
  
  if(n3*10+n4 > 59)
    err = 1;
  
  if(err)
  {
    dm_displayMessage("VAL ERR",100);
    return CLK_ERROR_VALUE;
  }
      
  var_hh = 10*n1+n2;
  var_hm = 10*n3+n4;
  var_hs = 0;
  var_hc = 0;
  
  return CLK_ERROR_OK;
}

char clk_hasChanged(char c) //the state it should have after the call, DETAIIILLZZZZZZ
{
  char temp = has_changed;
  has_changed = c;
  return temp;
}

char* clk_tostring()
{
  return convert_hhmmss(var_hs, var_hm, var_hh);
}