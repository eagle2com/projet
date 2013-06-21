#ifndef CLOCK_H
#define CLOCK_H

#define CLK_ERROR_OK 0
#define CLK_ERROR_SYNTAX 1
#define CLK_ERROR_VALUE 2

void clk_tick();  //this function is called on each time tick
void clk_synchronize();
char* clk_tostring();
unsigned char clk_getLED();
char clk_hasChanged(char c);
char clk_setTime(char* clock);


#endif