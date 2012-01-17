
#ifndef _CDOOR_H
#define _CDOOR_H

#include "main.h"
#include "Game.h"
#include "CTile.h"
#include "CPlayer.h"

class CDoor: public CTile
{
protected:
	bool exit;

private:
	void Initialize();
	void Delete();

public:
	CDoor(int _x, int _y);
	virtual ~CDoor();

	bool isExit() { return exit; }
	bool isEntrance() { return !exit; }
	void SetAttribute(string var, string val);
};

#endif