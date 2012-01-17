
#include "CVisual.h"

// CONSTRUCTORS
CVisual::CVisual()
{
	Initialize();
}

CVisual::CVisual(double _x, double _y)
{
	Initialize();
	setLoc(_x, _y);
}

CVisual::~CVisual()
{

}

void CVisual::Initialize()
{
	setCrop(0, 0, 0, 0);
	setLoc();
	setDimensions(0, 0);
}

// ACCESSORS
void CVisual::setLoc()
{
	x = y = 0;
	display = false;
}

void CVisual::setLoc(double _x, double _y)
{
	display = true;
	x = _x; y = _y;
//	x = lim<double>(_x, 0, MAP_WIDTH);	
//	y = lim<double>(_y, 0, MAP_HEIGHT);
	if(x < 0) setLoc();
}

void CVisual::setCrop(int cx, int cy, int cw, int ch)
{
	crop.x = cx;	crop.y = cy;
	crop.w = cw;	crop.h = ch;
}

void CVisual::setImage(string imgname, string imgstate)
{
	setImage(imgname);
	setImageState(imgstate);
}

void CVisual::setImage(string imgname)
{
	Image* oldimage = img;
	img = imgHandler->getImage(imgname);
	if(img == NULL) img = oldimage;
}

void CVisual::setImageState(string state)
{
	if(image_state != state)
		image_state = state;
	UpdateImage();
}

// METHODS

void CVisual::LifeCycle()
{

}

void CVisual::UpdateImage()
{
	_img = img->getImage(getImageState(), 0, false);
}

void CVisual::draw()
{
	if(!display) return;
	ApplySurface(_img, int(x), int(y));
}