
#ifndef _CMOVABLE_H
#define _CMOVABLE_H

#include "main.h"
#include "CDynamic.h"

// An object that handles its own movement and velocity
class CMovable: public CDynamic
{
protected:
	int dir;

private:
	void Initialize();
	void Delete();

public:
	CMovable();
	CMovable(double _x, double _y);
	CMovable(int _x, int _y);
	virtual ~CMovable();

	int getDir() { return dir; }
	void setDir(int d) { dir = normAngle(d); }
	void Bump(CObject* obj);

	void SetAttribute(string var, string val);
	virtual void HandleVelocity();
};

#endif