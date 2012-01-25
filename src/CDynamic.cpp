
#include "CDynamic.h"
#include "CPortal.h"
#include "CProjPortal.h"

CDynamic::CDynamic() : CObject()
{
	Initialize();
}

CDynamic::CDynamic(double _x, double _y) : CObject(_x, _y)
{
	Initialize();
}

CDynamic::CDynamic(int _x, int _y) : CObject(_x, _y)
{
	Initialize();
}

CDynamic::~CDynamic()
{
	Delete();
}

void CDynamic::Initialize()
{
	listdyn.push_back(this);
	_imgMirror = NULL;
	setAngle(0);
	setNormAngle(0);
	setMass(1);
	setVel(0, 0);
	setXAccel(0);
	setDimensions(0, 0);
}

void CDynamic::Delete()
{
	RemoveElement<CDynamic*>(listdyn, this);
}

void CDynamic::setAngle(int d)
{
	angle = normAngle(d);
}

void CDynamic::setLoc(double _x, double _y)
{
	CObject::setLoc(_x, _y);
	CPortal* warpArea = portal::inWarpArea(this, getX(), getY());
	if(warpArea != NULL)
		WarpPortal(warpArea);
}

void CDynamic::SetAttribute(string var, string val)
{
	CObject::SetAttribute(var, val);
	if(var == "mass")	mass = str2val<double>(val);
	if(var == "angle")	setAngle(str2val<int>(val));
}

bool CDynamic::onGround()
{
	return onSurface(0, 2);
}

bool CDynamic::onSurface(int xmod, int ymod)
{
	double xloc = x + xmod, yloc = y + ymod;
	double oX = getX(), oY = getY();
	bool onSurface = false;
	setLoc(xloc, yloc);

	if(getX()+getW()/2 > MAP_WIDTH || getX()-getW()/2 < 0 || getY()+getH()/2 > MAP_HEIGHT || getY()-getH()/2 < 0)
		onSurface = true;

	vector<CObject*>::iterator i;
	for(i=listobjs.begin(); i!=listobjs.end() && !onSurface; ++i)
		if(inCollision(*i, getX(), getY())) 
			onSurface = true;

	x = oX;	y = oY;
	return onSurface;
}

bool CDynamic::Colliding(double xloc, double yloc)
{
	if(xloc+w/2 > MAP_WIDTH || xloc-w/2 < 0 || yloc+h/2 > MAP_HEIGHT || yloc-h/2 < 0)
	{
		qBump(NULL);	
		return true;
	}

	vector<CObject*>::iterator i;
	for(i=listobjs.begin(); i!=listobjs.end(); ++i)
	{
		if(inCollision(*i, xloc, yloc)) 
		{
			qBump(*i);
			return true;
		}
	}
	return false;
}

void CDynamic::HandleAnglePos()
{
	if(nangle < 0) return;
	int step = dirstep(angle, nangle)*10;
	setAngle(angle + step);
	UpdateImage();
}

void CDynamic::UpdateMovement()
{
	HandleAnglePos();	// Update direction body is facing (nangle)
	HandleVelocity();	// Update velocities (ie. gravity, movement)
	
	if(xVel != 0 || yVel != 0)	// Don't do any calculations if not necessary
	{
		int mul = 2;	// Precision: higher number = faster & less precise
		double oX, oY, vX, vY;
		int oA;
		double xStep = mul*sign(xVel), yStep = mul*sign(yVel);	// direction and magnitude of stepping
		double a=0, b=0;	// counter variables

		while(a < abs(round(xVel)) || b < abs(round(yVel)))
		{
			oA = getAngle();
			oX = getX();	oY = getY();
			vX = getXVel(); vY = getYVel();

			// Move X
			if(xVel != 0 && a<abs(round(xVel)))
			{
				setLoc(getX() + xStep, getY());
				if(Colliding(getX(), getY()))
				{
					x = oX;		y = oY;		// Set variables back when in orig position
					setVel(vX, vY);
					angle = oA;
					a = abs(round(xVel));	// end X iteration
				}
				xStep = mul*sign(getXVel());
				a += mul;
			}

			// Move Y
			if(yVel != 0 && b<abs(round(yVel)))
			{
				setLoc(getX(), getY() + yStep);
				if(Colliding(getX(), getY()))
				{
					x = oX;		y = oY;
					setVel(vX, vY);
					angle = oA;
					b = abs(round(yVel));	// end Y iteration
				}
				yStep = mul*sign(getYVel());	// update direction based on velocity
				b += mul;
			}
		}
	}
	HandleBumping();
}

void CDynamic::HandleBumping()
{
	if(!queue_bump.size()) return;
	vector<CObject*>::iterator a;
	for(a=queue_bump.begin(); a!=queue_bump.end(); ++a)
		Bump(*a);
	queue_bump.clear();
}

void CDynamic::Bump(CObject* obj)
{
	if(obj == NULL) return;
	obj->Bumped(this);
	// int a = getangle(getX(), getY(), obj->getX(), obj->getY());
	if(onGround() || onSurface(0, -2))
		yVel = 0;
	if(onSurface(-1, 0) || onSurface(1, 0))
		xVel = 0;
}

