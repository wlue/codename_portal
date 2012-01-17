
#ifndef _GSCNLOADER_H
#define _GSCNLOADER_H

#include "main.h"
#include "GScenario.h"
#include "GLevel.h"

// Internal flags
namespace gscnloader
{
const int 
	READINFO	= 0x01,
	READLEVELS	= 0x02,
	READIGNORE	= 0x08;

const string 
	infoTag		= "==info==",
	levelTag	= "==levels==",
	levelID		= "@level";
}

// Scenario loader: looks at index file to load all scenarios in memory
class GScnLoader
{
private:
	void LoadScenarios();	
	void Load(string folder);

public:
	GScnLoader();



};

#endif