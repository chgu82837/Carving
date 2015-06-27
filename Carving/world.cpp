
#include "stdafx.h"

void world::init(int size)
{
	this->size = size;
	size *= size;
	chunks = new chunk[size]();
	//printf("%d,%d,%d",chunks[0].size(),chunks[1].size(),chunks[2].size());
	earch = new surface();
	characters = new std::vector<character*>();
	
	set_bg(0,0,0);
	set_view_distance(size / 5,size / 5);

	toUpdate = NULL;
	toRemove = NULL;
}

world::world(void)
{
	init(256);
}

world::world(int size)
{
	init(256);
}

world::~world(void)
{
	delete[] chunks;
	delete earch;
	delete characters;
}

void world::set_sun(int frames_per_cycle,GLenum light_index,GLfloat* ambient,GLfloat* diffuse,GLfloat* specular)
{
	glLightfv(light_index,GL_AMBIENT,ambient);
	glLightfv(light_index,GL_DIFFUSE,diffuse);
	glLightfv(light_index,GL_SPECULAR,specular);
	glEnable(light_index);

	sun = new looper(-90.0, 90.0, frames_per_cycle, 0.0, LOOPER_TRUN_BACK, LOOPER_AUTO_ENABLE);
}

void world::set_bg(float r,float g,float b)
{
	bg = new float[3];
	bg[0] = r;
	bg[1] = g;
	bg[2] = b;
}

void world::set_view_distance(int distance,int foging_range)
{
	this->distance = distance;
	
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, bg);
	glFogf(GL_FOG_END, distance);
	glFogf(GL_FOG_START, distance - foging_range);

}

chunk* world::get_chunk(int z,int x)
{
	return chunks + size * normalize_position(z) + normalize_position(x);
}

chunk* world::get_chunk(float z,float x)
{
	return chunks + size * normalize_position(z) + normalize_position(x);
}

void world::reshape(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(75.0, (GLfloat) w / (GLfloat) h, 0.1, distance);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void world::init_frame()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(bg[0],bg[1],bg[2],1.0);
}

void world::draw(character* user)
{
	GLfloat static light_pos_ori[] = { 0.0, 0.0, 0.0, 1.0 };

	if(toUpdate != NULL){ toUpdate->update(); toUpdate = NULL; }
	if(toRemove != NULL){ delete toRemove; hurtables.erase(toRemove); toRemove = NULL; }

	if(sun != NULL)
	{
		glPushMatrix();
		glRotatef(*sun, 0.0, 0.0, 1.0);
		glTranslatef(0.0,20.0,0.0);
		glLightfv(GL_LIGHT0, GL_POSITION, light_pos_ori);
		set_mtl(MTL_fire);
		glutSolidSphere(0.75, 30, 30);
		pop_mtl();
		glPopMatrix();
	}

#define U(X) ((int) user->get##X())
	int user_r = user->get_r() + 45;
	user_r %= 360;
	int d_t = distance / 2;
	int d_tx = d_t;
	int d_tz = d_t;
	int x_d = 0;
	int z_d = 0;

	if(user_r > 270){
		d_tz *= 2;
		x_d = -d_t;
	}
	else if(user_r > 180){
		d_tx *= 2;
		z_d = d_t;
	}
	else if(user_r > 90){
		d_tz *= 2;
		x_d = d_t;
	}
	else{
		d_tx *= 2;
		z_d = -d_t;
	}

	d_tx += 10;
	d_tz += 10;

	earch->draw(
		U(z) - d_tz + z_d,
		U(z) + d_tz + z_d,
		U(x) - d_tx + x_d,
		U(x) + d_tx + x_d
		);

	spot* tar = user->lockTarget();
	if(tar != NULL){
		set_mtl(MTL_hurtable_fire);
		glPushMatrix();
		glTranslatef(tar->getx(),tar->gety(),tar->getz());
		glutSolidSphere(0.06, 30, 30);
		glPopMatrix();
		pop_mtl();
		if(tar->isTmp()) delete tar;
	}
}

void world::add(hurtable* h)
{
	if(h != NULL)
		this->hurtables.insert(h);
}

void world::add(character* c)
{
	if(c != NULL){
		characters->insert(characters->begin(),c);
	}
}

void world::add(wall* w)
{
	if(w != NULL){
		this->walls.insert(w);
	}
}

int world::get_size()
{
	return size;
}

int world::normalize_position(float i)
{
	return normalize_position((int) floor(i));
}

int world::normalize_position(int i)
{
	while(i < 0 || i >= size){
		if(i < 0) i += size;
		if(i >= size) i -= size;
	}
	return i;
}

//void world::add_triangles(triangle* t)
//{
//	this->earch->add(t);
//}
//
//void world::add_spot(spot* s)
//{
//	this->earch->add(s);
//}
