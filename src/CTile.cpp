
#include "CTile.h"
#include "CPortal.h"
#include "CPlayer.h"

CTile::CTile(string _type, int _x, int _y) : CObject(_x, _y), type(_type)
{
	Initialize();
}

CTile::~CTile()
{
	Delete();
}

void CTile::Initialize()
{
	listtil.push_back(this);
	w = h = 32;
	TileSettings();
}

void CTile::Delete()
{
	RemoveElement<CTile*>(listtil, this);
}

void CTile::TileSettings()
{
	setImage("tile", type);
	setDimensions(_img->w, _img->h);
	if(type == "spike")
		setLoc(getX(), getY() + TILE_HEIGHT/2 - getH()/2);
	else if(type == "nspike")
		setLoc(getX(), getY() - TILE_HEIGHT/2 + getH()/2);

}	

void CTile::SetAttribute(string var, string val)
{
	CObject::SetAttribute(var, val);
}

void CTile::LifeCycle()
{
	CObject::LifeCycle();
	
	if(type == "spike" && counter % 10 == 0)
	{
		vector<CPlayer*>::iterator a;
		for(a=listply.begin(); a!=listply.end(); ++a)
			if(collision::box(getX(), getY() - 1, getW(), getH(), (*a)->getX(), (*a)->getY(), (*a)->getW(), (*a)->getH()))
			{
				PlaySFX("spike");
				(*a)->setHealth((*a)->getHealth() - 2);
			}
	}
}

bool CTile::isBase()
{
	return type == "wall";
}

bool CTile::isSupport()
{
	return true;
}

bool CTile::canPassThrough()
{
	return type == "glass" || type == "bglass";
}

void CTile::Bumped(CDynamic* dyn)
{
	CObject::Bumped(dyn);
}

bool CTile::canShatter()
{
	return type == "bglass";
}

bool CTile::canBreak()
{
	return canShatter() || type == "bwall";
}

void CTile::Shatter()
{
	if(canShatter())
	{
		PlaySFX("shatter");
		DeleteObj(this);
	}
}

void CTile::Break()
{
	PlaySFX("break");
	DeleteObj(this);
}

bool CTile::inCollision(CObject* obj, double xloc, double yloc)
{
	if(!isDense() || !obj->isDense() || obj == this || portal::inArea(this, getX(), getY()))
		return false;

	if(collision::box(xloc, yloc,  getW(), getH(), obj->getX(), obj->getY(), obj->getW(), obj->getH())) 
		return true;

	return false;
}

bool CTile::colException(CDynamic* dyn, double objXloc, double objYloc)
{
	if(portal::inAreaOfEffect((CObject*) dyn, objXloc, objYloc) && portal::inAreaOfEffect(this, getX(), getY()))
		return true;

	return false;
}

bool CTile::canPlacePortal(int dir)
{
	/*
		Conditions:
			- Tile must have adjacent valid blocks 
			- Adjacent blocks must not have adjacent blocks blocking
			  portal entrance
	*/

	CTile* left   = getTileStep(this, dirTurn(dir, 90), 1);
	CTile* right  = getTileStep(this, dirTurn(dir, -90), 1);
	CTile* center = getTileStep(this, dirTurn(dir, 180), 1);

	if(tile::isPortalBase(left) && tile::isPortalBase(right) && center == NULL)
	{
		CTile* leftLedge  = getTileStep(left,  dirTurn(dir, 180), 1);
		CTile* rightLedge = getTileStep(right, dirTurn(dir, 180), 1);

		if(leftLedge == NULL && rightLedge == NULL)
			return true;
	}

	return false;
}

// INDIV

namespace tile
{
bool isPortalBase(CTile* t)
{
	return t != NULL && t->isBase();
}
bool isPortalSupport(CTile* t)
{
	return t != NULL && t->isSupport();
}
}
