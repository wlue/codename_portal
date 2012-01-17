
#ifndef _CUNIT_H
#define _CUNIT_H

#include "main.h"
#include "CDynamic.h"
#include "CMovable.h"
#include "CPortal.h"

// An object that handles its own intelligence, whether or not it is intelligent :)
class CUnit: public CMovable
{
protected:
	string anchor_state;
	SDL_Surface* _imgAnchor;
	SDL_Surface* _imgAnchorMirror;
	int anchOffset;				// anchor offset (in pixels)
	int anchOffsetDir;			// direction (in degrees)

private:
	void Initialize();
	void Delete();

public:
	CUnit();
	CUnit(int _x, int _y);
	virtual ~CUnit();

	SDL_Surface* getAnchor() { return _imgAnchor; }
	string getAnchorState() { return anchor_state; }
	int getAnchOffset() { return anchOffset; }
	int getAnchOffsetDir() { return anchOffsetDir; }

	void setAnchorState(string state);
	void SetAttribute(string var, string val);

	virtual void Shoot(int type) =0;
	virtual void DrawAnchor(Client* client);
	virtual void DrawBody(Client* client);
	void UpdateImage();
	void draw(Client* client);

	virtual void UpdateMirrorImage(int newVisAngle, int newAnchAngle);
	void drawPortalMirror(Client* client, CPortal* enter);
};

#endif