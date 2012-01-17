
#include "CPortal.h"
#include "CPlayer.h"
#include "CProjPortal.h"

CPortal::CPortal(CPlayer* _owner, bool _type) : CObject(), owner(_owner), type(_type)
{
	Initialize();
}

CPortal::~CPortal()
{
	Delete(); 
}

void CPortal::Initialize()
{
	listptl.push_back(this);
	inverseBase = false;
	density = false;
	enterDir = 0;
	loc = NULL;
	_imgBase = _imgLedge = NULL;
	setImage("portal", "black");
	setDimensions(96, 96);
}

void CPortal::Delete()
{
	RemoveElement(listptl, this);
}

CPortal* CPortal::getOther()
{
	CPortal* portal;
	if(!type)	portal = owner->getPortal1();
	else		portal = owner->getPortal2();
	if(!portal || portal->loc == NULL)
		return NULL;
	return portal;
}

bool CPortal::isActive()
{
	return getOther() != NULL && getOther()->loc != NULL;
}

bool CPortal::canPlace(CTile* _loc, int projDir)
{
	int dir = angle2dir(projDir);
	if(_loc->canPlacePortal(dir))
	{
		CPortal* other = getOther();
		if(other != NULL)
		{
			CTile* _loc2 = other->loc;
			int dist = tileDist(_loc->getTX(), _loc->getTY(), _loc2->getTX(), _loc2->getTY());
			if(dist > 2)
				return true;
		}
		else
			return true;
	}
	return false;
}

void CPortal::Place(CTile* _loc, int projDir)
{
	if(!canPlace(_loc, projDir)) 
		return;
	
	PlaySFX("portal_hit");
	int dir = angle2dir(projDir);
	enterDir = dir2angle(dir);
	SetLocation(_loc);
	UpdateState();
	if(getOther() != NULL) getOther()->UpdateState();
}

void CPortal::SetLocation(CTile* _loc)
{
	loc = _loc;
	double _x = 0, _y = 0;
	switch(enterDir)
	{
	case 0:		_y =  loc->getH()/2;	break;
	case 180:	_y = -loc->getH()/2;	break;
	case 90:	_x =  loc->getW()/2;	break;
	case 270:	_x = -loc->getW()/2;	break;
	}
	if(enterDir == 0 || enterDir == 180)	setDimensions(PORTAL_LENGTH, PORTAL_WIDTH);
	else									setDimensions(PORTAL_WIDTH, PORTAL_LENGTH);
	
	inverseBase = enterDir == 90 || enterDir == 180;

	setLoc(loc->getX() + _x, loc->getY() + _y);
}


bool CPortal::inCollision(CObject* obj, double xloc, double yloc)
{
	if(obj == this)
		return false;

	if(collision::box(xloc, yloc,  getW(), getH(), obj->getX(), obj->getY(), obj->getW(), obj->getH())) 
		return true;

	return false;
}

bool CPortal::inPortal(CObject* obj, double objXloc, double objYloc)
{
	double oX = objXloc, oY = objYloc;
	int oW = obj->getW(), oH = obj->getH();
	if(collision::box(getX(), getY(), getW(), getH(), oX, oY, oW, oH)) 
	{	
		if(enterDir % 180 == 0) // if portal is facing down or up, check x
		{
			if(oX >= (getX() - getW()/2 + oW/2) && oX <= (getX() + getW()/2 - oW/2))
				return true;
		}
		else // otherwise check y
		{
			if(oY >= (getY() - getH()/2 + oH/2) && oY <= (getY() + getH()/2 - oH/2))
				return true;		
		}
	}
	return false;
}

bool CPortal::inWarpArea(CDynamic* obj, double objXloc, double objYloc)
{
	double cX = getX(), cY = getY();
	int cW = getW(), cH = getH();
	switch(enterDir)
	{
	case 0: // top section.
		cY -= cH/4;		cH /= 2;
		break;
	case 180:
		cY += cH/4;		cH /= 2;
		break;
	case 90:
		cX -= cW/4;		cW /= 2;
		break;	
	case 270:
		cX += cW/4;		cW /= 2;		
		break;	
	}
	if(collision::box(cX, cY, cW, cH, objXloc, objYloc, 0, 0))
		return true;
	return false;
}

// Drawing

void CPortal::UpdateState()
{
	if(getOther() != NULL)
		image_state = type ? "red" : "blue";
	UpdateImage();
}

void CPortal::UpdateImage()
{
	string iB, iL;
	switch(enterDir)
	{
	case 0:		iB = "-T"; iL = "-B";	break;
	case 180:	iB = "-B"; iL = "-T";	break;
	case 90:	iB = "-L"; iL = "-R";	break;
	case 270:	iB = "-R"; iL = "-L";	break;
	}

	_imgBase  = img->getImage(image_state + iB, 0, false);
	_imgLedge = img->getImage(image_state + iL, 0, false);
}

void CPortal::draw(Client* client)
{
//	drawLedge(client);
//	drawBase(client);
}

void CPortal::drawBase(Client* client)
{
	if(!loc || !display || !client->inView(this) || !_imgBase || !client->getEye()) 
		return;

	if(isActive())
		for(vector<CDynamic*>::iterator a=listdyn.begin(); a!=listdyn.end(); ++a)
			if(inPortal(*a, (*a)->getX(), (*a)->getY()))
				(*a)->drawPortalMirror(client, this);

	ApplySurface_Screen
	(
		_imgBase, 
		client->getSX() + (int(x) - client->getCameraX()) - (_imgBase->w/2), 
		client->getSY() + (int(y) - client->getCameraY()) - (_imgBase->h/2),
		client->getSX(), client->getSY(),
		client->getSW(), client->getSH()
	);
}

void CPortal::drawLedge(Client* client)
{
	if(!loc || !display || !client->inView(this) || !_imgLedge || !client->getEye()) 
		return;

	ApplySurface_Screen
	(
		_imgLedge, 
		client->getSX() + (int(x) - client->getCameraX()) - (_imgLedge->w/2), 
		client->getSY() + (int(y) - client->getCameraY()) - (_imgLedge->h/2),
		client->getSX(), client->getSY(),
		client->getSW(), client->getSH()
	);
}