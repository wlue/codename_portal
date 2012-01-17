
#ifndef _CVISUAL_H
#define _CVISUAL_H

#include "main.h"
#include "ImageHandler.h"

// An object displayed on the screen
class CVisual
{
protected:
	Image* img;
	SDL_Surface* _img;
	string image_state;
	SDL_Rect crop;

	double x, y;	// relative from top-left of screen
	int w, h;		// width, height
	bool display;	// internal; determines if obj is off the screen
	bool willCrop;

private:
	virtual void Initialize();

public:
	CVisual();
	CVisual(double _x, double _y);
	virtual ~CVisual();

	// ACCESSORS
	Image* getImage() { return img; }
	SDL_Surface* getIntImage() { return _img; }
	string getImageState() { return image_state; }
	bool isDisplay() { return display; }
	bool isCrop() { return willCrop; }

	double getX() { return x; }
	double getY() { return y; }
	int getW() { return w; }
	int getH() { return h; }

	// MUTATOR METHODS
	virtual void setLoc();
	virtual void setLoc(double _x, double _y); 
	void setDimensions(int _w, int _h) { w = _w; h = _h; }
	void setCrop(int cx, int cy, int cw, int ch);
	void addCrop() { willCrop = true; }
	void removeCrop() { willCrop = false; }

	void setImage(string imgname, string imgstate);
	void setImage(string imgname);
	void setImageState(string state);

	virtual void LifeCycle();
	virtual void UpdateImage();
	virtual void draw();
};

#endif