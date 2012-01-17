
#include "main.h"

#include "CTile.h"
#include "CPortal.h"
#include "CDynamic.h"

int SCREEN_WIDTH	= 0;
int SCREEN_HEIGHT	= 0;
int MAP_WIDTH		= 0;
int MAP_HEIGHT		= 0;
int map_maxx		= 0;	// in tiles
int map_maxy		= 0;

vector<CObject*>		deletion;
vector<CObject*>		listobjs;
vector<CTile*>			listtil;
vector<CTerrain*>		listter;
vector<CDynamic*>		listdyn;
vector<CMovable*>		listmov;
vector<CUnit*>			listunt;
vector<CPlayer*>		listply;
vector<CTurret*>		listtrt;
vector<CProjectile*>	listprj;
vector<CProjPortal*>	listppt;
vector<CPortal*>		listptl;
vector<GScenario*>		listscn;

map<string, TTF_Font*> fonts;
map<string, SDL_Color*> fontColors;
map<string, Mix_Chunk*> sfx;

SDL_Rect surfrect;
SDL_Rect surfclip;

SDL_Color* getSDLColor(int r, int g, int b)
{
	SDL_Color* c = new SDL_Color;
	c->r = r;	c->g = g;	c->b = b;
	return c;
}

inline SDL_Surface* TTF_DrawText(string fname, string text, string cname, int flags)
{
	TTF_Font* font = fonts[fname];
	SDL_Surface* textImg = NULL;
	if(flags) TTF_SetFontStyle(font, flags);
	textImg = TTF_RenderText_Solid(font, text.c_str(), *fontColors[cname]);
	if(flags) TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
	return textImg;
}

inline SDL_Surface* TTF_DrawText_Blended(string fname, string text, string cname, int flags)
{
	TTF_Font* font = fonts[fname];
	SDL_Surface* textImg = NULL;
	if(flags) TTF_SetFontStyle(font, flags);
	textImg = TTF_RenderText_Blended(font, text.c_str(), *fontColors[cname]);
	if(flags) TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
	return textImg;
}

void DeleteObj(CObject* obj)
{
	vector<CObject*>::iterator a = find(deletion.begin(), deletion.end(), obj);
	if(a == deletion.end())
		deletion.push_back(obj);
}

void PlaySFX(string sfxname)
{
	Mix_PlayChannel(-1, sfx[sfxname], 0);
}

double randReal()
{
	return rand() / (double(RAND_MAX) + 1);
}

double randReal(double dmin, double dmax)
{
	double width = dmax - dmin;
	return randReal() * width + dmin;
}

int rand(int max)
{
	return int(randReal() * max);
}

int rand(int min, int max)
{
	return min + rand(max - min + 1);
}

bool prob(int percent)
{
	if(percent == 0) return false;
	if(percent == 100) return true;
	return rand(0, 100) <= percent;
}

string sec2clock(int seconds)
{
	int minutes = 0;
	while(seconds >= 60) {
		seconds -= 60;
		minutes++;
	}
	return val2str(minutes)+":"+(seconds<10?"0":"")+val2str(seconds);
}

int angle2dir(int angle)
{
	if(45 >= angle || angle >= 315)
		return NORTH;
	else if(315 >= angle && angle >= 225)
		return EAST;
	else if(225 >= angle && angle >= 135)
		return SOUTH;
	else
		return WEST;
}

int dir2angle(int dir)
{
	switch(dir)
	{
	case NORTH:		return 0;
	case SOUTH:		return 180;
	case WEST:		return 90;
	case EAST:		return 270;
	default:		return 0;
	}
}

SDL_Surface* load_image(string filename) 
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;
	loadedImage = IMG_Load(filename.c_str());
	if(loadedImage != NULL)
	{
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		SDL_SetColorKey(optimizedImage, SDL_RLEACCEL, optimizedImage->format->colorkey);
		SDL_FreeSurface(loadedImage);
	}
	else
		cout << "Image: '" << filename << "' failed loading. (1)" << endl;
	return optimizedImage;
}

