
#ifndef _CPORTAL_H
#define _CPORTAL_H

#include "main.h"
#include "CObject.h"

/*
	Generic Portal: A visual object on the map used to denote two movements in seperate spaces
		Anchor point = middle
		base point = left of anchor in base direction (ledge going down)
		           = base point is inversed if "inverseTop" is true
				   = abstract
*/

class CPortal: public CObject
{
protected:
	SDL_Surface* _imgBase;		// base image.  drawn last
	SDL_Surface* _imgLedge;		// ledge image.  drawn first

	CPlayer* owner;		// owner of the portal
	CTile* loc;			// tile portal is located on.  cannot rely on x and y loc, since it is displaced
	bool type;			// red or blue portal?
	bool inverseBase;	// Is the base of the portal inversed?  Used to keep left-right and up-down portals consistent
	int enterDir;	// either 0, 90, 180, or 270.  Dir objects face into portal

private:
	void Initialize();
	void Delete();

public:
	CPortal(CPlayer* _owner, bool _type);
	virtual ~CPortal();

	bool isActive();		// true when portal has corresponding pair
	bool canPlace(CTile* _loc, int projDir);	// can this portal be placed on this tile?
	bool inCollision(CObject* obj, double xloc, double yloc);	// is an object in the portal's domain?  (mirror imgs)
	bool inPortal(CObject* obj, double objXloc, double objYloc);	// is this object in the portal's range of usage?
	bool inWarpArea(CDynamic* obj, double objXloc, double objYloc);	// is this object in the warp area?
	bool isInversed() { return inverseBase; }
	void Place(CTile* _loc, int projDir);

	int getEnterDir() { return enterDir; }
	int getExitDir() { return normAngle(enterDir + 180); }
	int getType() { return type; }
	CPortal* getOther();	// returns corresponding pair.  if does not exist, returns NULL

	void SetLocation(CTile* _loc);
	void UpdateState();
	void UpdateImage();
	void draw(Client* client);
	void drawBase(Client* client);
	void drawLedge(Client* client);
	
};

#endif