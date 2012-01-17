
#ifndef _CPROJPORTAL_H
#define _CPROJPORTAL_H

#include "main.h"
#include "CProjectile.h"
#include "CTile.h"

class CProjPortal: public CProjectile
{
protected:
	bool type;			// true - red, false = blue

private:
	void Initialize();
	void Delete();

public:
	CProjPortal(CPlayer* _owner, bool _type);
	virtual ~CProjPortal();
	bool getType() { return type; }
	void Bump(CObject* obj);
	
	void Impact(CObject* obj);
	bool inCollision(CObject* obj, double xloc, double yloc);

};



#endif