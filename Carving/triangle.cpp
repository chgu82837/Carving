
#include "stdafx.h"

triangle::triangle(spot* spot0,spot* spot1,spot* spot2) :
	spot( (spot0->getz() + spot1->getz() + spot2->getz()) / 3,
	(spot0->getx() + spot1->getx() + spot2->getx()) / 3,
	(spot0->gety() + spot1->gety() + spot2->gety()) / 3,
	SPOT_TYPE_TRIANGLE)
{
	this->spot0 = spot0;
	this->spot1 = spot1;
	this->spot2 = spot2;

	mtl = MTL_cyan;
}

triangle::~triangle(void)
{
}

void triangle::draw()
{
	if((spot0->getz() + spot1->getz() + spot2->getz()) / 3 != getz() ||
		(spot0->getx() + spot1->getx() + spot2->getx()) / 3 != getx() ||
		(spot0->gety() + spot1->gety() + spot2->gety()) / 3 != gety())
		scene->toUpdate = this;

	set_mtl(mtl);
	float u[] = {
		spot1->getx() - spot0->getx(),
		spot1->gety() - spot0->gety(),
		spot1->getz() - spot0->getz()};
	float v[] = {
		spot2->getx() - spot0->getx(),
		spot2->gety() - spot0->gety(),
		spot2->getz() - spot0->getz()
	};
	float n[] = {
		u[1] * v[2] - u[2] * v[1],
		u[2] * v[0] - u[0] * v[2],
		u[0] * v[1] - u[1] * v[0],
	};
	glNormal3fv(n);
	glBegin(GL_TRIANGLES);
	glVertex3f(spot0->getx(),spot0->gety(),spot0->getz());
	glVertex3f(spot1->getx(),spot1->gety(),spot1->getz());
	glVertex3f(spot2->getx(),spot2->gety(),spot2->getz());
	glEnd();
	pop_mtl();
}

spot* triangle::get_spot(int i)
{
	switch(i){
	case 0: return spot0; break;
	case 1: return spot1; break;
	case 2: return spot2; break;
	}
	return NULL;
}

void triangle::update()
{
	setzxy((spot0->getz() + spot1->getz() + spot2->getz()) / 3,
		(spot0->getx() + spot1->getx() + spot2->getx()) / 3,
		(spot0->gety() + spot1->gety() + spot2->gety()) / 3);
}

void triangle::use_mtl(int mtl_i)
{
	this->mtl = mtl_i;
}

void triangle::next_mtl()
{
	mtl ++;
	mtl %= MTL_MAX;
}