
#ifdef __APPLE__

#include <GL/glew.h>
#include <GLUT/glut.h>
#include <ApplicationServices/ApplicationServices.h>

#elif _WIN32

#pragma once

#include "targetver.h"

#include <tchar.h>
#include "GL/glew.h"
#include "GL/glut.h"
//#include <Windows.h>
#endif

#include <stdio.h>
#include <time.h>
#include <stack>
#include <cstdlib>
#include <limits>
#include <stdlib.h>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <vector>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>

// ============

#define X_MAX 10.0
#define X_MIN -10.0
#define Z_MAX 10.0
#define Z_MIN -10.0
#define WALL_MAX_L 5

class base;
class world;
class chunk;
class spot;
class surface;
class triangle;

class looper;
class hand;
class border;
class wall;
class character;
class hurtable;

#include "world.h"
#include "base.h"

#include "chunk.h"
#include "spot.h"
#include "surface.h"
#include "triangle.h"

#include "helper.h"
#include "looper.h"
#include "hand.h"
#include "border.h"
#include "wall.h"
#include "character.h"
#include "hurtable.h"
