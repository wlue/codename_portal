
#ifndef _CPLAYER_H
#define _CPLAYER_H

#include "main.h"
#include "CUnit.h"

class CPlayer: public CUnit
{
protected:
	Client* owner;		// owner of the player
	int health;			// the amount of health the player has (maximum of PLAYER_MAXHEALTH)
	CPortal* portal1;	// portal1 instance (red)
	CPortal* portal2;	// portal2 instance (blue)

private:
	void Initialize();
	void Delete();

public:
	CPlayer(Client* _owner);
	virtual ~CPlayer();
	void Reset();		// Called when game restarts/starts

	int getHealth() { return health; }
	bool canShoot(bool t);
	void getPortals(CPortal* A, CPortal* B) { A = portal1; B = portal2; }
	CPortal* getPortal1() { return portal1; }
	CPortal* getPortal2() { return portal2; }
	bool inDoor();

	void setHealth(int h);
	void setPortal1(CPortal* p) { portal1 = p; }
	void setPortal2(CPortal* p) { portal2 = p; }

	void DeathCheck();
	void Shoot(int type);
	void Jump();
	void PlacePortal(CTile* loc, bool type, int projDir);

	void UpdateImage();
	void UpdateMirrorImage(int newVisAngle, int newAnchAngle);

	void UpdateMovement();
	void Bump(CObject* obj);

};

#endif