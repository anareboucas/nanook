

#include <stdio.h>
#include <time.h>
#include <malloc.h>

#include <string.h>


#include "timer.h"

static NasaTimer * first;

NasaTimer* GetTimer(char * key){
	NasaTimer * current=first;
	
	while((current !=NULL)&&(!strcmp(current->key,key)))
		current = current->next;
	
	if(current == NULL){
		return new NasaTimer(first,key);
	}
	return current;

}

NasaTimer::NasaTimer(NasaTimer *n, char *k){
	int l = strlen(k)+1;
	key = (char *)malloc(sizeof(char)*l);
	strncpy(key,k,l);
	
	next = n;
	timeRun = 0;
	on = false;
	started = 0;

}

clock_t NasaTimer::Time(){
	if(on){
		
		return timeRun-started+clock();
	}
	
	return timeRun;
}

void NasaTimer::Start(){
	if(!on){
		on = true;
	
		started = clock();
	}
}

void NasaTimer::Stop(){
	if(on){
		on = false;
		timeRun+=clock()-started;
	}
}

void NasaTimer::Display(){
	printf("Timer \"%s\" has run for %d ticks\r\n",key,Time());

}

void DisplayAllTimers(){
	NasaTimer * current=first;
	
	while(current !=NULL){
		current->Display();
		current = current->next;
	}
}

