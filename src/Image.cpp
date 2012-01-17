
#include "Image.h"

Image::Image(string _name, string pPath, int _w, int _h)
{
	name = _name;
	prevPath = pPath;
	w = _w; h = _h;
}

Image::~Image()
{
	map<string, ImageState*>::iterator imgItr;
	for(imgItr = image_states.begin(); imgItr != image_states.end(); ++imgItr)
	{
		ImageState* currState = imgItr->second;
		delete currState;
	}
}

SDL_Surface* Image::getImage(string state, int mod, bool asDir)
{
	ImageState* imgstate = image_states[state];
	if(!imgstate) return NULL;

	int frame = imgstate->isAnimated() ? 
					frameState(counter, imgstate->getFrames(), imgstate->getFrameSpeed()) 
					: 0;

	if(asDir)	mod = mod / (360 / ANGLE_PRECISION);

	SDL_Surface* image = imgstate->getFrameImage(this, frame, mod);

	return image;
}


void Image::AddState(string name, vector<string>* filenames, int frame_speed, Uint8 setflags)
{
	
	bool useAngleStates = setflags & IMGSET_USEANGLES	? true : false;
	bool rewindStates	= setflags & IMGSET_REWIND		? true : false;


	for(vector<string>::iterator fnItr = filenames->begin(); fnItr != filenames->end(); ++fnItr)
	{
		*fnItr = IMAGE_DIRECTORY + this->prevPath + *fnItr;
	}	
	
	ImageState* state = new ImageState(filenames, frame_speed, rewindStates, useAngleStates);
	image_states[name] = state;
}

void Image::AddState(string name, string filename, Uint8 settings)
{
	bool useAngleStates = settings & IMGSET_USEANGLES	? true : false;
	bool noSmooth = settings & IMGSET_NOSMOOTH ? true : false;
	
	filename = IMAGE_DIRECTORY + this->prevPath + filename;
	vector<string>* states = new vector<string>; states->push_back(filename);

	ImageState* state = new ImageState(filename, useAngleStates, noSmooth);
	image_states[name] = state;

}

void Image::AddAutojoinState(string name, string filename)
{
	filename = this->prevPath + filename;

	// add image state with frames as autojoin states
	ImageState* state = new ImageState(filename);
	image_states[name] = state;

}
