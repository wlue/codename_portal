
#ifndef _GSCENARIO_H
#define _GSCENARIO_H

#include "main.h"

// An object grouping levels in similarity and difficulty
class GScenario
{
private:
	SDL_Surface* prevImage;		// preview image
	string name;
	string filedir;
	string difficulty;
	vector<GLevel*> levels;
	friend GLevel;

public:
	GScenario();
	~GScenario();

	string getName() { return name; }
	string getFileDir() { return filedir; }
	string getDifficulty() { return difficulty; }
	vector<GLevel*>& getLevels() { return levels; }
	int getLevelSize() { return int(levels.size()); }
	SDL_Surface* getPrevImg() { return prevImage; }

	GLevel* nextLevel(GLevel* start, int diff) { return nextItem(levels, start, diff); }
	void Configure(string _name, string _filefolder, string _diff);
	void LoadPreviewImg(string dir) { prevImage = load_image(dir); }

};

#endif