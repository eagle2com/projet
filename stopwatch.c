#include "stopwatch.h"
#include "tools.h"
#include "display_manager.h"

static unsigned var_cc = 0, var_cs = 0, var_cm = 0, var_ch = 0;
static unsigned char running = 0;

void sw_tick()
{
  if(running)
  {
    dm_swChanged();
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

void sw_start()
{
  running = 1;
}

void sw_stop()
{
  running = 0;
}

char sw_isRunning()
{
  return running;
}

char sw_toggle()
{
  running = !running;
  return running;
}

void sw_reset()
{
  var_cc = 0, var_cs = 0, var_cm = 0, var_ch = 0;
  dm_swChanged();
}

char* sw_tostring()
{
  return convert_mmsscc(var_cc, var_cs, var_cm);
}