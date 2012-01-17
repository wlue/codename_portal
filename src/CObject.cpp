
#include "CObject.h"

CObject::CObject() : CVisual()
{
	Initialize();
}

CObject::CObject(double _x, double _y) : CVisual(_x, _y)
{
	Initialize();
	setLoc(_x, _y);
}

CObject::CObject(int _x, int _y) : CVisual(0, 0)
{
	Initialize();
	setTileLoc(_x, _y);
}

CObject::~CObject()
{
	Delete();
}

void CObject::Initialize()
{
	listobjs.push_back(this);
	density = true;
	reqCollisionPrec = false;
}

void CObject::Delete()
{
	RemoveElement<CObject*>(listobjs, this);
}

// ACCESSORS

void CObject::Bumped(CDynamic* dyn)
{

}

void CObject::setLoc()
{
	tx = ty = 0;
	CVisual::setLoc();
}

void CObject::setLoc(double _x, double _y)
{
	CVisual::setLoc(_x, _y);
	tx = xloc2tile(x); ty = yloc2tile(y);
}

void CObject::setTileLoc()
{
	tx = ty = 0;
	x = y = 0;
	display = false;
}

void CObject::setTileLoc(int _tx, int _ty)
{
	tx = _tx;				ty = _ty;
	x = xtile2loc(_tx);		y = ytile2loc(_ty);
	if(tx < 0) setTileLoc();
}

void CObject::SetAttribute(string var, string val)
{
	if(var == "px")			setLoc(getX() + str2val<int>(val), getY());
	if(var == "py")			setLoc(getX(), getY() + str2val<int>(val));
	if(var == "density")	density = str2val<int>(val) == 1;
}

/* MEMBERS */
// collision

bool CObject::inCollision(CObject* obj, double xloc, double yloc)
{
	if(!isDense() || !obj->isDense() || obj == this)
		return false;

	if(collision::box(xloc, yloc,  getW(), getH(), obj->getX(), obj->getY(), obj->getW(), obj->getH())) 
		return reqCollisionPrec ? collisionPrecision(obj, xloc, yloc) : true;

	return false;
}

bool CObject::colException(CDynamic* dyn, double objXloc, double objYloc)
{
	return false;
}

bool CObject::collisionPrecision(CObject* obj, double xloc, double yloc)
{
	return false;
}

// draw

void CObject::draw(Client* client)
{
	if(!display || !client->inView(this) || !_img || !client->getEye()) 
		return;

	ApplySurface_Screen
	(
		_img, 
		client->getSX() + (int(x) - client->getCameraX()) - (_img->w/2), 
		client->getSY() + (int(y) - client->getCameraY()) - (_img->h/2),
		client->getSX(), client->getSY(),
		client->getSW(), client->getSH()
	);

}