
#ifndef _CPROJECTILE_H
#define _CPROJECTLIE_H

#include "main.h"
#include "CDynamic.h"

class CProjectile: public CDynamic
{
protected:
	CUnit* owner;

private:
	void Initialize();
	void Delete();

public:
	CProjectile(CUnit* _owner);
	virtual ~CProjectile();

	CUnit* getOwner() { return owner; }
	void SetLocation();
	void Bump(CObject* obj);
	virtual void Impact(CObject* obj);
	bool inCollision(CObject* obj, double xloc, double yloc);

	void UpdateImage();
	void UpdateMirrorImage(int newVisAngle);
};

#endif