
#ifndef _IMAGESTATE_H
#define _IMAGESTATE_H

#include "main.h"

class ImageState
{
private:
	vector<surface_vector> stateData;
	int frame_speed;
	bool animate;
	bool smoothAngles;
	bool useAngleSurface, rewind;	// use directional images?  rewind frames?

	bool useAutoJoining;			// if true, represent directional surfaces as autojoining states.  always 255 in length
	int joinMethod;					// method used to determine what type of joining to use.


public:
	ImageState(vector<string>*, int, bool, bool);	// filenames, frame speed, rewind, useAngleStates
	ImageState(string, bool, bool);	// filename, useAngleStates, nosmooth
	ImageState(string);			// for autojoining states 
	~ImageState();

	bool isAnimated() { return animate; };
	bool isUsingAngles() { return useAngleSurface; };
	bool isUsingAutoJoining() { return useAutoJoining; };
	int getJoinMethod() { return joinMethod; };
	int getFrameSpeed() { return frame_speed; };
	int getFrames() { return int(stateData.size()); };

	SDL_Surface* getFrameImage(Image*, int, int);				// frame, mod

	void SmoothAngles();
	void AddFrame(string);
	void AddAutojoinState(string);
	void GenerateRepeatFrames();


};

#endif
