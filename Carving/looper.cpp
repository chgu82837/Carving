
#include "stdafx.h"

looper::looper(float lower,float upper,int step,int init,bool behavior){
    this->lower = lower;
    this->upper = upper;
	this->init_inteval = (upper - lower) / step;
    this->inteval = init_inteval;
	this->init = init;
    this->cur = init;
    this->behavior = behavior;
    this->a_loop = false;
}

looper::looper(float lower,float upper,int step,int init,bool behavior,int a_loop){
    this->lower = lower;
    this->upper = upper;
	this->init_inteval = (upper - lower) / step;
    this->inteval = init_inteval;
	this->init = init;
    this->cur = init;
    this->behavior = behavior;
    this->a_loop = a_loop;
}

float looper::next(){
    float r = cur + inteval;
    if(r > upper || r < lower){
        if(behavior){ // LOOPER_TO_LOWER
            r = lower;
        }
        else{ // LOOPER_TRUN_BACK
            inteval = -inteval;
            r = cur + inteval;
        }

		if(a_loop >= LOOPER_AUTO_ENABLE_STOP){
			if(behavior == LOOPER_TRUN_BACK)
				a_loop++;
			else
				a_loop = LOOPER_AUTO_DISABLE;
		}
		if(a_loop == 4) a_loop = LOOPER_AUTO_DISABLE;
    }
    return cur = r;
}

looper::operator float() {
    if(a_loop) return next();
    else return this->cur;
}

void looper::auto_loop(int mode){
    this->cur = init;
    this->inteval = init_inteval;
    this->a_loop = mode;
}

void looper::set_speed(float inteval){
    this->inteval = inteval;
}
