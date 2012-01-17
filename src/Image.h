
#ifndef _IMAGE_H
#define _IMAGE_H

#include "main.h"
#include "ImageState.h"

class Image
{
private:
	string name;
	string prevPath;		// previous path (not including /rsc/image) where images will be looked at
	map<string, ImageState*> image_states;
	int w, h;

public:
	Image(string, string, int, int);		// name, pPath, width, height
	virtual ~Image();

	string getName() { return name; }
	SDL_Surface* getImage(string, int, bool);	// image state, mod, asDir
	bool isAnimated(string state) { return image_states[state] ? image_states[state]->isAnimated() : false; };
	int getJoinMethod(string state) { return image_states[state] ? image_states[state]->getJoinMethod() : AJ_NONE; };

	inline int getWidth() { return this->w; };
	inline int getHeight() { return this->h; };

	void AddState(string, vector<string>*, int, Uint8); //, bool, bool);	
	void AddState(string, string, Uint8);
	void AddAutojoinState(string, string);

	bool isAutoJoining(string s) { return image_states[s] != NULL ? image_states[s]->isUsingAutoJoining() : false; };


};


#endif