#include "stdafx.h"

base::base(void)
{
}


base::~base(void)
{
}

void base::set_scene(world* sc)
{
	base::scene = sc;
}

void base::debug_mode(bool toEnable)
{
    base::debug = toEnable;
}
