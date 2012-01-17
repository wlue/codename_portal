
#include "GScenario.h"
#include "GLevel.h"

GScenario::GScenario()
{

}

GScenario::~GScenario()
{
	RemoveElement(listscn, this);
}

void GScenario::Configure(string _name, string _filefolder, string _diff)
{
	name = _name;
	filedir = _filefolder;
	difficulty = _diff;
	listscn.push_back(this);
}