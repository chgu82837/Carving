
#include "stdafx.h"

void hand::init(int toolType)
{
	this->toolType = toolType;

	this->shoulder = new looper(-35, 35, 10, -35, LOOPER_TRUN_BACK);
	this->elbow = new looper(-65, 0, 10, -65, LOOPER_TRUN_BACK);
	this->wrist = new looper(20, 50, 10, 20, LOOPER_TRUN_BACK);

	if(toolType == HAND_WEAPONTYPE_STICK)
		this->thumb = new looper(0, 50, 30, 50, LOOPER_TRUN_BACK);
	else
		this->thumb = new looper(0, 50, 30, 10, LOOPER_TRUN_BACK);

	this->attackState = 0;

	this->sun_r = new looper(0, 360, 60, 0, LOOPER_TO_LOWER,LOOPER_AUTO_ENABLE);
	this->planet_r = new looper(0, 360, 60, 0, LOOPER_TO_LOWER,LOOPER_AUTO_ENABLE);
	this->planet_d = new looper(0, 360, 60, 0, LOOPER_TO_LOWER,LOOPER_AUTO_ENABLE);
	this->sate_r = new looper(0, 360, 60, 0, LOOPER_TO_LOWER,LOOPER_AUTO_ENABLE);
	this->sate_d = new looper(0, 360, 60, 0, LOOPER_TO_LOWER,LOOPER_AUTO_ENABLE);
}

hand::hand(int toolType,bool is_sphere,int mtl_i){
	this->is_sphere = is_sphere;
	this->mtl_i = mtl_i;

	init(toolType);

	this->cd = 0;
}

void hand::use(character* user,spot* target,surface* range)
{
	if(cd || target == NULL) return;
	this->shoulder->auto_loop(LOOPER_AUTO_ENABLE_STOP);
	this->elbow->auto_loop(LOOPER_AUTO_ENABLE_STOP);
	this->wrist->auto_loop(LOOPER_AUTO_ENABLE_STOP);

	switch(toolType)
	{
	case HAND_WEAPONTYPE_PUSHER:
		range->push(user,target);
		cd += 10;
		break;
	case HAND_WEAPONTYPE_PULLER:
		range->pull(user,target);
		cd += 10;
		break;
	case HAND_WEAPONTYPE_SOLAR:
		if(target->getType() == SPOT_TYPE_TRIANGLE)
			((triangle*) target)->next_mtl();
		cd += 10;
		break;
	case HAND_WEAPONTYPE_STICK:
		if(target->getType() == SPOT_TYPE_CHARACTER)
			((character*) target)->injured(20);
		cd += 15;
	default:
		if(target->getType() == SPOT_TYPE_CHARACTER)
			((character*) target)->injured(10);
		cd += 20;
		break;
	}
}