SDL_Surface* set_mask(SDL_Surface* surface, int r, int g, int b)
{
	SDL_Surface* optimizedImage = SDL_DisplayFormat(surface);
	SDL_FreeSurface(surface);
	if(optimizedImage != NULL) {
		Uint32 colorkey = SDL_MapRGB(optimizedImage->format, r, g, b);
		SDL_SetColorKey(optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
	}
	return optimizedImage;
}

SDL_Surface* roto_mask(SDL_Surface* surface)
{
	SDL_Surface* optimizedImage = surface;
	return optimizedImage;	
}

void ApplySurface(SDL_Surface* surface, int x, int y)
{
	if(!surface) return;
	surfrect.x = x;	surfrect.y = y;
	SDL_BlitSurface(surface, NULL, screen, &surfrect);
}

void ApplySurface_Rect(SDL_Surface* surface, int x, int y, int px, int py, int pw, int ph)
{
	surfrect.x = x;	surfrect.y = y;
	surfclip.x = px; surfclip.y = py;
	surfclip.w = pw; surfclip.h = ph;
	SDL_BlitSurface(surface, &surfclip, screen, &surfrect);
}

void ApplySurface_Screen(SDL_Surface* surface, int x, int y, int sx, int sy, int sw, int sh)
{
	int surf_x = maxVal(sx - x, 0);
	int surf_y = maxVal(sy - y, 0);

	ApplySurface_Rect
	(
		surface, x + surf_x, y + surf_y, 
		surf_x, surf_y, 
		minVal(sw - x, surface->w), minVal(sh - y, surface->h)
	);
}

bool fileExists(string filename)
{
	ifstream file (filename.c_str());
	if(file.is_open())
	{
		file.close();
		return true;
	}
	file.close();
	return false;
}

inline int normAngle(int angle)
{
	while(angle < 0) angle += 360;
	while(angle >= 360) angle -= 360;
	return angle;
//	return angle % 360;
}

int mirrorAngle(int angle, int dir) // mirrorAngle(x, y) == mirrorAngle(x, y + 180)
{
	int dist = dir - angle;
	return normAngle(dir + dist);
}

int getangle(double x1, double y1, double x2, double y2)
{
	int val = 0;
	double dx = x2 - x1, dy = y2 - y1;
	if(dy == 0) return (dx < 0) ? 90 : 270;
	val = (int)rad2deg(atan(dx / dy));
	if(dy > 0) return val + 180;
	if(val < 0)	return val + 360;
	return val;
}

void getStep(double& nx, double& ny, double dist, double angle)
{
	double xinc = sin(angle * pi/180) * dist;
	double yinc = cos(angle * pi/180) * dist;
	nx = lim<double> (nx - xinc, 0, MAP_WIDTH);
	ny = lim<double> (ny - yinc, 0, MAP_HEIGHT);
}

void getStepDist(double& dx, double& dy, double dist, double angle)
{
	dx = -sin(angle * pi/180) * dist;
	dy = -cos(angle * pi/180) * dist;
}

int dirTurn(int dir, int turn)
{
	return angle2dir(normAngle(dir2angle(dir) + turn));
}

int dirstep(int dir1, int dir2) // step positive, or negative from dir1 to dir2
{
	if(dir1 == dir2) return 0;
	int diff = abs(dir1 - dir2);
	return diff < 180 ? -1 : 1;
}

int dirstepDist(int dir1, int dir2) // closest distance from dir1 to dir2
{
	if(dir1 == dir2) return 0;
	int diff = abs(dir1 - dir2);
	return diff < 180 ? diff : (360 - diff);
}

int tileDist(int x1, int y1, int x2, int y2)
{
	return abs(x1 - x2) + abs(y1 - y2);
}

CTile* locateTile(int tx, int ty)
{
	vector<CTile*>::iterator a;
	for(a=listtil.begin(); a!=listtil.end(); ++a)
		if((*a)->getTX() == tx && (*a)->getTY() == ty)
			return *a;
	return NULL;
}

void getLocationStep(int& x, int& y, int dir, int steps)
{
	if(dir & NORTH)		y -= steps;
	if(dir & SOUTH)		y += steps;
	if(dir & EAST)		x += steps;
	if(dir & WEST)		x -= steps;
}

CTile* getTileStep(CTile* tile, int dir, int steps)
{
	int nx = tile->getTX(), ny = tile->getTY();
	getLocationStep(nx, ny, dir, steps);
	return locateTile(nx, ny);
}


// COLLISION
namespace collision
{

bool box(double Ax, double Ay, int Aw, int Ah, double Bx, double By, int Bw, int Bh)
{
	return !(((By+(Bh>>1))<=(Ay-(Ah>>1))) || ((Ay+(Ah>>1))<=(By-(Bh>>1))) || 
			((Ax+(Aw>>1))<=(Bx-(Bw>>1))) || ((Bx+(Bw>>1))<=(Ax-(Aw>>1))));
}

bool circle(double Ax, double Ay, int Ar, double Bx, double By, int Br)
{
	return distance_bare(Ax, Ay, Bx, By) < (Ar*Ar + Br*Br);
}

bool circlebox(double Ax, double Ay, int Ar, double Bx, double By, int Bw, int Bh)
{
	double leftB = Bx - (Bw>>1), rightB  = Bx + (Bw>>1),
			topB = By - (Bh>>1), bottomB = By + (Bh>>1);

	if( (((Ay + Ar) > topB) && ((Ay - Ar) < bottomB) && (Ax > leftB) &&	(Ax < rightB)) || 
		((Ay > topB) && (Ay < bottomB) && ((Ax + Ar) > leftB) &&((Ax - Ar) < rightB)) )
		return true;

	double bare_radius = Ar * Ar;
	if(	(distance_bare(Ax, Ay, Bx - (Bw>>1), By - (Bh>>1)) < bare_radius) ||
		(distance_bare(Ax, Ay, Bx + (Bw>>1), By - (Bh>>1)) < bare_radius) ||
		(distance_bare(Ax, Ay, Bx - (Bw>>1), By + (Bh>>1)) < bare_radius) ||
		(distance_bare(Ax, Ay, Bx + (Bw>>1), By + (Bh>>1)) < bare_radius) )
		return true;

	return false;
}

} // COLLISION

// Portal Calculations
namespace portal
{

int calcObjAngle(CPortal* enter, CPortal* exit, int angle, int angleDiff)
{
	int newVisAngle;
	newVisAngle = mirrorAngle(angle + angleDiff, angleDiff);
	if(enter->getEnterDir() % 180 == 0)
		newVisAngle = normAngle(newVisAngle + 180);

	if(enter->isInversed() != exit->isInversed()) // base inversed: mirror directions
		newVisAngle = mirrorAngle(newVisAngle, exit->getEnterDir());
	
	return newVisAngle;
}


void calcStepDist(double& dx, double& dy, CPortal* enter, CPortal* exit, CDynamic* obj, int angleDiff)
{
	int objAngle = getangle(enter->getX(), enter->getY(), obj->getX(), obj->getY());
	int newObjAngle = normAngle(objAngle + angleDiff);
	double dist = distance(enter->getX(), enter->getY(), obj->getX(), obj->getY());
	getStepDist(dx, dy, dist, newObjAngle);

	if(exit->getEnterDir() % 180 == 0)	
		dy = -dy;
	else						
		dx = -dx;

	if(enter->isInversed() != exit->isInversed()) // base inversed: mirror dx/dy
	{
		if(exit->getEnterDir() % 180 == 0)	
			dx = -dx;
		else						
			dy = -dy;
	}
	dx = round(dx);
	dy = round(dy);
}

void calcStepVel(double& vx, double& vy, CPortal* enter, CPortal* exit, CDynamic* obj, int angleDiff)
{
	int objAngle = getangle(0, 0, vx, vy);
	int newObjAngle = normAngle(objAngle + angleDiff);
	double dist = sqrt(vx*vx + vy*vy);
	getStepDist(vx, vy, dist, newObjAngle);

	if(exit->getEnterDir() % 180 == 0)	
		vy = -vy;
	else						
		vx = -vx;

	if(enter->isInversed() != exit->isInversed()) // base inversed: mirror dx/dy
	{
		if(exit->getEnterDir() % 180 == 0)	
			vx = -vx;
		else						
			vy = -vy;
	}
	
	vx = round(vx);
	vy = round(vy);
}


bool inArea(CObject* obj, double xloc, double yloc)
{
	vector<CPortal*>::iterator a;
	for(a=listptl.begin(); a!=listptl.end(); ++a)
		if((*a)->isActive() && (*a)->inCollision(obj, xloc, yloc))
			return true;
	return false;

}

bool inAreaOfEffect(CObject* obj, double xloc, double yloc)
{
	vector<CPortal*>::iterator a;
	for(a=listptl.begin(); a!=listptl.end(); ++a)
		if((*a)->isActive() && (*a)->inPortal(obj, xloc, yloc))
			return true;
	return false;

}

CPortal* inWarpArea(CDynamic* obj, double xloc, double yloc)
{
	vector<CPortal*>::iterator a;
	for(a=listptl.begin(); a!=listptl.end(); ++a)
		if((*a)->isActive() && (*a)->inWarpArea(obj, xloc, yloc))
			return *a;
	return NULL;

}

}


/*

void calcStepDist(double& dx, double& dy, CPortal* enter, CPortal* exit, CDynamic* obj, int angleDiff)
{
	int objAngle = getangle(enter->getX(), enter->getY(), obj->getX(), obj->getY());
	int newObjAngle = normAngle(objAngle + angleDiff);
	double dist = distance(enter->getX(), enter->getY(), obj->getX(), obj->getY());
	getStepDist(dx, dy, dist, newObjAngle);

	if(exit->getEnterDir() % 180 == 0)	
		dy = -dy;
	else						
		dx = -dx;

	if(enter->isInversed() != exit->isInversed()) // base inversed: mirror directions and dx/dy
	{
		if(exit->getEnterDir() % 180 == 0)	
			dx = -dx;
		else						
			dy = -dy;
	}
}
*/