
#include "MapHandler.h"
#include "CTile.h"
#include "CTurret.h"

MapHandler::MapHandler(string file)
{
	OpenFile(file);
}

MapHandler::MapHandler()
{
	datInfo = datKey = datMap = "";
	loaded = false;
}

MapHandler::~MapHandler()
{
	
}

void MapHandler::OpenFile(string file)
{
	ClearData();
	filename = file;
	LoadData();
	ParseData();
}

void MapHandler::ClearData()
{
	filename = "";
	datInfo = datKey = datMap = "";
	loaded = false;
	mapKey.clear();
}

void MapHandler::ParseTile(string& objtype, string& tiletype, string parse)
{
	objtype = parse.substr(0, parse.find('('));
	tiletype = parse.substr(parse.find('(') + 1, parse.length() - objtype.length() - 2);
}

void MapHandler::ParseAttributes(string& tiletype, string& tileatr, string tileinfo, char delim)
{
	string::size_type loc = tileinfo.find(delim, 0);
	if(loc != string::npos) // if delim was found
	{
		tiletype = tileinfo.substr(0, loc);
		tileatr = tileinfo.substr(loc + 1, tileinfo.length());
	}
	else
	{
		tiletype = tileinfo;
	}
}

void MapHandler::LoadMap()
{
	if(!loaded)
	{
		cout << "Error:  Cannot load map when data not loaded and parsed." << endl;
		return;
	}

	map_maxx = w;
	map_maxy = h;

	MAP_WIDTH = w * TILE_WIDTH;
	MAP_HEIGHT = h * TILE_HEIGHT;

	// load through map tiles
	stringstream stream (stringstream::in | stringstream::out);
	stream << datMap;

	for(int y=0; y < (int) map_maxy; ++y)
	{
		for(int x=0; x < (int) map_maxx; ++x)
		{
			string tText;	stream >> tText;
			if(tText.empty()) { cout << "Error:  Map ran out of bounds." << endl; return; };
			
			map<string, string> tiles = getKeyMap(tText);
			map<string, string>::iterator tilesItr;
			for(tilesItr = tiles.begin(); tilesItr != tiles.end(); ++tilesItr)
			{
				string objtype = tilesItr->first;
				string tileinfo = tilesItr->second;
				string tiletype, tileatr;

				ParseAttributes(tiletype, tileatr, tileinfo, '|');

				CObject* newObj = NULL;
				if(objtype == "tile")		newObj = new CTile(tiletype, x, y);
				if(objtype == "turret")		newObj = new CTurret(x, y);

				if(newObj != NULL)
				{
//					if(!tileatr.empty())
//						newObj->SetAttributes(tileatr);
				}
				else
				{
					cout << "Unknown type: " << tiletype << endl;
				}
			}
		}
	}
}

void MapHandler::ParseData()
{
	// parse info
	stringstream info_stream (stringstream::in | stringstream::out);
	info_stream << datInfo;

	string data = "";
	while(info_stream >> data)
	{
		string key;
		int value;
		GetSides<string, int> (key, value, data, "=");

		if(key == "width")		w = value;
		if(key == "height")		h = value;

		data = "";
	}

	// parse keys
	stringstream key_stream (stringstream::in | stringstream::out);
	key_stream << datKey;

	string keylines = "";
	while(key_stream >> keylines)
	{
		string key, value;
		GetSides<string, string> (key, value, keylines, "=");

		vector<string> valList;
		SplitValues(valList, value, '+');
		
		map<string, string> typeIdent;

		for(vector<string>::iterator valItr = valList.begin(); valItr != valList.end(); ++valItr)
		{
			string tval = *valItr; // a string formatted as such: objtype(tiletype)
			string objtype, tiletype;
			ParseTile(objtype, tiletype, tval);

			if(objtype.empty()) { 
				cout << "Error: objtype empty: '" << tval << "'."; return;
			}

			typeIdent[objtype] = tiletype;
		}

		mapKey[key] = typeIdent;

		keylines = "";
	}


	loaded = true;
}


void MapHandler::LoadData()
{
	ifstream file (filename.c_str());
	if(!file.is_open())
	{
		cout << "Error: Could not open file '" << filename << "'." << endl;
		return;
	}

	int status = 0;

	while(!file.eof())
	{
		string line = "";
		getline(file, line);

		if(line == "/*") status |= READIGNORE;
		if(line == "*/") status &= ~READIGNORE;
		if(status & READIGNORE || line.substr(0, 1) == "!") continue; // comments

		if(line == infoTag)		status ^= READINFO;
		else if(line == keyTag)	status ^= READKEY;
		else if(line == mapTag)	status ^= READMAP;

		else if(status)
		{
			switch(status)
			{
			case READINFO:
				if(!datInfo.empty()) datInfo = datInfo + " ";
				datInfo += line;
				break;
			case READKEY:
				if(!datKey.empty()) datKey = datKey + " ";
				datKey += line;
				break;
			case READMAP:
				if(!datMap.empty()) datMap = datMap + " ";
				datMap += line;
				break;
			}
		}
	}

	file.close();
}