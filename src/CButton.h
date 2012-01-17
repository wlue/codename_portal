
#ifndef _CBUTTON_H
#define _CBUTTON_H

#include "main.h"
#include "CScreenObj.h"

// CButton is a button clickable by the player that triggers game events
class CButton: public CScreenObj
{
protected:
	string orig_state;
	int dscreen;
	int dstate;

private:
	void Initialize();
	void Delete();

	void onClick();				// called when successfully clicked on
	void onMouseOver();			// called when mouse is moved over button
	void onMouseDown();			// called when mouse clicks down

public:
	CButton(string _type, int _dscreen, int _dstate, int _x, int _y);
	CButton(string _type, string _orig_state, int _dscreen, int _dstate, int _x, int _y);
	virtual ~CButton();
	bool onButton(int _x, int _y);
	bool inScreen(int screen, int state) { return dscreen == screen && dstate == state; }
	int getScreen() { return dscreen; }
	int getState() { return dstate; }

	void setLoc(int _x, int _y);

	void onEvent(Uint8 eventType);		// called when an event triggers the button
	void UpdateImage();


};

#endif