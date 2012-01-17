
#ifndef _IMAGEHANDLER_H
#define _IMAGEHANDLER_H

#include "main.h"
#include "Image.h"
#include "ImageState.h"

class ImageHandler
{
private:
	map<string, Image*> images;
public:
	ImageHandler();
	virtual ~ImageHandler();

	Image* getImage(string s) { return images[s]; };

	void LoadImages();
	void Clear();	// frees all surfaces and sets to NULL
	
	void ImportImage(Image*);	// file name

};

#endif