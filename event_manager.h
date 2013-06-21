#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#define VIBRATION_DELAY 9

void em_onPress();      //callback method handling events
void em_tick();         //a standard tick method, called on each 10[ms] "frame"
void em_init();

#endif