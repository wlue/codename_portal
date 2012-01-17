
#ifndef _GMAPLOADER_H
#define _GMAPLOADER_H

#include "main.h"

// Internal flag variables
namespace gmaploader
{
const int 
	READINFO	= 0x01,
	READKEY		= 0x02,
	READMAP		= 0x04,
	READIGNORE	= 0x08;

const string 
	infoTag		= "==info==",
	keyTag		= "==key==",
	mapTag		= "==map==";
}

// Loads the map of a level.  Takes GLevel reference as input
class GMapLoader
{
private:
	string filename;
	string datInfo, datKey, datMap;

	bool loaded;
	int w, h, d;	// width, height, depth
	map<string, string_map> mapKey;

	inline void ParseTile(string&, string&, string);
	inline void ParseAttributes(string&, string&, string, char);
	map<string, string>& getKeyMap(string let) { return mapKey[let]; };


public:
	GMapLoader();
	virtual ~GMapLoader();

	void LoadLevel(GLevel* level);	
	void LoadMap();
	void ClearData();

	void LoadData();
	void ParseData();


};

#endif;