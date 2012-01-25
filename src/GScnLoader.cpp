
#include "GScnLoader.h"

using namespace gscnloader;

GScnLoader::GScnLoader()
{
	LoadScenarios();
}

void GScnLoader::LoadScenarios()
{
	ifstream index (SCENARIO_INDEX.c_str());
	if(!index.is_open())
	{
		cout << "Error: Could not open index file at \"" << SCENARIO_INDEX << "\"." << endl;
		return;
	}

	while(!index.eof())
	{
		string dir;
		getline(index, dir);
		if (dir.size() > 0) {
			cout << "Loading scenario: " << dir << endl;
			Load(dir);
		}
	}
	
	index.close();
}

void GScnLoader::Load(string folder)
{
	string directory = LEVEL_DIRECTORY + "/" + folder + "/";
	string info = directory + "info.scn";
	string imageDir = directory + "index.png";

	ifstream file (info.c_str());
	if(!file.is_open())
	{
		cout << "Error: Could not open scenario info file at \"" << info << "\"." << endl;
		return;
	}
	
	// LOADING
	GScenario* scenario = new GScenario();
	scenario->LoadPreviewImg(imageDir);
	string scn_name;
	string scn_diff;

	int rFlags = 0;					// read flags
	string rLvl = "";				// read level (ie. @level3).  Set when prefix == levelsID
	map<string, string> pLevels;	// parsed level info (ie. "@level2"="a=1 b=2 c=3")

	while(!file.eof())
	{
		string line;
		getline(file, line);
		if(line.empty()) continue;
		if(line == "/*") rFlags |= READIGNORE;	// C-style comments in script file
		if(line == "*/") rFlags &= ~READIGNORE;
		if(rFlags & READIGNORE || line.substr(0, 1) == "!") continue; // skip line when comments flag is on

		if(line == infoTag)			rFlags ^= READINFO;
		else if(line == levelTag)	rFlags ^= READLEVELS;

		if(rFlags & READINFO)
		{
			string key, val;
			GetStringSides(key, val, line, "=");
			if(key == "name")			scn_name = val;
			if(key == "difficulty")		scn_diff = val;
		}

		if(rFlags & READLEVELS)
		{
			if(line.substr(0, levelID.size()) == levelID)
				rLvl = line;
			else if(!rLvl.empty())
			{
				if(!pLevels[rLvl].empty()) pLevels[rLvl].append("\n");
				pLevels[rLvl].append(line);
			}
		}
	}

	// PARSING

	// scenario
	scenario->Configure(scn_name, directory, scn_diff);

	// levels
	map<string, string>::iterator lvlItr;
	for(lvlItr = pLevels.begin(); lvlItr != pLevels.end(); ++lvlItr)
	{
		GLevel* level = new GLevel(scenario);
		string read = lvlItr->second;

		string name, filename;
		int difficulty, timelimit;

		stringstream lvl_stream (stringstream::in | stringstream::out);
		lvl_stream << read;
		string decl;
		while(!lvl_stream.eof())
		{
			getline(lvl_stream, decl);
			string key, val;
			GetStringSides(key, val, decl, "=");

			if(key == "name")			name = val;
			if(key == "filename")		filename = val;
			if(key == "difficulty")		difficulty = str2val<int>(val);
			if(key == "timelimit")		timelimit = str2val<int>(val);
		}
		if(fileExists(directory + filename))
			level->Configure(name, filename, difficulty, timelimit);
		else
		{	
			cout << "Error: Level \"" << name << 
				"\" removed because level file could not be loaded. (" << 
				directory + filename << ")" << endl;
			delete level;
		}
	}
}