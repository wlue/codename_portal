
#include "CDoor.h"

CDoor::CDoor(int _x, int _y) : CTile("door", _x, _y)
{
	Initialize();
}

CDoor::~CDoor()
{
	Delete();
}

void CDoor::Initialize()
{
	setDimensions(64, 96);
	setLoc(getX(), getY() - TILE_HEIGHT);
	density = false;
	exit = false;
}

void CDoor::Delete()
{

}

void CDoor::SetAttribute(string var, string val)
{
	CTile::SetAttribute(var, val);
	if(var == "exit")
		exit = str2val<int>(val) == 1;
	if(exit)
		setImageState("doorexit");
}