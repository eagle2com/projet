#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

void dm_tick();         //a standard tick method, called on each 10[ms] "frame"
void dm_init();         //initializes the module
void dm_setDisplayMode(char dmode);             //changes the display mode
void dm_displayMessage(const char* message, unsigned int delay); 

//values assigned to each display mode
#define STOPWATCH 0
#define CLOCK 1
#define MESSAGE 2


#endif