#include "clock.h"
#include "tools.h"

static unsigned var_hs=0,var_hm=0,var_hh=0,var_hc = 0;

void clk_tick()
{
  var_hc++;
   if(var_hc >= 100)
  {
    var_hc = 0;
    var_hs++;
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
  if(var_hh >= 99)
  {
    var_hh = 0;
  }
}

void clk_synchronize(unsigned h, unsigned m, unsigned s, unsigned c)
{
  var_hh = h;
  var_hm = m;
  var_hs = s;
  var_hc = c;
}

char* clk_tostring()
{
  return convert_hhmmss(var_hs, var_hm, var_hh);
}