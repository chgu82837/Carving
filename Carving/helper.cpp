
#include "stdafx.h"

// Obj control =========================

bool obj_using_sphere = false;
std::stack<bool> obj_statk;

void pop_obj_type()
{
    if(!obj_statk.size()) return;
    obj_using_sphere = obj_statk.top();
    obj_statk.pop();
}

void set_obj_type(bool is_sphere)
{
    obj_statk.push(obj_using_sphere);
    obj_using_sphere = is_sphere;
}

void draw_obj()
{
    if(obj_using_sphere) glutSolidSphere(0.75, 30, 30);
    else glutSolidCube (1.0);
}

// Light Material control ==============

const float mtl_v[] =
{
    /*
    ** [index] color_name: **
    Ka0, Ka1, Ka2, Ka3,
    Kd0, Kd1, Kd2, Kd3,
    Ks0, Ks1, Ks2, Ks3,
    Ke0, Ke1, Ke2, Ke3,
    Se,
    */

    /* [0] gray: */
    0.11, 0.06, 0.11, 1.00,
    0.43, 0.47, 0.54, 1.00,
    0.33, 0.33, 0.52, 1.00,
    0.00, 0.00, 0.00, 0.00,
    10,

    /* [1] red: */
    0.11, 0.06, 0.11, 1.00,
    0.86, 0.27, 0.31, 1.00,
    1.00, 0.14, 0.12, 1.00,
    0.00, 0.00, 0.00, 0.00,
    10,

    /* [2] lavender: */
    0.11, 0.06, 0.11, 1.00,
    0.83, 0.31, 0.98, 1.00,
    0.68, 0.16, 0.78, 1.00,
    0.00, 0.00, 0.00, 0.00,
    10,

    /* [3] blue: */
    0.11, 0.06, 0.11, 1.00,
    0.20, 0.22, 1.00, 1.00,
    0.34, 0.34, 0.94, 1.00,
    0.00, 0.00, 0.00, 0.00,
    10,

    /* [4] yellow: */
    0.11, 0.06, 0.11, 1.00,
    0.85, 0.85, 0.25, 1.00,
    0.72, 0.71, 0.28, 1.00,
    0.00, 0.00, 0.00, 0.00,
    10,

    /* [5] orange: */
    0.11, 0.06, 0.11, 1.00,
    1.00, 0.66, 0.25, 1.00,
    1.00, 0.04, 0.07, 1.00,
    0.00, 0.00, 0.00, 0.00,
    10,

    /* [6] green: */
    0.11, 0.06, 0.11, 1.00,
    0.22, 1.00, 0.18, 1.00,
    0.62, 1.00, 0.63, 1.00,
    0.00, 0.00, 0.00, 0.00,
    10,

    /* [7] cyan: */
    0.11, 0.06, 0.11, 1.00,
    0.15, 0.93, 1.00, 1.00,
    0.21, 1.00, 0.98, 1.00,
    0.00, 0.00, 0.00, 0.00,
    10,

    /* [8] purple: */
    0.11, 0.06, 0.11, 1.00,
    0.70, 0.10, 1.00, 1.00,
    0.44, 0.02, 0.76, 1.00,
    0.00, 0.00, 0.00, 0.00,
    10,

    /* [9] sun: */
    0.00, 0.00, 0.00, 1.00,
    0.20, 0.47, 0.54, 1.00,
    0.23, 0.33, 0.52, 1.00,
    0.74, 0.15, 0.16, 0.00,
    10,

    /* [10] blood: */
    0.00, 0.00, 0.00, 1.00,
    0.00, 0.00, 0.00, 1.00,
    0.00, 0.00, 0.00, 1.00,
    0.81, 0.27, 0.37, 0.00,
    10,

    /* [11] fire: */
    0.61, 0.50, 0.42, 1.00,
    0.32, 0.36, 0.43, 1.00,
    0.18, 0.17, 0.21, 1.00,
    0.84, 0.60, 0.00, 0.00,
    10,

    /* [12] wall_gray: */
    0.11, 0.06, 0.11, 0.30,
    0.43, 0.47, 0.54, 0.60,
    0.33, 0.33, 0.52, 0.40,
    0.18, 0.00, 0.00, 0.00,
    10,

    /* [13] wall_red: */
    0.11, 0.06, 0.11, 0.30,
    0.86, 0.27, 0.31, 0.40,
    1.00, 0.14, 0.12, 0.60,
    0.08, 0.00, 0.00, 0.00,
    10,

    /* [14] wall_lavender: */
    0.11, 0.06, 0.11, 0.70,
    0.83, 0.31, 0.98, 0.50,
    0.68, 0.16, 0.78, 0.40,
    0.08, 0.00, 0.00, 0.00,
    10,

    /* [15] wall_blue: */
    0.11, 0.06, 0.11, 0.40,
    0.20, 0.22, 1.00, 0.60,
    0.34, 0.34, 0.94, 0.40,
    0.08, 0.00, 0.00, 0.00,
    10,

    /* [16] wall_yellow: */
    0.11, 0.06, 0.11, 0.50,
    0.85, 0.85, 0.25, 0.50,
    0.72, 0.71, 0.28, 0.50,
    0.08, 0.00, 0.00, 0.00,
    10,
    
	/* [17] hurtable_fire: */
    0.61, 0.50, 0.42, 0.40,
    0.32, 0.36, 0.43, 0.40,
    0.18, 0.17, 0.21, 0.40,
    0.08, 0.06, 0.00, 0.00,
    10,
    
	/* [18] hurtable_death: */
    0.11, 0.06, 0.11, 0.40,
    0.43, 0.47, 0.54, 0.40,
    0.33, 0.33, 0.52, 0.50,
    0.00, 0.00, 0.00, 0.00,
    10,
};

std::stack<int> mtl_statk;
int mtl_state = 0;

void _set_mtl()
{
    int mtl_index_base = mtl_state * 17;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mtl_v + mtl_index_base); mtl_index_base+= 4;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl_v + mtl_index_base); mtl_index_base+= 4;
    glMaterialfv(GL_FRONT, GL_SPECULAR, mtl_v + mtl_index_base); mtl_index_base+= 4;
    glMaterialfv(GL_FRONT, GL_EMISSION, mtl_v + mtl_index_base); mtl_index_base+= 4;
    glMaterialfv(GL_FRONT, GL_SHININESS, mtl_v + mtl_index_base);
}

void pop_mtl()
{
	if(!mtl_statk.size()) return;
	mtl_state = mtl_statk.top();
	mtl_statk.pop();
	_set_mtl();
}

void set_mtl(int mtl_i)
{
	mtl_statk.push(mtl_state);
	mtl_state = mtl_i;
	_set_mtl();
}
