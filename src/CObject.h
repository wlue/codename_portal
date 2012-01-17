
#ifndef _COBJECT_H
#define _COBJECT_H

#include "main.h"

#include "Client.h"
#include "CVisual.h"

// An object on the map
class CObject: public CVisual
{
protected:
	int tx, ty;		// tile location
	bool density;
	bool reqCollisionPrec;

private:
	void Initialize();
	virtual void Delete();

public:
	CObject();
	CObject(double _x, double _y);
	CObject(int _x, int _y);
	virtual ~CObject();

	int getTX() { return tx; }
	int getTY() { return ty; }
	bool isDense() { return density; }

	virtual void Bumped(CDynamic* dyn);
	virtual void setLoc();
	virtual void setLoc(double _x, double _y);
	void setTileLoc();
	void setTileLoc(int _tx, int _ty);
	virtual void SetAttribute(string var, string val);
	
	virtual bool colException(CDynamic* dyn, double objXloc, double objYloc);
	virtual bool inCollision(CObject* obj, double xloc, double yloc);
	virtual bool collisionPrecision(CObject* obj, double xloc, double yloc);

	virtual void draw(Client* client);

};

#endif