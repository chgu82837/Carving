
#include "stdafx.h"

#define CHARACTER_TYPE_OBJ 13
#define CHARACTER_HEIGHT 1.6
#define CHARACTER_LOWERED_HEIGHT 0.8

bool base::debug;
world* base::scene;
int base::chara_0_obj_li;
int base::chara_1_obj_li;
int base::chara_2_obj_li;

character::character(int type, float z, float x, float r, int leftHandType, int rightHandType, int mtl_i) : spot(z,x,0,SPOT_TYPE_CHARACTER) {
	this->draw_type = type;

	this->r = r;
	this->h = 0;

	this->left = new hand(leftHandType, type == CHARACTER_TYPE_SPHERE, mtl_i);
	this->right = new hand(rightHandType, type == CHARACTER_TYPE_SPHERE, mtl_i);
	this->mtl_i = mtl_i;

	this->walker_cnt = 0;
	this->walker_sta = 0;

	this->jumpping = 0;
	this->health = 100;
	this->inj_cd = 0;

	this->mounted = false;
	this->light_enabled = false;
	this->specialAtk_cd = 0;
	this->lowered = false;
	this->y_floor = 0.0;

	if(type >= CHARACTER_TYPE_OBJ){
		switch(type){
			case CHARACTER_TYPE_OBJ1:
				pmodel_list_index = base::chara_0_obj_li;
				break;
			case CHARACTER_TYPE_OBJ2:
				pmodel_list_index = base::chara_1_obj_li;
				break;
			case CHARACTER_TYPE_OBJ3:
				pmodel_list_index = base::chara_2_obj_li;
				break;
		}
		this->draw_type = CHARACTER_TYPE_OBJ;
	}
}

character::~character()
{
	delete left;
	delete right;
}

spot* character::lockTarget()
{
	int rt = (r + 45) % 360;
	int xi = getix();
	int zi = getiz();

	//printf("xi = %d, zi = %d\n",xi,zi);

	int xid = 0;
	int zid = 0;

	if(rt > 270){
		xi--;
		zi--;
		zid = 1;
	}
	else if(rt > 180){
		zi++;
		xi--;
		xid = 1;
	}
	else if(rt > 90){
		xi++;
		zi--;
		zid = 1;
	}
	else{
		xi--;
		zi--;
		xid = 1;
	}

	float r_tmp = r * M_PI / 180;
	float h_tmp = h * M_PI / 180;
	float x = this->getx();
	float y = this->gety() + this->get_height();
	float z = this->getz();
	float xd = sin(r_tmp) * cos(h_tmp);
	float yd = sin(h_tmp);
	float zd = - cos(r_tmp) * cos(h_tmp);
	float xt,zt,dt,dc,m,n,l;
	spot* tar = NULL;
	chunk* c;

#define FIND_SPOT(X,Y) \
	for(auto it = c->iter_spots(X); ! c->iter_ends(X,&it) ; it++){ \
	m = x - (*it)->getx(); n = y - (*it)->gety() - Y ; l = z - (*it)->getz(); \
	dt = \
	(n * zd - l * yd) * (n * zd - l * yd) + \
	(l * xd - m * zd) * (l * xd - m * zd) + \
	(m * yd - n * xd) * (m * yd - n * xd) ; \
	set_mtl(MTL_hurtable_death); \
	glPushMatrix(); \
	glTranslatef(x - m,y - n,z - l); \
	glutSolidSphere(0.05, 30, 30); \
	glPopMatrix(); \
	pop_mtl(); \
	/* printf("(%f,%f,%f,%d) : dt = %f ,dc = %f\n",x - m,y - n,z - l,(*it)->getType(),dt,dc); */ \
	if(dt < dc){ \
	tar = (*it); \
	dc = dt; \
	} \
	}

	dc = 0.2;

	for (int _cnt = 0; _cnt < 3; _cnt++)
	{
		chunk::hightlight(xi,zi,0.1);

		c = scene->get_chunk(zi,xi);

		//printf("===================\n Searching in (%d,%d) ...\n",xi,zi);

		FIND_SPOT(SPOT_TYPE_CHUNK_BASE, 0);
		FIND_SPOT(SPOT_TYPE_SIMPLE, 0);
		FIND_SPOT(SPOT_TYPE_TRIANGLE, 0);
		FIND_SPOT(SPOT_TYPE_CHARACTER, this->get_height());

		//printf("==================\n");

		xi += xid;
		zi += zid;
	}

	//printf(" >>> \n");

	if(tar == NULL){
		dc = y / yd;
		if(dc > 0) return NULL;
		dc = - dc;
		xt = x + dc * xd;
		zt = z + dc * zd;

		if(xt < xi - 3 * xid || xt > xi + 1 || zt < zi - 3 * zid || zt > zi + 1)
			return NULL;

		m = (xt - floor(xt)) > 0.5 ? ceil(xt) : floor(xt);
		n = (zt - floor(zt)) > 0.5 ? ceil(zt) : floor(zt);
		dt = 
			(xt - m) * (xt - m) +
			(zt - n) * (zt - n) ;

		//printf("x = %f, z = %f , xt = %f, zt = %f, dt = %f\n",x,z,xt,zt,dt);

		if(dt < 0.1)
			tar = new spot(n,m,0,SPOT_TYPE_CHUNK_BORDER);
		else
			tar = new spot(zt,xt,0,SPOT_TYPE_PREVIEW);
	}

	return tar;
}

