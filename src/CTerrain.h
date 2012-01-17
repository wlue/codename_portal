
#ifndef _CTERRAIN_H
#define _CTERRAIN_H

#include "main.h"
#include "CMovable.h"

// A dynamic object on the screen that has its own movement (ie. box, spikeball)
class CTerrain: public CMovable
{
protected:
	string type;

private:
	void Initialize();
	void Delete();

public:
	CTerrain(string _type, int _x, int _y);
	virtual ~CTerrain();
	
	void Bump(CObject* obj);

};

#endif