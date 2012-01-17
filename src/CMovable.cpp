
#include "CMovable.h"
#include "CProjectile.h"

CMovable::CMovable() : CDynamic()
{
	Initialize();
}

CMovable::CMovable(double _x, double _y) : CDynamic(_x, _y)
{
	Initialize();
}

CMovable::CMovable(int _x, int _y) : CDynamic(_x, _y)
{
	Initialize();
}

CMovable::~CMovable()
{
	Delete();
}

void CMovable::Initialize()
{
	listmov.push_back(this);
	setDir(0);
}

void CMovable::Delete()
{
	RemoveElement<CMovable*>(listmov, this);	
}

void CMovable::Bump(CObject* obj)
{
	CDynamic* dyn = dynamic_cast <CDynamic*> (obj);
	if(dyn != NULL)
		dyn->setVel(getXVel()*1.4, getYVel()*0.75);
	CDynamic::Bump(obj);
}

void CMovable::SetAttribute(string var, string val)
{
	CDynamic::SetAttribute(var, val);
	if(var == "dir")	setDir(str2val<int>(val));
}

void CMovable::HandleVelocity()
{
	CDynamic::HandleVelocity();
	UpdateImage();
}
