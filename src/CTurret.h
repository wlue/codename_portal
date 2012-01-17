
#ifndef _CTURRET_H
#define _CTURRET_H

#include "main.h"
#include "CUnit.h"

class CTurret: public CUnit
{
protected:
	CPlayer* target;

private:
	void Initialize();
	void Delete();

public:
	CTurret(int _x, int _y);
	virtual ~CTurret();

	void SearchTarget();
	void LifeCycle();

	bool canShoot();
	void Shoot(int type);
};

#endif