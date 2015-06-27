
#include "stdafx.h"

hurtable::hurtable(int type,int mtl_i,float thick,float x1,float z1,float x2,float z2,int cnt,int damage) :
	wall(mtl_i,thick,x1,z1,x2,z2,SPOT_TYPE_HURTABLE)
{
	this->type = type;
	this->cd = cnt;
	this->cd_init = cnt;
	this->damage = damage;
}

hurtable::~hurtable()
{
}

bool hurtable::tick(){
	if(cd <= 0){
		scene->toRemove = this;
	}
	cd--;

	glPushMatrix();
	glTranslatef(0,((float) cd / (float) cd_init) * 10.0,0);
	switch(type){
	case HURTABLE_TYPE_NONE:
		break;
	case HURTABLE_TYPE_SPHERE:
		set_obj_type(true);
		this->draw();
		pop_obj_type();
		break;
	case HURTABLE_TYPE_CUBE:
		set_obj_type(false);
		this->draw();
		pop_obj_type();
		break;
	}
	glPopMatrix();

	if(cd == 3){
		chunk* ct;
		character* c;
		int x1 = getix() - WALL_MAX_L * 2;
		int z1 = getiz() - WALL_MAX_L * 2;
		int x2 = getix() + WALL_MAX_L * 2;
		int z2 = getiz() + WALL_MAX_L * 2;
		for (int i = x1; i < x2; i++)
		{
			for (int j = z1; j < z2; j++)
			{
				ct = scene->get_chunk(j,i);
				if(ct->size(SPOT_TYPE_CHARACTER) != 0){
					for(auto it = ct->iter_spots(SPOT_TYPE_CHARACTER); ! ct->iter_ends(SPOT_TYPE_CHARACTER,&it) ; it++){
						c = (character*) *it;
						if(!(c->valid(this))) c->injured(damage);
					}
				}
			}
		}
	}

	return true;
}