void hand::use(character* user,float xd,float zd,surface* range)
{
	if(cd) return;
	float ux = user->getx();
	float uz = user->getz();
	int mtl;
	switch(toolType)
	{
	case HAND_WEAPONTYPE_WALL_CREATOR:
		scene->add(new wall(rand() % MTL_WALL_L + MTL_WALL_BASE,0.5,
			ux + xd * 3 + zd,
			uz + zd * 3 - xd ,
			ux + xd * 3 - zd,
			uz + zd * 3 + xd));
		cd += 100;
		break;
	case HAND_WEAPONTYPE_SPECIAL_ATTACK_TYPE_0:
		scene->add(new hurtable(
			HURTABLE_TYPE_CUBE,
			MTL_hurtable_fire,
			1,
			ux + xd * 5,
			uz + zd * 5,
			ux + xd * 9,
			uz + zd * 9,
			60,
			20
			));
		cd += 50;
		break;
	case HAND_WEAPONTYPE_SPECIAL_ATTACK_TYPE_1:
		scene->add( new hurtable(
			HURTABLE_TYPE_SPHERE,
			MTL_hurtable_death,
			1,
			ux + xd * 9,
			uz + zd * 9,
			ux + xd * 10,
			uz + zd * 10,
			120,
			30
			));
		cd += 50;
		break;
	case HAND_WEAPONTYPE_MON_CREATOR:
		mtl = rand() % MTL_NORMAL_MAX;
		switch(rand() % 5){
		case 0:
			scene->add(new character(CHARACTER_TYPE_OBJ1, uz + zd * 5, ux + xd * 5, 180, HAND_WEAPONTYPE_NONE, HAND_WEAPONTYPE_STICK, mtl));
			break;
		case 1:
			scene->add(new character(CHARACTER_TYPE_OBJ2, uz + zd * 5, ux + xd * 5, 180, HAND_WEAPONTYPE_NONE, HAND_WEAPONTYPE_STICK, mtl));
			break;
		case 2:
			scene->add(new character(CHARACTER_TYPE_OBJ3, uz + zd * 5, ux + xd * 5, 180, HAND_WEAPONTYPE_NONE, HAND_WEAPONTYPE_STICK, mtl));
			break;
		case 3:
			scene->add(new character(CHARACTER_TYPE_CUBE, uz + zd * 5, ux + xd * 5, 180, HAND_WEAPONTYPE_NONE, HAND_WEAPONTYPE_STICK, mtl));
			break;
		case 4:
			scene->add(new character(CHARACTER_TYPE_SPHERE, uz + zd * 5, ux + xd * 5, 180, HAND_WEAPONTYPE_NONE, HAND_WEAPONTYPE_STICK, mtl));
			break;
		}
		cd += 100;
		break;
	}

}


void hand::draw(){
	glPushMatrix();

	set_obj_type(is_sphere);

	if(cd){
		cd--;
		set_mtl(MTL_fire);
	}
	else
		set_mtl(mtl_i);

	draw_arm(*shoulder, -*elbow, *wrist, 0.25);

	for (int i = 0; i < 5; i++)
	{
		glPushMatrix();
		glRotatef(-30 + i * 15, 0.0, 1.0, 0.0);
		glTranslatef(0.5, 0.0, 0.0);
		glScalef(0.2, 0.3, 0.2);
		draw_arm(*thumb, *thumb + 20.0, *thumb + 10.0, 0.0);
		glPopMatrix();
	}

	switch(toolType)
	{
	case HAND_WEAPONTYPE_STICK:
		glTranslatef(0.0, 0.4, 1.5);
		glScalef(0.5, 0.5, 3.0);
		set_mtl(MTL_red);
		glutSolidCube (1.0);
		pop_mtl();
		break;
	case HAND_WEAPONTYPE_SPECIAL_ATTACK_TYPE_0:
	case HAND_WEAPONTYPE_SPECIAL_ATTACK_TYPE_1:
	case HAND_WEAPONTYPE_MON_CREATOR:
	case HAND_WEAPONTYPE_SOLAR:

		glTranslatef(0.0, 1.0, 0.0);
		glRotatef (90.0, 1.0, 0.0, 0.0);

		switch(toolType){
		case HAND_WEAPONTYPE_SPECIAL_ATTACK_TYPE_0:
			set_mtl(MTL_hurtable_fire);
			break;
		case HAND_WEAPONTYPE_SPECIAL_ATTACK_TYPE_1:
			set_mtl(MTL_hurtable_death);
			break;
		case HAND_WEAPONTYPE_MON_CREATOR:
			set_mtl(MTL_green);
			break;
		default:
			set_mtl(MTL_sun);
			break;
		}

		glPushMatrix();
		glRotatef(*sun_r, 0.0, 0.0, 1.0);
		glScalef(0.5,0.5,0.5);
		glutSolidSphere(0.75, 30, 30);
		glPopMatrix();

		pop_mtl();
		set_mtl(MTL_green);

		glRotatef(*planet_d, 0.0, 0.0, 1.0);
		glTranslatef(1.0 * (50 - *thumb) / 50, 0.0, 0.0);

		glPushMatrix();
		glRotatef(*planet_r, 0.0, 0.0, 1.0);
		glScalef(0.2,0.2,0.2);
		glutSolidSphere(0.75, 30, 30);
		glPopMatrix();

		pop_mtl();
		set_mtl(MTL_cyan);

		glRotatef(*sate_d, 0.0, 0.0, 1.0);
		glTranslatef(0.5 * (50 - *thumb) / 50, 0.0, 0.0);

		glPushMatrix();
		glRotatef(*sate_r, 0.0, 0.0, 1.0);
		glScalef(0.1,0.1,0.1);
		glutSolidSphere(0.75, 30, 30);
		glPopMatrix();

		pop_mtl();

		break;
	case HAND_WEAPONTYPE_PULLER:
		glTranslatef(0.0, 0.4, 1.5);
		glScalef(0.5, 0.5, 2.0);
		set_mtl(MTL_green);
		glutSolidCube (1.0);
		pop_mtl();
		break;
	case HAND_WEAPONTYPE_PUSHER:
		glTranslatef(0.0, 0.4, 1.5);
		glScalef(0.5, 0.5, 2.0);
		set_mtl(MTL_fire);
		glutSolidCube (1.0);
		pop_mtl();
		break;
	case HAND_WEAPONTYPE_WALL_CREATOR:
		glTranslatef(0.0, 0.4, 1.5);
		glScalef(0.5, 0.5, 2.0);
		set_mtl(MTL_lavender);
		glutSolidCube (1.0);
		pop_mtl();
		break;
	case HAND_WEAPONTYPE_NONE:
		break;
	}

	pop_mtl();
	pop_obj_type();

	glPopMatrix();
}

