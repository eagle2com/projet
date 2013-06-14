#ifndef STOPWATCH_H
#define STOPWATCH_H


void sw_tick();         //a standard tick method, called on each 10[ms] "frame"
void sw_reset();        //resets the stopwatch to 0, does not stop it
char* sw_tostring();    //returns a pointer to the string to display
void sw_start();        //starts the stopwatch
void sw_stop();         //stops the stopwatch
char sw_isRunning();    //returns the state of the stopwatch
char sw_toggle();       //toggles the state of the stopwatch
unsigned char sw_getLED();  //returns the bit pattern needed to set LEDs
char sw_hasChanged(char c); //has the stopwatch value changed since the last
                            //call? 


#endif