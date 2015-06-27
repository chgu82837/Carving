
#include "stdafx.h"

chunk::chunk(void)
{
	this->spots = NULL;
}

void chunk::prepare()
{
	if(spots == NULL){
		spots = new std::map<int,std::set<spot*>*>();
		prepare(SPOT_TYPE_SIMPLE);
	}
}

void chunk::prepare(int type)
{
	prepare();
	if(spots->count(type) == 0){
		spots->emplace(type,new std::set<spot*>());
	}
}

chunk::~chunk(void)
{
	if(spots != NULL)
		delete spots;
}

#define S (*spots)

std::set<spot*>::iterator chunk::iter_spots()
{
	return iter_spots(SPOT_TYPE_SIMPLE);
}

std::set<spot*>::iterator chunk::iter_spots(int type)
{
	prepare(type);
	return S[type]->begin();
}

bool chunk::iter_ends(std::set<spot*>::iterator* i)
{
	return iter_ends(SPOT_TYPE_SIMPLE,i);
}

bool chunk::iter_ends(int type,std::set<spot*>::iterator* i)
{
	prepare(type);
	return *i == S[type]->end();
}

int chunk::size()
{
	int total = 0;
	if(spots == NULL)
		return total;
	for (auto it=spots->begin(); it!=spots->end(); ++it)
		total += it->second->size();
	return total;
}

int chunk::size(int type)
{
	prepare(type);
	return S[type]->size();
}

void chunk::insert(spot* s)
{
	int type = s->getType();
	prepare(type);
	S[type]->insert(s);
}

void chunk::erase(spot* s)
{
	int type = s->getType();
	prepare(type);
	S[type]->erase(s);
}

void chunk::hightlight(int x,int z,float size)
{
	set_mtl(MTL_hurtable_fire);
	glPushMatrix();
	glTranslatef(x + 0.5,0.1,z + 0.5);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRectf(size, size, -size, -size);
	glPopMatrix();
	pop_mtl();
}