
#include "stdafx.h"

surface::surface(void)
{
	triangles = new std::set<triangle*>();
	spots = new std::set<spot*>();
}

surface::~surface(void)
{
	delete triangles;
}

void surface::draw(int z1,int z2,int x1,int x2)
{
	float n[] = {0,1,0};
	chunk* ct;
	std::queue<character*> walkers;
	set_mtl(MTL_green);
	for (int i = z1; i <= z2; i++)
	{
		for (int j = x1; j <= x2; j++)
		{
			ct = scene->get_chunk(i,j);
			if(ct->size(SPOT_TYPE_TRIANGLE) == 0){
				glPushMatrix();
				glTranslatef(j + 0.5,0,i + 0.5);

				//glPushMatrix();
				//glRotatef(-90, 1.0, 0.0, 0.0);
				//glRectf(0.1, 0.1, -0.1, -0.1);
				//glPopMatrix();

				glNormal3fv(n);
				glBegin(GL_POLYGON);
				glVertex3f(-0.5,0,-0.5);
				glVertex3f(-0.5,0,0.5);
				glVertex3f(0.5,0,0.5);
				glVertex3f(0.5,0,-0.5);
				glEnd();
				glPopMatrix();
			}
			else{
				for(auto it = ct->iter_spots(SPOT_TYPE_TRIANGLE); ! ct->iter_ends(SPOT_TYPE_TRIANGLE,&it) ; it++){
					((triangle*) *it)->draw();
				}
			}

			if(ct->size(SPOT_TYPE_CHARACTER) != 0){
				chunk::hightlight(j,i,0.2);
				for(auto it = ct->iter_spots(SPOT_TYPE_CHARACTER); ! ct->iter_ends(SPOT_TYPE_CHARACTER,&it) ; it++){
					walkers.push((character*)(*it));
				}
			}

			while(!walkers.empty())
			{
				walkers.front()->walk();
				walkers.front()->draw();
				walkers.pop();
			}

			if(ct->size(SPOT_TYPE_WALL) != 0){
				for(auto it = ct->iter_spots(SPOT_TYPE_WALL); ! ct->iter_ends(SPOT_TYPE_WALL,&it) ; it++){
					((wall*) *it)->draw();
				}
			}
			
			if(ct->size(SPOT_TYPE_HURTABLE) != 0){
				for(auto it = ct->iter_spots(SPOT_TYPE_HURTABLE); ! ct->iter_ends(SPOT_TYPE_HURTABLE,&it) ; it++){
					((hurtable*) *it)->tick();
				}
			}
		}
	}
	pop_mtl();
}

void surface::add(triangle* t)
{
	this->triangles->insert(t);
}

void surface::add(spot* t)
{
	this->spots->insert(t);
}

void surface::pux(spot* target,float z,float x,float y)
{
	int xt = target->getix();
	int zt = target->getiz();
	spot* b;
	triangle* t;

	switch(target->getType())
	{
	case SPOT_TYPE_PREVIEW:
		b = init_chunk(zt,xt,0);

		b->setzxy(z,x,y);

		if(b->getix() != xt || b->getiz() != zt) init_chunk(b->getix(),b->getiz());

		break;
	case SPOT_TYPE_CHUNK_BORDER:
		if(scene->get_chunk(zt-1,xt)->size(SPOT_TYPE_TRIANGLE) == 0) init_chunk(zt-1,xt);
		if(scene->get_chunk(zt,xt-1)->size(SPOT_TYPE_TRIANGLE) == 0) init_chunk(zt,xt-1);
		if(scene->get_chunk(zt-1,xt-1)->size(SPOT_TYPE_TRIANGLE) == 0) init_chunk(zt-1,xt-1);
		b = init_chunk(zt,xt,1);

		b->setzxy(z,x,y);

		break;
	case SPOT_TYPE_CHUNK_BASE:
	case SPOT_TYPE_SIMPLE:
		target->setzxy(z,x,y);
		break;
	case SPOT_TYPE_TRIANGLE:
		t = (triangle*) target;
		b = new spot(z,x,y);
		add(new triangle(b,t->get_spot(0),t->get_spot(1)));
		add(new triangle(b,t->get_spot(1),t->get_spot(2)));
		add(new triangle(b,t->get_spot(2),t->get_spot(0)));
		delete t;
		break;
	}
}

void surface::pull(character* user,spot* target)
{
	pux(target,
		(user->getz() + target->getz() * 4 ) / 5,
		(user->getx() + target->getx() * 4 ) / 5,
		(user->gety() + user->get_height() + target->gety() * 4 ) / 5);
}

void surface::push(character* user,spot* target)
{
	pux(target,
		(-user->getz() + target->getz() * 6 ) / 5,
		(-user->getx() + target->getx() * 6 ) / 5,
		(-user->gety() - user->get_height() + target->gety() * 6 ) / 5);
}

void surface::init_chunk(int z,int x)
{
	init_chunk(z,x,1);
}

spot* surface::init_chunk(int z,int x,int indexToReturn)
{
	if(debug) printf("initualizing chunk (z,x) = (%d,%d)\n",z,x);

#define CHUNK_INIT_INTEVAL 1.00

	chunk* c;
	spot* b1 = new spot(z,x,0,SPOT_TYPE_PREVIEW);
	spot* b2 = new spot(z+1,x,0,SPOT_TYPE_PREVIEW);
	spot* b3 = new spot(z,x+1,0,SPOT_TYPE_PREVIEW);
	spot* b4 = new spot(z+1,x+1,0,SPOT_TYPE_PREVIEW);
	float d1 = CHUNK_INIT_INTEVAL;
	float d2 = CHUNK_INIT_INTEVAL;
	float d3 = CHUNK_INIT_INTEVAL;
	float d4 = CHUNK_INIT_INTEVAL;
	float dt;

#define STEP_1(X) \
	dt = (*k)->distance(b##X); \
	if(dt < d##X){ \
	if(b##X->getType() == SPOT_TYPE_PREVIEW) delete b##X; \
	b##X = (*k); \
	d##X = dt; \
	}

	// Macro STEP_1 Generate the following ...
	// dt = (*k)->distance(b1);
	// if(dt < d1){
	// 	if(b1->getType() == SPOT_TYPE_PREVIEW) delete b1;
	// 	b1 = (*k);
	// 	d1 = dt;
	// }

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			c = scene->get_chunk(z + i,x + j);
			for(auto k = c->iter_spots(SPOT_TYPE_CHUNK_BASE); ! c->iter_ends(SPOT_TYPE_CHUNK_BASE,&k) ; k++){
				STEP_1(1);
				STEP_1(2);
				STEP_1(3);
				STEP_1(4);
			}
		}
	}

	int a = 0;
#define STEP_2(X) \
	if(b##X->getType() == SPOT_TYPE_PREVIEW) { b##X->setType(SPOT_TYPE_CHUNK_BASE); a++; }

	STEP_2(1);
	STEP_2(2);
	STEP_2(3);
	STEP_2(4);

	if(debug) printf("new vertex generated: %d\n",a);

	spot* b = new spot(z+0.5,x+0.5,0);

	add(new triangle(b1,b2,b));
	add(new triangle(b3,b1,b));
	add(new triangle(b4,b3,b));
	add(new triangle(b2,b4,b));

	add(b1);
	add(b2);
	add(b3);
	add(b4);
	add(b);

	switch(indexToReturn)
	{
	case 1: return b1; break;
	case 2: return b2; break;
	case 3: return b3; break;
	case 4: return b4; break;
	}
	return b;
}
