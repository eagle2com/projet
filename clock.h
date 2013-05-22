#ifndef CLOCK_H
#define CLOCK_H

void clk_update();  //this function is called when an interrupt is issued
void clk_synchronize(unsigned h, unsigned m, unsigned s, unsigned c);





#endif