void character::useLeft()
{
	if(health <= 0) return;
	spot* tar = lockTarget();
	float rt = r * M_PI / 180;
	if(tar == NULL || left->long_range())
		left->use(this,sin(rt),-cos(rt),scene->earch);
	else
		left->use(this,lockTarget(),scene->earch);
}

void character::useRight()
{
	if(health <= 0) return;
	spot* tar = lockTarget();
	float rt = r * M_PI / 180;
	if(tar == NULL || right->long_range())
		right->use(this,sin(rt),-cos(rt),scene->earch);
	else
		right->use(this,lockTarget(),scene->earch);
}

void character::injured(int damage)
{
	if (inj_cd) return;
	if(debug) printf("hit!\n");
	health -= damage;
	inj_cd += 15;
	if(mounted && !health) printf("You died ... \n");
}

void character::jump()
{
	if(health <= 0) return;
	this->jumpping = 1;
}

void character::move(float z, float x, int r)
{
	if(health <= 0) return;
	turn(r,0);
	float user_r_tmp = this->r * M_PI / 180;
	//if(debug) printf("from (z,x) (%f,%f)",this->getz(),this->getx());
	float xt = this->getx() + x * cos(-user_r_tmp) + z * sin(-user_r_tmp);
	float zt = this->getz() + z * cos(user_r_tmp) + x * sin(user_r_tmp);

	y_floor = std::numeric_limits<float>::min();
	chunk* c = scene->get_chunk(getz(),getx());
	for(auto it = c->iter_spots(SPOT_TYPE_TRIANGLE); ! c->iter_ends(SPOT_TYPE_TRIANGLE,&it) ; it++){
		if((*it)->gety() > y_floor) y_floor = (*it)->gety(); 
	}

	chunk* ct;
	wall* w;
	float* corrected_location;
	int x1 = getix() - WALL_MAX_L * 2;
	int z1 = getiz() - WALL_MAX_L * 2;
	int x2 = getix() + WALL_MAX_L * 2;
	int z2 = getiz() + WALL_MAX_L * 2;
	for (int i = x1; i < x2; i++)
	{
		for (int j = z1; j < z2; j++)
		{
			ct = scene->get_chunk(j,i);
			if(ct->size(SPOT_TYPE_WALL) != 0){
				for(auto it = ct->iter_spots(SPOT_TYPE_WALL); ! ct->iter_ends(SPOT_TYPE_WALL,&it) ; it++){
					w = ((wall*) *it);
					if(!w->valid(xt,zt)){
						corrected_location = w->closest(xt,zt);
						xt = corrected_location[0];
						zt = corrected_location[1];
						delete[] corrected_location;
					}
				}
			}
		}
	}

	setzxy(zt,xt);
	if(debug) printf(" to (%f,%f)\n",zt,xt);
}

