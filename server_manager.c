#include <stdio.h>
#include <stdlib.h>
#include "server_manager.h"

// ------ DSA: MIN-HEAP of Server by load ------
static Server heapS[MAX_SERVERS+1]; // 1-based
static int    posById[MAX_SERVERS]; // serverId -> heap index
static int    nS = 0;

static int less(Server a, Server b){
    if(a.load != b.load) return a.load < b.load;
    return a.serverId < b.serverId;
}
static void swapS(int i,int j){
    Server t=heapS[i]; heapS[i]=heapS[j]; heapS[j]=t;
    posById[heapS[i].serverId]=i; posById[heapS[j].serverId]=j;
}
static void up(int i){ while(i>1 && less(heapS[i],heapS[i/2])){ swapS(i,i/2); i/=2; } }
static void down(int i){
    for(;;){
        int l=2*i,r=l+1,b=i;
        if(l<=nS && less(heapS[l],heapS[b])) b=l;
        if(r<=nS && less(heapS[r],heapS[b])) b=r;
        if(b==i) break; swapS(i,b); i=b;
    }
}

void initServerManager(int count){
    nS = count;
    for(int i=1;i<=nS;i++){
        heapS[i].serverId = i-1; // ids 0..count-1
        heapS[i].load = 0;
        posById[heapS[i].serverId] = i;
    }
    for(int i=nS/2;i>=1;i--) down(i);
}

Server peekLeastLoaded(){ return heapS[1]; }
int    getLeastLoadedId(){ return heapS[1].serverId; }
void   updateServerLoad(int serverId, int deltaLoad){
    int i = posById[serverId];
    heapS[i].load += deltaLoad;
    up(i); down(i); // one of them will fix; both is fine & simple
}
int serverCount(){ return nS; }
