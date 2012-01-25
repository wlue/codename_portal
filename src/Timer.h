
#ifndef _CTIMER_H
#define _CTIMER_H

#include "SDL.h"

// Timer used to keep track of time to the millisecond
class Timer
{
	private:
		int startTicks;
		int pausedTicks;
		bool paused;
		bool started;

	public:
		Timer();

		void Start();
		void Stop();
		void Pause();
		void Unpause();

		int getTicks();

		bool isStarted();
		bool isPaused();    
};

#endif
