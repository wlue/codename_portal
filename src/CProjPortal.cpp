
#include "CProjPortal.h"
#include "CPlayer.h"

CProjPortal::CProjPortal(CPlayer* _owner, bool _type) : CProjectile(_owner), type(_type)
{
	Initialize();
}

CProjPortal::~CProjPortal()
{
	Delete();
}

void CProjPortal::Initialize()
{
	listppt.push_back(this);
	// Set the image dependant on what type of portal it is
	setImage("projectile", type ? "portal_red" : "portal_blue");
}

void CProjPortal::Delete()
{
	RemoveElement(listppt, this);
}

void CProjPortal::Bump(CObject* obj)
{
	CProjectile::Bump(obj);
}

void CProjPortal::Impact(CObject* obj)
{
	if(obj == NULL) return;
	CTile* tile = dynamic_cast<CTile*> (obj);
	if(tile != NULL)
	{
		// Place the portal onto the bumped tile if possibile
		CPlayer* player = dynamic_cast<CPlayer*> (owner);
		if(owner == NULL) return;
		player->PlacePortal(tile, getType(), getangle(getX(), getY(), tile->getX(), tile->getY()));
	}
}

bool CProjPortal::inCollision(CObject* obj, double xloc, double yloc)
{
	// Do not allow the portal projectile to collide with any dynamic objects
	CDynamic* dyn = dynamic_cast<CDynamic*>(obj);
	if(dyn != NULL) return false;

	// Don't allow the portal projectile to collide with see-through tiles
	CTile* tile = dynamic_cast<CTile*> (obj);
	if(((tile != NULL && tile->canPassThrough())))
		return false;

	// Return parent projectile implementation
	return CProjectile::inCollision(obj, xloc, yloc);
}
