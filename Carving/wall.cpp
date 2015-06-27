
#include "stdafx.h"

wall::wall(int mtl_i,float thick) :
	spot(0,0,0,SPOT_TYPE_PREVIEW)
{
	float x1t = float(rand() % 20) * (X_MAX - X_MIN) / 20.0 + X_MIN;
	float z1t = float(rand() % 20) * (Z_MAX - Z_MIN) / 20.0 + Z_MIN;
	float x2t = x1t + float((rand() % (WALL_MAX_L * 2)) - WALL_MAX_L);
	float z2t = z1t + float((rand() % (WALL_MAX_L * 2)) - WALL_MAX_L);
    init(mtl_i,thick,x1t,z1t,x2t,z2t,SPOT_TYPE_WALL);
}

wall::wall(int mtl_i,float thick,float x1,float z1,float x2,float z2) :
	spot(0,0,0,SPOT_TYPE_PREVIEW)
{
    init(mtl_i,thick,x1,z1,x2,z2,SPOT_TYPE_WALL);
}

wall::wall(int mtl_i,float thick,float x1,float z1,float x2,float z2,int type) :
	spot(0,0,0,SPOT_TYPE_PREVIEW)
{
    init(mtl_i,thick,x1,z1,x2,z2,type);
}

wall::~wall()
{
	delete b0;
	delete b1;
	delete b2;
	delete b3;
}

void wall::init(int mtl_i,float thick,float x1,float z1,float x2,float z2,int type){
    this->mtl_i = mtl_i;
	this->thick = thick;
	this->setzxy((z1 + z2) / 2,(x1 + x2) / 2);
	this->setType(type);
	this->r = 90.0 - atan ((z1 - z2) / (x1 - x2)) * 180 / M_PI;
	this->l = sqrt((x1 - x2) * (x1 - x2) + (z1 - z2) * (z1 - z2));

	float mx = (x1 - x2) * (thick + 1.25) / l;
	float mz = (z1 - z2) * (thick + 1.25) / l;
	float nx = (z2 - z1) * (thick + 2) / l;
	float nz = (x1 - x2) * (thick + 2) / l;
	float ax = x1 + mx + nx;
	float az = z1 + mz + nz;
	float bx = x1 + mx - nx;
	float bz = z1 + mz - nz;
	float cx = x2 - mx + nx;
	float cz = z2 - mz + nz;
	float dx = x2 - mx - nx;
	float dz = z2 - mz - nz;

	this->b0 = new border(ax,az,bx,bz,getx(),getz());
	this->b1 = new border(bx,bz,dx,dz,getx(),getz());
	this->b2 = new border(dx,dz,cx,cz,getx(),getz());
	this->b3 = new border(cx,cz,ax,az,getx(),getz());
}

void wall::draw(){
	glPushMatrix();
	glTranslatef(getx(),0.0,getz());
	glRotatef(r,0,1,0);
	glScalef(thick * 2 + 0.5,20.0,l + thick * 2);
	set_mtl(mtl_i);
	draw_obj();
	pop_mtl();
	glPopMatrix();
}

bool wall::valid(float x,float z){
	return b0->valid(x,z) || b1->valid(x,z) || b2->valid(x,z) || b3->valid(x,z);
}

float* wall::closest(float x,float z){
	int i_m = 0;
	float min_d = std::numeric_limits<float>::max();
	float tmp_d;

	tmp_d = b0->distance(x,z); if(min_d > tmp_d){ min_d = tmp_d; i_m = 0; }
	tmp_d = b1->distance(x,z); if(min_d > tmp_d){ min_d = tmp_d; i_m = 1; }
	tmp_d = b2->distance(x,z); if(min_d > tmp_d){ min_d = tmp_d; i_m = 2; }
	tmp_d = b3->distance(x,z); if(min_d > tmp_d){ min_d = tmp_d; i_m = 3; }

	switch(i_m){
		case 0: return b0->closest(x,z); break;
		case 1: return b1->closest(x,z); break;
		case 2: return b2->closest(x,z); break;
		case 3: return b3->closest(x,z); break;
	}
	return b0->closest(x,z);
}
