#include <time.h>

#ifndef TIMER_H
#define TIMER_H

class NasaTimer;

NasaTimer * GetTimer(char * key);
void DisplayAllTimers();


class NasaTimer{

	// gets the timer object associated with a given key string
	// if no timer for that ket string exists, then a new one will be created
	friend NasaTimer* GetTimer(char * key);
	// loops over the list of all timers, and prints how long they have run to
	// standard out
	friend void DisplayAllTimers();
public:

	// start the timer
	void Start();

	// stop the timer
	void Stop();

	// get the time elapsed while the timer was on
	// the time is in clock ticks, not seconds
	// to get seconds, divide by CLOCKS_PER_SEC 
	// this function measures CPU time, not wall time
	// so it should give a reasonably accurate measure of how long stuff takes
	clock_t Time();

	// display the amount of time used on standard out
	void Display();

	

private:

	clock_t timeRun;
	clock_t started;
	bool on;
	char * key;

	NasaTimer * next;

	NasaTimer(NasaTimer * next, char * key);

	
};


#endif