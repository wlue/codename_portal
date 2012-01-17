
#include "CButton.h"
#include "Game.h"

CButton::CButton(string _type, int _dscreen, int _dstate, int _x, int _y) 
	: CScreenObj(game.getClient(), _type, _x, _y), dscreen(_dscreen), dstate(_dstate)
{
	Initialize();
	setLoc(_x, _y);
}

CButton::~CButton()
{
	Delete();
}

void CButton::Initialize()
{
	image_state = getType();
	setImage("button", image_state);
	UpdateImage();
	setDimensions(_img->w, _img->h);
}

void CButton::Delete()
{

}

bool CButton::onButton(int _x, int _y)
{
	if(getX() <= _x && _x <= getX() + getW() && getY() <= _y && _y <= getY() + getH())
		return true;
	return false;
}

// Event Handling

void CButton::onEvent(Uint8 eventType)
{
	if(!display) return;
	int mouse_x = event.button.x;
	int mouse_y = event.button.y;

	if(onButton(mouse_x, mouse_y))
	{
		if(eventType == SDL_MOUSEBUTTONDOWN)
			onMouseDown();
		else if(eventType == SDL_MOUSEBUTTONUP)
			onClick();
		else if(eventType == SDL_MOUSEMOTION)
			onMouseOver();
	}
	else
		setImageState(getType());
		
}

void CButton::onClick()
{
	setImageState(getType() + "-over");
	game.ButtonAction(this);
}

void CButton::onMouseOver()
{
	setImageState(getType() + "-over");
}	

void CButton::onMouseDown()
{
	setImageState(getType() + "-down");
}

void CButton::UpdateImage()
{
	_img = img->getImage(image_state, 0, false);
}

// Other

void CButton::setLoc(int _x, int _y)
{
	switch(_x)
	{
	case BUTTON_CENTER:
		_x = SCREEN_WIDTH/2 - w/2;
		break;
	case BUTTON_LEFT:
		_x = MENU_MARGIN_LEFT;
		break;
	case BUTTON_RIGHT:
		_x = SCREEN_WIDTH - MENU_MARGIN_RIGHT - w;
		break;
	}

	switch(_y)
	{
	case BUTTON_CENTER:
		_y = SCREEN_HEIGHT/2 - h/2;
		break;
	case BUTTON_TOP:
		_y = MENU_MARGIN_TOP;
		break;
	case BUTTON_BOTTOM:
		_y = SCREEN_HEIGHT - MENU_MARGIN_BOTTOM - h;
	}

	CVisual::setLoc(_x, _y);	
}