void character::turn(int r,int h)
{
	this->r += r;
	if(this->r < 0) this->r += 360;
	else this->r %= 360;

	this->h += h;
	if(this->h < -80) this->h = -80;
	else if(this->h > 80) this->h = 80;
}

void character::mount()
{
	float r_tmp = r * M_PI / 180;
	float h_tmp = h * M_PI / 180;
	glLoadIdentity();

	this->update();

	if(health > 0)
		gluLookAt(
		this->getx(),
		this->y + this->get_height(),
		this->getz(),
		this->getx() + sin(r_tmp) * cos(h_tmp),
		this->y + this->get_height() + sin(h_tmp),
		this->getz() - cos(r_tmp) * cos(h_tmp),
		0.0,
		1.0,
		0.0);
	else
		gluLookAt(
		this->getx(),
		this->y + this->get_height(),
		this->getz(),
		this->getx() + sin(r_tmp),
		this->y + this->get_height(),
		this->getz() - cos(r_tmp),
		1.0,
		0.0,
		0.0);

	if(light_enabled)
	{
		glPushMatrix();
		glTranslated(this->getx(), this->y + 1.5, this->getz());
		glRotatef (-r, 0.0, 1.0, 0.0);
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(light_index, GL_POSITION, light_position);
		glPopMatrix();
	}

	mounted = true;
}

void character::walk()
{
	if(mounted) return;
	if(walker_cnt--)
	{
		if(walker_sta == 0) move(0.0, 0.0, -2.5);
		else if(walker_sta == 1) move(0.0, 0.0, 2.5);
		else if(walker_sta <= 5) move(-0.05, 0.0, 0.0);
	}
	else
	{
		walker_sta = rand() % 10;
		walker_cnt = rand() % 120;

		spot* target = lockTarget();
		if(target != NULL)
		{
		    if(rand() % 2) left->use(this,target,scene->earch);
		    else right->use(this,target,scene->earch);
		}
	}
}

void character::update()
{
	if(jumpping){
		this->y += 0.5;
		jumpping++;
		if(jumpping < 15)
			jumpping = 0;
	}
	else{
		this->y -= 0.5;
	}
	if(this->y <= y_floor) this->y = y_floor;
	//printf("y_floor = %f, this->y = %f\n",y_floor,this->y);
}

void character::draw()
{
	if(health <= 0 || draw_type == CHARACTER_TYPE_NONE || mounted) {
		return;
	}

	glPushMatrix();

	this->update();

	glTranslatef(this->getx(), this->y + this->get_height() * 0.75, this->getz());
	glRotatef (-r, 0.0, 1.0, 0.0);

	if(light_enabled)
	{
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
		glLightfv(light_index, GL_POSITION, light_position);
	}

	if(specialAtk_cd){
		set_mtl(MTL_fire);
		specialAtk_cd--;
	}
	else if(inj_cd){
		set_mtl(MTL_blood);
		inj_cd--;
	}
	else
		set_mtl(mtl_i);

	glPushMatrix(); // health bar
	glTranslatef(0.0, 2.0, 0.0);

	set_mtl(MTL_blood);
	glScalef (health / 100.0, 0.1, 0.1);
	glRectf (0.5, 0.5, -0.5, -0.5);
	pop_mtl();
	glPopMatrix();

	if(draw_type == CHARACTER_TYPE_OBJ){
		glPushMatrix(); // GLM
		//glTranslatef(0,1.2,0);
		glScalef(1.5,1.5,1.5);
		glRotatef (180, 0.0, 1.0, 0.0);
		glCallList(pmodel_list_index);
		glPopMatrix();
	}
	else if(draw_type == CHARACTER_TYPE_SPHERE || draw_type == CHARACTER_TYPE_CUBE)
	{
		set_obj_type(draw_type == CHARACTER_TYPE_SPHERE);
		glScalef (0.7, 0.7, 0.7);

		glPushMatrix(); // body
		glScalef (1.6, 3.0, 0.75);
		draw_obj();
		glPopMatrix();

		glPushMatrix(); // head
		glTranslatef(0.0, 2.0, 0.0);
		draw_obj();
		glPopMatrix();

		glPushMatrix(); // right foot
		glTranslatef(0.8, -1.5, 0.0);
		draw_obj();
		glPopMatrix();

		glPushMatrix(); // left foot
		glTranslatef(-0.8, -1.5, 0.0);
		draw_obj();
		glPopMatrix();

		glPushMatrix(); // right hand
		glTranslatef(0.8, 1.0, 0.0);
		glScalef (0.4, 0.4, 0.4);
		glRotatef (75, 0.0, 0.0, 1.0);
		glRotatef (90, 0.0, 1.0, 0.0);
		right->draw();
		glPopMatrix();

		glPushMatrix(); // left hand
		glTranslatef(-0.8, 1.0, 0.0);
		glScalef (-0.4, 0.4, 0.4);
		glRotatef (75, 0.0, 0.0, 1.0);
		glRotatef (90, 0.0, 1.0, 0.0);
		left->draw();
		glPopMatrix();

		pop_obj_type();
	}

	pop_mtl();
	glPopMatrix();
}

