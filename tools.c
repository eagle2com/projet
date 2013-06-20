#include "tools.h"

static char buffer[9] = {0};

//returs the ascii code of single digit
unsigned char itoa(unsigned i)
{
  return '0'+i;
}

/* convert_mmsscc(var_cc, var_cs, var_cm)
* - handles conversion of time from integer values into ascii strings
* - format: 1/100, seconds, minutes
* - dramaticaly reduces compiled size because we avoid the stdlib
*/
char* convert_mmsscc(unsigned var_cc, unsigned var_cs, unsigned var_cm)
{ 
  buffer[8] = 0;
  buffer[7] = itoa(var_cc%10);
  buffer[6] = itoa(var_cc/10);
  buffer[5] = itoa(var_cs%10);
  buffer[4] = itoa(var_cs/10);
  buffer[3] = itoa(var_cm%10);
  buffer[2] = itoa(var_cm/10);
  buffer[1] = 'W';
  buffer[0] = 'S';
 
  return buffer;
}

/* convert_hhmmss(var_hs, var_hm, var_hh)
* - handles conversion of time from integer values into ascii strings
* - format: seconds, minutes, hours
* - dramaticaly reduces compiled size because we avoid the stdlib
*/

char* convert_hhmmss(unsigned var_hs, unsigned var_hm, unsigned var_hh)
{ 
  buffer[8] = 0;
  buffer[7] = itoa(var_hs%10);
  buffer[6] = itoa(var_hs/10);
  buffer[5] = itoa(var_hm%10);
  buffer[4] = itoa(var_hm/10);
  buffer[3] = itoa(var_hh%10);
  buffer[2] = itoa(var_hh/10);
  buffer[1] = 'K';
  buffer[0] = 'C';
 
  return buffer;
}