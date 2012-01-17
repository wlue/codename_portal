
#include "CUnit.h"

CUnit::CUnit() : CMovable()
{
	Initialize();
}

CUnit::CUnit(int _x, int _y) : CMovable(_x, _y)
{
	Initialize();
}

CUnit::~CUnit()
{
	Delete();
}

void CUnit::Initialize()
{
	_imgAnchor = NULL;
	image_state = "base";
	anchor_state = "anchor";
	anchOffset = 0;
	anchOffsetDir = 0;
}

void CUnit::Delete()
{
	RemoveElement<CUnit*>(listunt, this);
}

void CUnit::SetAttribute(string var, string val)
{
	CMovable::SetAttribute(var, val);
	if(var == "anch")	setAnchorState(val);
}

void CUnit::setAnchorState(string state)
{
	if(anchor_state != state)
		anchor_state = state;
	UpdateImage();
}

// Drawing

void CUnit::UpdateImage()
{
	_img		= img->getImage(getImageState(), angle, true);
	_imgAnchor	= img->getImage(getAnchorState(), dir, true);
}

void CUnit::UpdateMirrorImage(int newVisAngle, int newAnchAngle)
{
	_imgMirror	= img->getImage(getImageState(), newVisAngle, true);
	_imgAnchorMirror = img->getImage(getAnchorState(), newAnchAngle, true);
}

void CUnit::drawPortalMirror(Client* client, CPortal* enter)
{
	CPortal* exit = enter->getOther();
	if(exit == NULL) return;

	int angleDiff;				
	int newVisAngle;
	int newAnchAngle;
	double nx = exit->getX();
	double ny = exit->getY();	
	double dx, dy;	

	angleDiff = normAngle(exit->getEnterDir() - enter->getEnterDir());		
	portal::calcStepDist(dx, dy, enter, exit, this, angleDiff);

	nx += dx;
	ny += dy;

	newVisAngle = portal::calcObjAngle(enter, exit, angle, angleDiff);
	newAnchAngle = portal::calcObjAngle(enter, exit, dir, angleDiff);
	UpdateMirrorImage(newVisAngle, newAnchAngle);

	if(_imgMirror != NULL)
		ApplySurface_Screen
		(
			_imgMirror, 
			client->getSX() + (int(nx) - client->getCameraX()) - (_imgMirror->w/2), 
			client->getSY() + (int(ny) - client->getCameraY()) - (_imgMirror->h/2),
			client->getSX(), client->getSY(),
			client->getSW(), client->getSH()
		);

	if(_imgAnchorMirror != NULL)
	{
		double xmod, ymod;
		getStepDist(xmod, ymod, anchOffset, newVisAngle + anchOffsetDir);
		ApplySurface_Screen
		(
			_imgAnchorMirror, 
			client->getSX() + (int(nx + xmod) - client->getCameraX()) - (_imgAnchorMirror->w/2), 
			client->getSY() + (int(ny + ymod) - client->getCameraY()) - (_imgAnchorMirror->h/2),
			client->getSX(), client->getSY(),
			client->getSW(), client->getSH()
		);
	}
}

void CUnit::draw(Client* client)
{
	if(!display || !client->inView(this) || !_img || !client->getEye()) 
		return;
	UpdateImage();
	DrawBody(client);
	DrawAnchor(client);
}

void CUnit::DrawBody(Client* client)
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

void CUnit::DrawAnchor(Client* client)
{
	if(!_imgAnchor) 
		return;

	double xmod, ymod;
	getStepDist(xmod, ymod, anchOffset, angle + anchOffsetDir);

	ApplySurface_Screen
	(
		_imgAnchor, 
		client->getSX() + (int(x + xmod) - client->getCameraX()) - (_imgAnchor->w/2), 
		client->getSY() + (int(y + ymod) - client->getCameraY()) - (_imgAnchor->h/2),
		client->getSX(), client->getSY(),
		client->getSW(), client->getSH()
	);
}
