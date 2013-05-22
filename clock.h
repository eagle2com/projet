#ifndef CLOCK_H
#define CLOCK_H

void clk_tick();  //this function is called when an interrupt is issued
void clk_synchronize(unsigned h, unsigned m, unsigned s, unsigned c);
char* clk_tostring();




#endif