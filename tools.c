#include "tools.h"

static buffer[9] = 0;

//returs the ascii code of single digit number
unsigned char itoa(unsigned i)
{
  return '0'+i;
}

char* convert_mmsscc(unsigned var_cc, unsigned var_cs, unsigned var_cm)
{ 
  buffer[8] = 0;
  buffer[7] = itoa(var_cc%10);
  buffer[6] = itoa(var_cc/10);
  buffer[5] = '.';
  buffer[4] = itoa(var_cs%10);
  buffer[3] = itoa(var_cs/10);
  buffer[2] = '.';
  buffer[1] = itoa(var_cm%10);
  buffer[0] = itoa(var_cm/10);
 
  return buffer;
}

char* convert_hhmmss(unsigned var_hs, unsigned var_hm, unsigned var_hh)
{ 
  buffer[8] = 0;
  buffer[7] = itoa(var_hs%10);
  buffer[6] = itoa(var_hs/10);
  buffer[5] = '.';
  buffer[4] = itoa(var_hm%10);
  buffer[3] = itoa(var_hm/10);
  buffer[2] = '.';
  buffer[1] = itoa(var_hh%10);
  buffer[0] = itoa(var_hh/10);
 
  return buffer;
}