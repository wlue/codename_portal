
#include "CTurret.h"
#include "CPlayer.h"
#include "CProjectile.h"

CTurret::CTurret(int _x, int _y) : CUnit(_x, _y)
{
	Initialize();
	setLoc(getX(), getY() + TILE_HEIGHT/2 - getH()/2);
}	

CTurret::~CTurret()
{
	Delete();
}

void CTurret::Initialize()
{
	listtrt.push_back(this);
	target = NULL;
	setImage("turret");
	setDimensions(40, 40);
	nangle = -1;
}

void CTurret::Delete()
{
	RemoveElement(listtrt, this);
}

void CTurret::LifeCycle()
{
	CUnit::LifeCycle();
	if(angle != 0) return;
	if(target != NULL)
	{
		int direction = getangle(getX(), getY(), target->getX(), target->getY());
		if(counter % 10 == 0 && floorVal(dir, 10) == floorVal(direction, 10))
			Shoot(0);
		else
		{
			int dirstep = dirstepDist(getDir(), direction);
			setDir(getDir() + dirstep);
		}
		if(distance(getX(), getY(), target->getX(), target->getY()) > 300)
			target = NULL;	
	}
	else
	{
		setDir(getDir() + 10);
		vector<CPlayer*>::iterator a;
		for(a=listply.begin(); a!=listply.end(); ++a)
		{
			if(distance(getX(), getY(), (*a)->getX(), (*a)->getY()) <= 300)
				target = *a;
		}
	}
}

bool CTurret::canShoot()
{
	return onGround() && angle == 0;
}

void CTurret::Shoot(int type)
{
	if(canShoot())
	{
		PlaySFX("gunshot");
		new CProjectile(this);
	}
}