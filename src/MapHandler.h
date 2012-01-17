
#ifndef _MAPLOADER_H
#define _MAPLOADER_H

#include "main.h"

const int 
	READINFO	= 0x01,
	READKEY		= 0x02,
	READMAP		= 0x04,
	READIGNORE	= 0x08;

const string 
	infoTag		= "==info==",
	keyTag		= "==key==",
	mapTag		= "==map==";

class MapHandler
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
	MapHandler(string);
	MapHandler();
	virtual ~MapHandler();

	void OpenFile(string);
	void ClearData();

	void LoadData();
	void ParseData();

	void LoadMap();	

};

#endif;