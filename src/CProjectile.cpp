
#include "CProjectile.h"
#include "CUnit.h"
#include "CPlayer.h"
#include "CTile.h"

CProjectile::CProjectile(CUnit* _owner) : CDynamic(), owner(_owner)
{
	Initialize();
	SetLocation();
}

CProjectile::~CProjectile()
{
	Delete();
}

void CProjectile::Initialize()
{
	listprj.push_back(this);
	density = true;
	setMass(0);
	setAngle((int)owner->getDir());
	setImage("projectile", "bullet");
	setDimensions(2, 2);
}

void CProjectile::Delete()
{
	RemoveElement(listprj, this);
}

void CProjectile::SetLocation()
{
	// Set the starting location for the projectile dependant on 
	// what direction the unit firing it is facing, and where the 
	// unit is in the map
	x = owner->getX(); y = owner->getY();
	getStep(x, y, owner->getAnchOffset(), owner->getAnchOffsetDir());
	getStep(x, y, 20, getAngle());
	getStepDist(xVel, yVel, 30, getAngle());
}

void CProjectile::Bump(CObject* obj)
{
	// Stop bullet, call impact function, and delete object when a 
	// projectile bumps into something
	setVel(0, 0);
	if(obj != NULL) Impact(obj);
	DeleteObj(this);
}

void CProjectile::Impact(CObject* obj)
{
	// If tile is shatterable, shatter it
	if(obj == NULL) return;
	CTile* tile = dynamic_cast<CTile*>(obj);
	if(tile != NULL) tile->Shatter();

	// If a player is hit, decrease health and play sound effect
	CPlayer* ply = dynamic_cast<CPlayer*>(obj);
	if(ply != NULL) {
		PlaySFX("hit");
		ply->setHealth(ply->getHealth() - TURRET_DMG);
	}
}

bool CProjectile::inCollision(CObject* obj, double xloc, double yloc)
{
	// Don't allow projectiles to collide into themselves
	CProjectile* proj = dynamic_cast<CProjectile*> (obj);
	if(proj != NULL) return false;

	// Don't allow projectiles to collide into undense objects, and their owner
	if(!obj->isDense() || obj == this || obj == owner)
		return false;

	if(collision::box(xloc, yloc, 0, 0, obj->getX(), obj->getY(), obj->getW(), obj->getH())) 
		return true;

	return false;
}

void CProjectile::UpdateImage()
{
	_img = img->getImage(getImageState(), 0, false);
}

void CProjectile::UpdateMirrorImage(int newVisAngle)
{
	_imgMirror = img->getImage(getImageState(), 0, false);
}