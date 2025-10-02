#ifndef EVENT_SIMULATOR_H
#define EVENT_SIMULATOR_H
#include "structures.h"

// DSA USED: Min-Heap by Event.time

void  initEventQueue(int capacity);
void  scheduleEvent(Event e);  // O(log n)
Event nextEvent();             // O(log n)
int   isEventQueueEmpty();     // O(1)

// Very simple metrics (arrays/counters)
void  metricsInit();
void  metricsOnAssign(int time);
void  metricsOnComplete(int time);
void  printFinalStats();

#endif
