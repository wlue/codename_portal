
#include "GLevel.h"
#include "GScenario.h"

GLevel::GLevel(GScenario* _scene) : scenario(_scene)
{

}

void GLevel::Configure(string _name, string _filename, int _diff, int _time)
{
	name = _name;
	filename = _filename;
	difficulty = _diff;
	time_limit = _time;
	scenario->levels.push_back(this);
}

string GLevel::getFileDir()
{
	return scenario->getFileDir() + filename;
}	