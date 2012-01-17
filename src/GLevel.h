
#ifndef _GLEVEL_H
#define _GLEVEL_H

#include "main.h"

// A game level where games are played in.  Belongs to a scenario
class GLevel
{
private:
	GScenario* scenario;	// what scenario this is from
	string name;			// name of level
	string filename;		// name of .lvl file
	int difficulty;			// rating from 1 to 10
	int time_limit;			// in seconds

public:
	GLevel(GScenario* _scene);

	GScenario* getScenario() { return scenario; }
	string getName() { return name; }
	int getDifficulty() { return difficulty; }
	int getTimeLimit() { return time_limit; }
	string getFileDir();

	// used to configure 
	void Configure(string _name, string _filename, int _diff, int _time);

};

#endif