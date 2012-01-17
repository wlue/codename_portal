
#include "CPlayer.h"
#include "CDoor.h"
#include "CProjPortal.h"
#include "CPortal.h"

CPlayer::CPlayer(Client* _owner) : CUnit(), owner(_owner)
{
	Initialize();
}

CPlayer::~CPlayer()
{
	Delete();
}

void CPlayer::Initialize()
{
	listply.push_back(this);
	portal1 = portal2 = NULL;
	setDimensions(40, 40);
	setDir(270);
	setAngle(0);
	setImage("stickman");
	anchOffset = 5;
	anchOffsetDir = 0;
	health = PLAYER_MAXHEALTH;
}

void CPlayer::Delete()
{
	RemoveElement<CPlayer*>(listply, this);
}

void CPlayer::Reset()
{
	// Reset all variables when game is started/reset
	portal1 = portal2 = NULL;
	setDimensions(40, 40);
	setDir(270);
	setAngle(0);
	setVel(0, 0);
	setXAccel(0);
	setImage("stickman");
	setHealth(PLAYER_MAXHEALTH);
}

void CPlayer::UpdateMovement()
{
	CUnit::UpdateMovement();
	if(!onGround())	image_state = "jump";	// Player animation is jumping when they are not on the ground
	else			image_state = "base";
}

void CPlayer::setHealth(int h)
{
	// Reduce health, and check for death
	health = lim(h, 0, PLAYER_MAXHEALTH); 
	owner->DeathCheck();
}

void CPlayer::PlacePortal(CTile* loc, bool type, int projDir)
{
	// Place and set different colours dependant on what type of portal it is
	if(type)
	{
		if(!portal1) portal1 = new CPortal(this, type);
		portal1->Place(loc, projDir);
	}
	else 
	{
		if(!portal2) portal2 = new CPortal(this, type);
		portal2->Place(loc, projDir);
	}
}

bool CPlayer::canShoot(bool t)
{
	// Can the player shoot?  The player can only shoot a portal projectile when an instance of 
	// a projectile of the same colour is found on the map
	vector<CProjPortal*>::iterator p;
	for(p=listppt.begin(); p!=listppt.end(); ++p)
		if((*p)->getOwner() == this && (*p)->getType() == t)
			return false;
	return true;
}

bool CPlayer::inDoor()
{
	// Is the player beside the exit door in a level?
	vector<CTile*>::iterator p;
	for(p=listtil.begin(); p!=listtil.end(); ++p)
	{
		CDoor* door = dynamic_cast<CDoor*> (*p);
		if(door != NULL && collision::box(getX(), getY(),  getW(), getH(), door->getX(), door->getY(), door->getW(), door->getH()) && door->isExit())
			return true;
	}
	return false;
}	

void CPlayer::Shoot(int type)
{
	bool stype;
	if(type == SDL_BUTTON_LEFT)			stype = true;	// Shoot red portal
	else if(type == SDL_BUTTON_RIGHT)	stype = false;	// Shoot blue portal
	else								return;

	if(canShoot(stype))	// Shoot a new portal if conditions meet
	{
		PlaySFX("portal_shot");
		new CProjPortal(this, stype);
	}
}

void CPlayer::Jump()
{
	// Player may only jump when he/she is on the ground
	if(onGround())
		yVel = -JUMP_FORCE;	// excert jump force by setting yVel a value up
}

void CPlayer::Bump(CObject* obj)
{
	double old_yvel = getYVel();
	CUnit::Bump(obj);
	double change = (abs(old_yvel - getYVel()) - JUMP_FORCE * 1.3);
	if(change > 0)
	{
		PlaySFX("fall");
		setHealth(health - int(change));
	}
}

void CPlayer::UpdateImage()
{
	_img		= img->getImage(getImageState() + angle2dirstate(dir), angle, true);
	_imgAnchor	= img->getImage(getAnchorState() + angle2dirstate(dir), dir, true);
}

void CPlayer::UpdateMirrorImage(int newVisAngle, int newAnchAngle)
{
	_imgMirror	= img->getImage(getImageState() + angle2dirstate(dir), newVisAngle, true);
	_imgAnchorMirror = img->getImage(getAnchorState() + angle2dirstate(dir), newAnchAngle, true);
}