void character::ui()
{
	glClear (GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslated(this->getx(), this->y + get_height() / 2, this->getz());
	glRotatef (-r, 0.0, 1.0, 0.0);
	glTranslated(0,0,1);

	glPushMatrix();
	glTranslated(0.0, -0.75, -1.5);
	glScalef (health / 100.0, 0.1, 0.1);
	set_mtl(MTL_blood);
	glRectf(0.5, 0.5, -0.5, -0.5);
	pop_mtl();
	glPopMatrix();

	if(health > 0){
		if(specialAtk_cd){
			set_mtl(MTL_fire);
			specialAtk_cd--;
		}
		else if(!inj_cd)
			set_mtl(mtl_i);
		else{
			set_mtl(MTL_blood);
			inj_cd--;
		}
		set_obj_type(draw_type == CHARACTER_TYPE_SPHERE);

		glTranslatef(0.0, -1.75, -1.0);

		glPushMatrix(); // right hand
		glTranslatef(0.8, 1.0, 0.0);

		glScalef (0.4, 0.4, 0.4);
		glRotatef (75, 0.0, 0.0, 1.0);
		glRotatef (90, 0.0, 1.0, 0.0);
		right->draw();
		glPopMatrix();

		glPushMatrix(); // left hand
		glTranslatef(-0.8, 1.0, 0.0);
		glScalef (-0.4, 0.4, 0.4);
		glRotatef (75, 0.0, 0.0, 1.0);
		glRotatef (90, 0.0, 1.0, 0.0);
		left->draw();
		glPopMatrix();

		pop_obj_type();
		pop_mtl();
	}
	else{
		glDisable(GL_LIGHTING);
		glPushMatrix();
		glColor4f(1.0, 0.0, 0.0, 0.2);
		glTranslated(0.0, -1.75, -1.5);
		glRectf(10.0,10.0,-10.0,-10.0);
		glPopMatrix();
		glEnable(GL_LIGHTING);
	}

	glPopMatrix();
}

bool character::valid(wall* w){
	return w->valid(this->getx(),this->getz());
}

void character::setLight(GLenum light_index,GLfloat* ambient,GLfloat* diffuse,GLfloat* specular)
{
	glLightfv(light_index,GL_AMBIENT,ambient);
	glLightfv(light_index,GL_DIFFUSE,diffuse);
	glLightfv(light_index,GL_SPECULAR,specular);

	this->light_index = light_index;
}

void character::disableLight()
{
	this->light_enabled = false;
	glDisable(light_index);
}

void character::enableLight()
{
	this->light_enabled = true;
	glEnable(light_index);
}

void character::toggleLight()
{
	if(light_enabled) disableLight();
	else enableLight();
}

int character::get_r()
{
	return r;
}

float character::get_height()
{
	return lowered ? CHARACTER_LOWERED_HEIGHT : CHARACTER_HEIGHT;
}

void character::lower(bool enable)
{
	this->lowered = enable;
}

void character::unmount()
{
	this->mounted = false;
}
