#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

void dm_tick();
void dm_init();
void dm_setDisplayMode(char dmode);
void dm_displayMessage(char* message, unsigned int delay);
#define STOPWATCH 0
#define CLOCK 1
#define MESSAGE 2


#endif