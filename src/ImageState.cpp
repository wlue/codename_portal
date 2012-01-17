
#include "ImageState.h"
#include "Image.h"

ImageState::ImageState(vector<string>* files, int fs, bool r, bool angle)
{
	frame_speed = fs;
	useAutoJoining = false;
	joinMethod = AJ_NONE;
	useAngleSurface = angle;
	rewind = r;

	vector<string>::iterator filesItr;
	for(filesItr = files->begin(); filesItr != files->end(); ++filesItr)
	{
		string filename = *filesItr;
		AddFrame(filename);
	}

	animate = (fs != 0) && (files->size() > 1);
	GenerateRepeatFrames();
}

ImageState::ImageState(string filename, bool angle, bool nosmooth)
{
	useAutoJoining = false;
	smoothAngles = !nosmooth;
	joinMethod = AJ_NONE;
	useAngleSurface = angle;
	AddFrame(filename);
}

ImageState::ImageState(string filepref)
{
	useAutoJoining = true;
	joinMethod = AJ_NONE;
	AddAutojoinState(filepref);
}

ImageState::~ImageState()
{
	
}

SDL_Surface* ImageState::getFrameImage(Image* img, int frame, int mod)
{
	vector<SDL_Surface*>& modList = stateData.at(frame);

	if(mod > int(modList.size()) || mod < 0)
	{
		cout << "Error:  dir_state out of bounds.  Mod: " << mod << " Frame: " << frame << endl;
		cout << img->getName() << endl;
		return NULL;
	}

	SDL_Surface* image = modList.at(mod);
	return image;
}

void ImageState::AddFrame(string filename)
{
	SDL_Surface* image = load_image(filename);
#ifdef SHOW_LOADING_MESSAGE
	cout << "Loading image: " << filename << endl;
#endif
	if(image == NULL)
		return;

	vector<SDL_Surface*> dirlist;

	if(useAngleSurface)
	{
		int step = 360 / ANGLE_PRECISION;
		for(int i=0; i<ANGLE_PRECISION; i++)
		{
			SDL_Surface* rotosurface = rotozoomSurface(image, i * step, 1, smoothAngles ? SMOOTHING_ON : SMOOTHING_OFF);
			rotosurface = SDL_DisplayFormatAlpha(rotosurface);
			dirlist.push_back(rotosurface);
		}
	}
	else
	{
		dirlist.push_back(image);
	}
	
//	cout << "  dirlist.size(): " << dirlist.size() << endl;

	stateData.push_back(dirlist);
}

void ImageState::AddAutojoinState(string filepref)
{
	vector<SDL_Surface*> ajlist;

	// Insert 255 objects in vector, and load images using prefix
	const int size = 256;
	int state_count = 0;		// used to determine what type of joining will be used
	for(int i = 0; i < size; ++i)
	{
		string filename = IMAGE_DIRECTORY + filepref + "-" + val2str<int>(i) + ".png";
		if(fileExists(filename))
		{
			SDL_Surface* ajState = load_image(filename);
			ajlist.push_back(ajState);
			state_count++;
		}
		else
			ajlist.push_back(NULL);

	}

	if(ajlist.size() != size)
	{
		cout << "Error: autojoin state list not " << size << ": " << ajlist.size() << endl;
		return;
	}

	switch(state_count)
	{
	case 16:		joinMethod = AJ_16STATE; break;
	case 47:		joinMethod = AJ_47STATE; break;
	case 161:		joinMethod = AJ_161STATE; break;
	case 256:		joinMethod = AJ_256STATE; break;
	default:		cout << "Error: Unknown join method for " << filepref << ": " << state_count << " states found." << endl; break;
	}

	stateData.push_back(ajlist);

}

void ImageState::GenerateRepeatFrames()
{
	if(!isAnimated() || !rewind) return;

	for(int i = (int) stateData.size() - 1; i > 0; i--)
	{
		vector<SDL_Surface*>* currState = &stateData.at(i);
		vector<SDL_Surface*> newstate;
		newstate.assign(currState->begin(), currState->end());
		stateData.push_back(newstate);
	}

}