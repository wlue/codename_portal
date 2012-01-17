
#include "CScreenObj.h"

CScreenObj::CScreenObj(Client* _client, string _type, int _x, int _y) : CVisual(_x, _y), client(_client), type(_type)
{
	Initialize();
}

CScreenObj::~CScreenObj()
{
	Delete();
}

void CScreenObj::Initialize()
{

}

void CScreenObj::Delete()
{

}