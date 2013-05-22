#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H


void dm_clkChanged();
void dm_swChanged();
void dm_tick();
void dm_init();
void dm_setDisplayMode(char dmode);
#define STOPWATCH 0
#define CLOCK 1


#endif