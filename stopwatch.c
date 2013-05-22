#include "stopwatch.h"
#include "tools.h"s

static unsigned var_cc = 0, var_cs = 0, var_cm = 0, var_ch = 0;
static unsigned running = 0;

void sw_update()
{
  if(running)
    var_cc++;
  
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
    var_cm = 0;
}

void sw_reset()
{
  var_cc = 0, var_cs = 0, var_cm = 0, var_ch = 0;
}

char* sw_getstring()
{
  return convert_mmsscc(var_cc, var_cs, var_cm);
}