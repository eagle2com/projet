#ifndef CLOCK_H
#define CLOCK_H

void clk_tick();  //this function is called when an interrupt is issued
void clk_synchronize(char* clock);
char* clk_tostring();
unsigned char clk_getLED();



#endif