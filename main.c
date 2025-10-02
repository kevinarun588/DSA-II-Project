#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "task_scheduler.h"
#include "server_manager.h"
#include "task_mapper.h"
#include "event_simulator.h"

// Main simulation with USER INPUT for tasks

int main(){
    int N_SERVERS, N_TASKS;

    // ---- get basic simulation parameters ----
    printf("Enter number of servers: ");
    scanf("%d", &N_SERVERS);

    printf("Enter number of tasks: ");
    scanf("%d", &N_TASKS);

    // ---- init modules ----
    initTaskScheduler(N_TASKS + 10);     // Rafan (Max-Heap of tasks)
    initServerManager(N_SERVERS);        // Ruel  (Min-Heap of servers)
    mapperInit(257);                     // Saivarad (HashMap)
    initEventQueue(N_TASKS*3 + 10);      // Kevin (Min-Heap of events)
    metricsInit();

    // ---- input tasks from USER ----
    for(int i=1; i<=N_TASKS; i++){
        Task t;
        t.taskId = i;  // unique ID (could also be user-provided)

        printf("\n--- Enter details for Task %d ---\n", i);

        printf("Priority (1-10): ");
        scanf("%d", &t.priority);

        printf("Size (processing time, e.g. 2-8): ");
        scanf("%d", &t.size);

        printf("Deadline: ");
        scanf("%d", &t.deadline);

        // Insert into Task Scheduler (Max-Heap)
        insertTask(t);

        // Ask for arrival time
        int arrivalTime;
        printf("Arrival time: ");
        scanf("%d", &arrivalTime);

        // Schedule an ARRIVAL event
        Event a = { .time=arrivalTime, .eventType=ARRIVAL, 
                    .taskId=t.taskId, .serverId=-1, .duration=0 };
        scheduleEvent(a);
    }

    // ---- simulation loop ----
    while(!isEventQueueEmpty()){
        Event e = nextEvent();

        if(e.eventType == ARRIVAL){
            if(isTaskQueueEmpty()){ continue; }
            Task t = extractHighestPriority();

            int sid = getLeastLoadedId();           // pick least-loaded server
            mapTaskToServer(t.taskId, sid);         // map task -> server
            updateServerLoad(sid, t.size);          // increase load
            metricsOnAssign(e.time);                // record metrics

            // Schedule completion
            Event c = { .time = e.time + t.size, .eventType = COMPLETION,
                        .taskId = t.taskId, .serverId = sid, .duration = t.size };
            scheduleEvent(c);

            printf("[t=%d] ARRIVAL: Task %d (prio=%d,size=%d) -> Server %d (new load)\n",
                    e.time, t.taskId, t.priority, t.size, sid);
        }
        else if(e.eventType == COMPLETION){
            updateServerLoad(e.serverId, -e.duration);  // reduce load
            metricsOnComplete(e.time);
            printf("[t=%d] COMPLETE: Task %d finished on Server %d\n",
                    e.time, e.taskId, e.serverId);
        }
        // MIGRATION path optional for v1
    }

  
    printFinalStats();
    return 0;
}
