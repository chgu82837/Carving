
#include "stdafx.h"

void spot::init(float z,float x,float y,int type)
{
	this->z = z;
	this->x = x;
	this->y = y;
	this->type = type;
	if(type >= 0) scene->get_chunk(z,x)->insert(this);
}

spot::spot(float z,float x,float y)
{
	init(z,x,y,SPOT_TYPE_SIMPLE);
}

spot::spot(float z,float x,float y,int type)
{
	init(z,x,y,type);
}

spot::~spot(void)
{
	if(type >= 0) scene->get_chunk(this->z,this->x)->erase(this);
}

float spot::getx() { return x; }
int spot::getix() { return (int) floor(x); }
int spot::getnix() { return scene->normalize_position(x); }

float spot::gety() { return y; }
//int spot::getiy() { return normalize_position(x); }

float spot::getz() { return z; }
int spot::getiz() { return (int) floor(z); }
int spot::getniz() { return scene->normalize_position(z); }

int spot::getType() { return type; }

void spot::setzxy(float z,float x)
{
	if((scene->normalize_position(z) != (scene->normalize_position(this->z))) || (scene->normalize_position(x) != scene->normalize_position(this->x))){
		scene->get_chunk(this->z,this->x)->erase(this);
		scene->get_chunk(z,x)->insert(this);
		if(debug) printf("spot to (z,x) (%d,%d)\n",scene->normalize_position(z),scene->normalize_position(x));
	}
	this->z = z;
	this->x = x;
}

void spot::setzxy(float z,float x,float y)
{
	this->setzxy(z,x);
	this->y = y;
}

float spot::distance(spot* s)
{
	float a = (x - s->x) * (x - s->x) + (y - s->y) * (y - s->y) + (z - s->z) * (z - s->z); 
	return a;
}

void spot::setType(int type)
{
	if(type == this->type) return;
	if(this->type >= 0) scene->get_chunk(z,x)->erase(this);
	this->type = type;
	if(type >= 0) scene->get_chunk(z,x)->insert(this);
}

bool spot::isTmp()
{
	return type < 0;
}