void CDynamic::qBump(CObject* obj)
{
	queue_bump.push_back(obj);
}

bool CDynamic::inCollision(CObject* obj, double xloc, double yloc)
{
	CProjPortal* proj = dynamic_cast<CProjPortal*>(obj);
	if(proj != NULL) return false;

	if(obj->colException(this, xloc, yloc))
		return false;
	return CObject::inCollision(obj, xloc, yloc);
}

void CDynamic::HandleVelocity()
{
	double limit = false;
	double dV = xAccel;
	if(!onGround()) 
	{
		double magnitude = GRAVITY * (FPS / 1000.0);
		if(!getMass()) magnitude = 0; 
		yVel = lim(yVel + magnitude, -TERMINAL_VEL, TERMINAL_VEL);
		dV *= MOVABLE_AIR_DEACCEL;
		if((xVel > 0 && dV > 0) || (xVel < 0 && dV < 0))
			dV = 0;
	}
	else
	{
		if(dV == 0 && xVel != 0)
		{
			double diff = -sign(xVel) * PLAYER_ACCEL;
			double newVel = xVel + diff;
			if((xVel > 0 && newVel < 0) || (xVel < 0 && newVel > 0))
				dV = -xVel;
			else
				dV = diff;
		}
		else
			limit = true;

	}

	xVel = lim(dV + xVel, -TERMINAL_VEL, TERMINAL_VEL);
	if(limit) xVel = lim(xVel, -PLAYER_MAXSPEED, PLAYER_MAXSPEED);
}

void CDynamic::WarpPortal(CPortal* enter)
{
	CPortal* exit = enter->getOther();
	if(exit == NULL) return;

	int angleDiff;				
	int newVisAngle;	
	double dx, dy;
	double vx = getXVel(), vy = getYVel();

	angleDiff = normAngle(exit->getEnterDir() - enter->getEnterDir());		
	newVisAngle = portal::calcObjAngle(enter, exit, getAngle(), angleDiff);
	portal::calcStepDist(dx, dy, enter, exit, this, angleDiff);
	portal::calcStepVel (vx, vy, enter, exit, this, angleDiff);

	CObject::setLoc(exit->getX() + dx, exit->getY() + dy);
	setAngle(newVisAngle);
	setVel(vx, vy);
}


void CDynamic::LifeCycle()
{
	UpdateMovement();
}

// drawing

void CDynamic::UpdateImage()
{
	_img = img->getImage(getImageState(), angle, true);
}

void CDynamic::UpdateMirrorImage(int newVisAngle)
{
	_imgMirror = img->getImage(getImageState(), newVisAngle, true);
}

void CDynamic::drawPortalMirror(Client* client, CPortal* enter)
{
	// 'portal' is the portal 'this' is approaching
	// Preconditions: Dynamic object is in collision with the portal ledge
	//                Portal has an exit portal
	CPortal* exit = enter->getOther();
	if(exit == NULL) return;

	int angleDiff;				// angle from enter to exit enterDir, the direction of movement the object must turn to
	int newVisAngle;			// the object entering the portal's mirror angle image
	double nx = exit->getX();	// location of object's mirror image
	double ny = exit->getY();	
	double dx, dy;				// vector distance from exit portal to object.  determined by dist and newObjAngle

	angleDiff = normAngle(exit->getEnterDir() - enter->getEnterDir());		
	portal::calcStepDist(dx, dy, enter, exit, this, angleDiff);

	nx += dx;
	ny += dy;

	newVisAngle = portal::calcObjAngle(enter, exit, angle, angleDiff);
	UpdateMirrorImage(newVisAngle);

	if(_imgMirror == NULL) return;
	ApplySurface_Screen
	(
		_imgMirror, 
		client->getSX() + (int(nx) - client->getCameraX()) - (_imgMirror->w/2), 
		client->getSY() + (int(ny) - client->getCameraY()) - (_imgMirror->h/2),
		client->getSX(), client->getSY(),
		client->getSW(), client->getSH()
	);

}