void hand::draw_arm(GLfloat d1, GLfloat d2, GLfloat d3, GLfloat thick_d)
{
	GLfloat thick = 1;

	glRotatef (d1, 0.0, 0.0, 1.0);
	glTranslatef (1.0, 0.0, 0.0);

	glPushMatrix();
	glScalef (2.0, thick -= thick_d, 1.0);
	draw_obj();
	glPopMatrix();

	glTranslatef (1.0, 0.0, 0.0);
	glRotatef (d2, 0.0, 0.0, 1.0);
	glTranslatef (1.0, 0.0, 0.0);

	glPushMatrix();
	glScalef (2.0, thick -= thick_d, 1.0);
	draw_obj();
	glPopMatrix();

	glTranslatef (1.0, 0.0, 0.0);
	glRotatef (d3, 0.0, 0.0, 1.0);
	glTranslatef (0.5, 0.0, 0.0);

	glPushMatrix();
	glScalef (1.0, thick -= thick_d, 1.0);
	draw_obj();
	glPopMatrix();
}

void hand::change()
{
	change(toolType + 1);
}

void hand::change(int type)
{
	if(cd) return;
	type %= HAND_WEAPONTYPE_MAX + 1;

#define C(X,Y) case X: printf("Tool changed : %s\n",Y); break;
	switch(type){
		C(HAND_WEAPONTYPE_NONE,"Unarmed");
		C(HAND_WEAPONTYPE_SOLAR,"Painter");
		C(HAND_WEAPONTYPE_STICK,"Bat");
		C(HAND_WEAPONTYPE_PUSHER,"Surface Pusher");
		C(HAND_WEAPONTYPE_PULLER,"Surface Puller");
		C(HAND_WEAPONTYPE_WALL_CREATOR,"Wall Builder");
		C(HAND_WEAPONTYPE_SPECIAL_ATTACK_TYPE_0,"Spell - Rectangle Bomb");
		C(HAND_WEAPONTYPE_SPECIAL_ATTACK_TYPE_1,"Spell - Circle Bomb");
		C(HAND_WEAPONTYPE_MON_CREATOR,"Monster Creator");
	}
	if(debug) printf("Hand weapon changed to %d\n",type);
	init(type);
	cd += 10;
}

bool hand::long_range()
{
	return this->toolType >= HAND_WEAPONTYPE_LONG_RANGE_BASE;
}
