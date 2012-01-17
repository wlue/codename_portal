
#ifndef _CSCREENOBJ_H
#define _CSCREENOBJ_H

#include "main.h"
#include "CVisual.h"

// CScreenObj is an object located on the screen (ie. button, hud)
class CScreenObj: public CVisual
{
protected:
	Client* client;
	string type;

private:
	void Initialize();
	void Delete();

public:
	CScreenObj(Client* _client, string _type, int _x, int _y);
	virtual ~CScreenObj();

	void setDisplay(bool d) { display = d; }
	string getType() { return type; }
	Client* getClient() { return client; }

};

#endif