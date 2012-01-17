
#ifndef _CTILE_H
#define _CTILE_H

#include "main.h"
#include "CObject.h"

namespace tile
{
bool isPortalBase(CTile* t);
bool isPortalSupport(CTile* t);
}

class CTile: public CObject
{
protected:
	string type;

private:
	void Initialize();
	void Delete();
	void TileSettings();

public:
	CTile(string _type, int _x, int _y);
	virtual ~CTile();
	string getType() { return type; }
	void SetAttribute(string var, string val);
	void Bumped(CDynamic* dyn);

	bool colException(CDynamic* dun, double objXloc, double objYloc);
	bool inCollision(CObject* obj, double xloc, double yloc);
	bool canPlacePortal(int dir);
	bool isBase();
	bool isSupport();
	bool canShatter();
	bool canBreak();
	bool canPassThrough();
	
	void LifeCycle();
	void Shatter();
	void Break();

};

#endif