/*
void CDynamic::drawPortalMirror(Client* client, CPortal* enter)
{
	// 'portal' is the portal 'this' is approaching
	// Precondition: Dynamic object is in collision with the portal ledge
	//               Portal has an exit portal
	CPortal* exit = enter->getOther();
	if(exit == NULL) return;

	int angleDiff;		// angle from enter to exit enterDir, the direction of movement the object must turn to
	int newPorAngle;	// the angle of the exit portal
	int newVisAngle;	// the object entering the portal's mirror angle image
	int objAngle;		// angle from obj's center to entrance portal's center
	int newObjAngle;	// angle from mirror obj's center to exit portal's center
	double nx, ny;		// location of object's mirror image
	double dist;		// scalar distance.  used to rotate to find relative location
	double dx, dy;		// vector distance from exit portal to object.  determined by dist and newObjAngle

	angleDiff = normAngle(exit->getEnterDir() - enter->getEnterDir());		
	newPorAngle = exit->getEnterDir();
	objAngle = getangle(enter->getX(), enter->getY(), getX(), getY());
	newObjAngle = normAngle(objAngle + angleDiff);

	nx = exit->getX();
	ny = exit->getY();

	dist = distance(enter->getX(), enter->getY(), getX(), getY());
	getStepDist(dx, dy, dist, newObjAngle);

	if(newPorAngle % 180 == 0)	dy = -dy;
	else						dx = -dx;

	newVisAngle = mirrorAngle(angle + angleDiff, angleDiff);
	if(enter->getEnterDir() % 180 == 0)
		newVisAngle = normAngle(newVisAngle + 180);

	if(enter->isInversed() != exit->isInversed()) // base inversed: mirror directions and dx/dy
	{
		newVisAngle = mirrorAngle(newVisAngle, newPorAngle);
		if(newPorAngle % 180 == 0)	dx = -dx;
		else						dy = -dy;
	}

	nx += dx;
	ny += dy;

	_imgMirror = img->getImage(getImageState(), newVisAngle, true);
	if(_imgMirror == NULL) return;
	ApplySurface_Screen
	(
		_imgMirror, 
		client->getSX() + (int(nx) - client->getCameraX()) - (_imgMirror->w/2), 
		client->getSY() + (int(ny) - client->getCameraY()) - (_imgMirror->h/2),
		client->getSX(), client->getSY(),
		client->getSW(), client->getSH()
	);

}

OLD MOVEMENT CODE: Does not take account for portals mid iter-step
void CDynamic::UpdateMovement()
{
	HandleAnglePos();
	HandleVelocity();
	double xStep = sign(xVel), yStep = sign(yVel);
	double tmpX = x, tmpY = y;

	for(double a=0; inRangeOf(a, xVel) && !Colliding(tmpX + xStep, tmpY); a += xStep)
		tmpX += xStep;

	for(double b=0; inRangeOf(b, yVel) && !Colliding(tmpX, tmpY + yStep); b += yStep)
		tmpY += yStep;

	setLoc(tmpX, tmpY);
}

// NON-WORKING MOVEMENT CODE: vector direction in 1 var

void CDynamic::UpdateMovement()
{
	HandleAnglePos();
	HandleVelocity();

	double prec = abs(max(xVel, yVel));
	double step = prec;

	while(step > 0)
	{
		double nx = getX() + xVel/prec;
		double ny = getY() + yVel/prec;
		if(!Colliding(nx, ny))
			setLoc(nx, ny);
		step--;
	}
	cout << prec << endl;
}

*/


/*

void CDynamic::UpdateMovement()
{
	HandleAnglePos();
	HandleVelocity();
	
	if(xVel != 0 || yVel != 0)
	{
		int mul = 2;
		double oX, oY, vX, vY;
		int oA;
		double xStep = mul*sign(xVel), yStep = mul*sign(yVel);

		for(double a=0; a<abs(round(xVel)); a += mul)
		{
			oA = getAngle();
			oX = getX();	oY = getY();
			vX = getXVel(); vY = getYVel();
			setLoc(getX() + xStep, getY());
			if(Colliding(getX(), getY()))
			{
				x = oX;		y = oY;
				xVel = vX;  yVel = vY;
				angle = oA;
				break;
			}
			xStep = mul*sign(getXVel());
		}

		for(double b=0; b<abs(round(yVel)); b += mul)
		{
			oA = getAngle();
			oX = getX();	oY = getY();
			vX = getXVel(); vY = getYVel();
			setLoc(getX(), getY() + yStep);
			if(Colliding(getX(), getY()))
			{
				x = oX;		y = oY;
				xVel = vX;  yVel = vY;
				angle = oA;
				break;
			}
			yStep = mul*sign(getYVel());
		}
	}
	HandleBumping();
}

void CDynamic::UpdateMovement()
{
	HandleAnglePos();
	HandleVelocity();
	
	int mul = 1;
	double oX, oY;
	double xStep = mul*sign(xVel), yStep = mul*sign(yVel);

	for(double a=0; a<abs(round(xVel)); a += mul)
	{
		oX = getX();	oY = getY();
		setLoc(getX() + xStep, getY());
		if(Colliding(getX(), getY()))
		{
			x = oX;		y = oY;
			break;
		}
		xStep = mul*sign(xVel);
	}

	for(double b=0; b<abs(round(yVel)); b += mul)
	{
		oX = getX();	oY = getY();
		setLoc(getX(), getY() + yStep);
		if(Colliding(getX(), getY()))
		{
			x = oX;		y = oY;
			break;
		}
		yStep = mul*sign(yVel);
	}
}

void CDynamic::UpdateMovement()
{
	HandleAnglePos();
	HandleVelocity();
	int mul = 2;
	double xStep = mul*sign(xVel), yStep = mul*sign(yVel);
	for(double a=0; a<abs(xVel) && !Colliding(getX() + xStep, getY()); a += mul)
	{
		setLoc(getX() + xStep, getY());
		xStep = mul*sign(xVel);
	}

	for(double b=0; b<abs(yVel) && !Colliding(getX(), getY() + yStep); b += mul)
	{
		setLoc(getX(), getY() + yStep);
		yStep = mul*sign(yVel);
	}
}
*/
