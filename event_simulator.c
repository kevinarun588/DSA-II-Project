#include <stdio.h>
#include <stdlib.h>
#include "event_simulator.h"

// ------ DSA: MIN-HEAP of Event by time ------
static Event *heapE;  // 1-based
static int sizeE=0, capE=0;

static int earlier(Event a, Event b){
    if(a.time != b.time) return a.time < b.time;
    return a.eventType < b.eventType;
}
static void swapE(Event *a, Event *b){ Event t=*a; *a=*b; *b=t; }

void initEventQueue(int capacity){
    capE=capacity; sizeE=0; heapE=(Event*)malloc(sizeof(Event)*(capE+1));
}
void scheduleEvent(Event e){
    if(sizeE==capE){ printf("Event queue full\n"); return; }
    heapE[++sizeE]=e;
    for(int i=sizeE;i>1 && earlier(heapE[i],heapE[i/2]); i/=2) swapE(&heapE[i],&heapE[i/2]);
}
Event nextEvent(){
    Event none={-1,-1,-1,-1,0};
    if(sizeE==0) return none;
    Event top=heapE[1]; heapE[1]=heapE[sizeE--];
    for(int i=1;;){
        int l=2*i,r=l+1,b=i;
        if(l<=sizeE && earlier(heapE[l],heapE[b])) b=l;
        if(r<=sizeE && earlier(heapE[r],heapE[b])) b=r;
        if(b==i) break; swapE(&heapE[i],&heapE[b]); i=b;
    }
    return top;
}
int isEventQueueEmpty(){ return sizeE==0; }

// ------- super-simple metrics -------
static int assigned=0, completed=0;

void metricsInit(){ assigned=0; completed=0; }
void metricsOnAssign(int time){ (void)time; assigned++; }
void metricsOnComplete(int time){ (void)time; completed++; }
void printFinalStats(){
    printf("\n==== FINAL STATS ====\n");
    printf("Assigned:  %d\n", assigned);
    printf("Completed: %d\n", completed);
}
