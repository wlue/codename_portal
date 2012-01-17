
#include "CTerrain.h"
#include "CTile.h"

CTerrain::CTerrain(string _type, int _x, int _y) : CMovable(_x, _y), type(_type)
{
	Initialize();
	setLoc(getX(), getY() + TILE_HEIGHT/2 - getH()/2);
}

CTerrain::~CTerrain()
{
	Delete();
}

void CTerrain::Initialize()
{
	listter.push_back(this);
	nangle = 0;
	setImage("item", type);
	density = true;
	if(_img != NULL)	setDimensions(_img->w, _img->h);
	else				setDimensions(0, 0);
}

void CTerrain::Delete()
{
	RemoveElement(listter, this);
}

void CTerrain::Bump(CObject* obj)
{
	if(type == "spikeball")
	{
		CTile* tile = dynamic_cast<CTile*>(obj);
		if(tile != NULL)
		{
			if(tile->canShatter()) {
				tile->Shatter();
				return;
			}
			else if(tile->canBreak())
				tile->Break();
		}
	}
	CMovable::Bump(obj);

}