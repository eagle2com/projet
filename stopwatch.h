#ifndef STOPWATCH_H
#define STOPWATCH_H


void sw_tick(); //this function is called when an interrupt is issued
void sw_reset();  //set all to zero
char* sw_tostring();   //returns a pointer to the stopwatch string
void sw_start();
void sw_stop();
char sw_isRunning();
char sw_toggle();
unsigned char sw_getLED();














#endif