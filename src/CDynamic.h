
#ifndef _CDYNAMIC_H
#define _CDYNAMIC_H

#include "main.h"
#include "CObject.h"

// An object that follows the laws of gravity (sometimes)
class CDynamic: public CObject
{
protected:
	SDL_Surface* _imgMirror;
	int angle, nangle;	// normalized angle: degree where the object will be weighted onto
	double xVel, yVel;	// velocity		(vector)
	double xAccel;		// acceleration (vector)
	double mass;		// in kg

	vector<CObject*> queue_bump;	// list of objects waiting to be bumped.  used in HandleMovement()

private:
	void Initialize();
	void Delete();

public:
	CDynamic();
	CDynamic(double _x, double _y);
	CDynamic(int _x, int _y);
	virtual ~CDynamic();

	void SetAttribute(string var, string val);
	bool onGround();
	bool onSurface(int xmod, int ymod);
	bool inCollision(CObject* obj, double xloc, double yloc);

	int getAngle() { return angle; }
	double getXVel() { return xVel; }
	double getYVel() { return yVel; }
	double getXAccel() { return xAccel; }
	double getMass() { return mass; }

	void setLoc(double _x, double _y);

	void setAngle(int d);
	void setNormAngle(int d) { nangle = d; }
	void setVel(double _x, double _y) { xVel = _x; yVel = _y; }
	void setXAccel(double a) { xAccel = a; }
	void setMass(double m) { mass = m; }

	void HandleAnglePos();
	virtual void HandleVelocity();
	virtual void UpdateMovement();
	virtual void HandleBumping();
	virtual bool Colliding(double xloc, double yloc);
	virtual void WarpPortal(CPortal* enter);

	virtual void Bump(CObject* obj);
	void qBump(CObject* obj);			// queue bumping

	void LifeCycle();

	void UpdateImage();
	virtual void UpdateMirrorImage(int newVisAngle);
	virtual void drawPortalMirror(Client* client, CPortal* enter);
};

#endif