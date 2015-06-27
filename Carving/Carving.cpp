/***************************
4101056017 第二組 邱冠喻 期末專題 Carving 6/26
4101056017 Team_2 Chiu Guan-Yu FinalProject Carving 6/26
***************************/

#include "stdafx.h"
#include "glm.c"

int loadMdl(GLMmodel* pmodel);

#define THE_BG 0.22,0.54,0.75

GLfloat light_pos_ori[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_Ka[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_Kd[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_Ks[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat lightHead_Ka[] = { 0.7, 0.2, 0.2, 1.0 };
GLfloat lightHead_Kd[] = { 1.0, 1.0, 0.0, 1.0 };
GLfloat lightHead_Ks[] = { 1.0, 1.0, 0.45, 1.0 };

GLfloat lmodel_Ka[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat local_viewer[] = { 0.0 };

// ========================================

bool muted = true;
bool fps_mode = false;
bool keyStates[256] = {0}; // 0 for mouse left, 1 for mouse right, 2 for mouse middle

character* user;
world* scene;
int screen_w,screen_h; // WARN: THIS IT IN HALF
int mouse_x,mouse_y;
int fps_mode_cd = 20;

void display(void)
{
	static int t;
	static int fps;
	if(t != time(NULL))
	{
		if(!muted) printf("displaying at FPS: %d\n", fps);
		fps = 1;
		t = time(NULL);
	}
	else
		fps++;

	if(fps_mode_cd) fps_mode_cd--;
	if(fps_mode){
		user->turn(((float) mouse_x - screen_w)/10.0,((float) screen_h - mouse_y)/10.0);
#ifdef __APPLE__
		CGWarpMouseCursorPosition(CGPointMake( screen_w + glutGet(GLUT_WINDOW_X), screen_h + glutGet(GLUT_WINDOW_Y)));
#else
		glutWarpPointer(screen_w, screen_h);
#endif
	}

	if(keyStates['d'])  user->move(0.0, 0.05, 0.0);
	if(keyStates['a']) user->move(0.0, -0.05, 0.0);
	if(keyStates['w']) user->move(-0.05, 0.0, 0.0);
	if(keyStates['s'])  user->move(0.05, 0.0, 0.0);
	if(keyStates[' ']) user->jump();
	user->lower(keyStates['c']);

	scene->init_frame();
	user->mount();

	scene->draw(user);

	user->ui();

	glutSwapBuffers();
}

void reshape (int w, int h)
{
	scene->reshape(w,h);
	screen_w = w / 2;
	screen_h = h / 2;
}

int last_detec = 0;
bool polyoffEnabled = false;
bool fogEnabled = false;

void keyboard (unsigned char key, int x, int y)
{
	keyStates[key] = true;

	switch(key)
	{
	case 27:
		if(fps_mode){
			fps_mode = false;
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			printf("Use `ESC` key again to exit the game\n");
		}
		else{
			exit(0);
		}
		break;
	case 'l':
		user->toggleLight();
		break;
	case 'z':
		user->left->change();
		break;
	case 'x':
		user->right->change();
		break;
	case 'p':
		if(!polyoffEnabled){
			glEnable(GL_POLYGON_OFFSET_LINE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDisable(GL_POLYGON_OFFSET_FILL);
		}
		polyoffEnabled = !polyoffEnabled;
		break;
	case 'k':
		user->health = 0.0;
		break;
	case 'm':
		if(muted) printf("muted disabled...");
		else printf("muted...");
		muted = !muted;
		base::debug_mode(!muted);
		break;
	}
}

void keyup(unsigned char key, int x, int y)
{
	keyStates[key] = false;
}

void mouse(int button, int state, int x, int y)
{
	if(fps_mode_cd) return;
	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		if(!muted) printf("Left key\n");
		keyStates[0] = state == GLUT_DOWN;

		if(fps_mode) user->useLeft();
		break;
	case GLUT_RIGHT_BUTTON:
		if(!muted) printf("Right key\n");
		keyStates[1] = state == GLUT_DOWN;

		if(fps_mode)  user->useRight();
		break;
	case GLUT_MIDDLE_BUTTON:
		if(!muted) printf("Middle key\n");
		keyStates[2] = state == GLUT_DOWN;
		break;
	}

	if(!fps_mode)
	{
		glutWarpPointer(screen_w, screen_h);
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		mouse_x = screen_w;
		mouse_y = screen_h;
		printf("Use `ESC` key to exit FPS mode\n");
		fps_mode = true;
		fps_mode_cd = 20;
	}
}

void mouse_move(int x, int y)
{
	if(mouse_x == x && mouse_y == y) return;
	mouse_x = x;
	mouse_y = y;
	// printf("mouse moving: %d %d, screen: %d %d\n",x,y,screen_w,screen_h);
}

int main(int argc, char **argv)
{

	char* chara_0_obj_path = "./data/Hulk.obj";
	char* chara_1_obj_path = "./data/Luigi_obj.obj";
	char* chara_2_obj_path = "./data/mario_obj.obj";
	switch(argc){
	case 4:
		chara_2_obj_path = argv[3];
	case 3:
		chara_1_obj_path = argv[2];
	case 2:
		chara_0_obj_path = argv[1];
		break;
	}

	printf("Loading model 0 from [%s] ... \n",chara_0_obj_path);
	GLMmodel* mdl0 = glmReadOBJ(chara_0_obj_path);

	printf("Loading model 1 from [%s] ... \n",chara_1_obj_path);
	GLMmodel* mdl1 = glmReadOBJ(chara_1_obj_path);

	printf("Loading model 2 from [%s] ... \n",chara_2_obj_path);
	GLMmodel* mdl2 = glmReadOBJ(chara_2_obj_path);
	
	printf("\nLoading Complete\n\n");

	// Manual
	printf("Carving\n========================\nA small 3D First Person Shooter (FPS) game that the terrain cam be modified by pulling and pushing the vertices and triangles\n\nmovement controls:\nkeyboard `d` key to walk forward\nkeyboard `s` key to walk backward\nkeyboard `a` key to walk left\nkeyboard `d` key to walk right\nkeyboard ` ` (space) key to fly (or jump)\nkeyboard `c` key to lower the body\n\nclick on the game window to start First Person Shooter (FPS) mode, then\n1. Use mouse move to move the FOV (or the head)\n2. Use `ESC` key to exit FPS mode\n\ntool controls:\nkeyboard `z` key to change left hand tool*\nkeyboard `x` key to change right hand tool*\nkeyboard `l` key to toggle head light\nmouse left key to use left hand tool\nmouse right key to use right hand tool\n\nother controls:\nkeyboard `p` key to toggle polygon offset mode\nkeyboard 'k' key to suicide\nkeyboard 'm' key to toggle frames per second (FPS) and debug message\nkeyboard `ESC` key to exit First Person Shooter (FPS) or exit the game\n\nhand tools:\n1. Unarmed: attack a monster in front of you with 10 damage\n2. Painter: a solar system that change the color (material) of the locked triangle\n3. Bat: attack a monster in front of you with 20 damage\n4. Surface Pusher: push a vertice or triangle in front of you\n5. Surface Puller: pull a vertice or triangle in front of you\n6. Wall Builder: build a wall ahead of you\n7. Spell - Rectangle Bomb: put a Rectangle Bomb that cause 20 damage ahead of you\n8. Spell - Circle Bomb: put a Circle Bomb that cause 30 damage ahead of you\n9. Monster Creator: put a monster ahead of you");
	printf("\n>>> Please read the manual above and press Enter to Start the game!");
	getchar();

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500, 500);
	// glutInitWindowPosition (100, 100);
	glutCreateWindow ("Carving");
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_Ka);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_viewer);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glPolygonOffset(0.8, 1);

	srand (time(NULL));

#ifdef __APPLE__
	CGSetLocalEventsSuppressionInterval(0.0);
#endif

	scene = new world(256);
	base::set_scene(scene);
	// base::debug_mode(true);
	base::chara_0_obj_li = loadMdl(mdl0);
	base::chara_1_obj_li = loadMdl(mdl1);
	base::chara_2_obj_li = loadMdl(mdl2);

	user = new character(CHARACTER_TYPE_CUBE, 3.0, 0, 0, HAND_WEAPONTYPE_PULLER, HAND_WEAPONTYPE_MON_CREATOR,MTL_orange);

	user->setLight(GL_LIGHT1,lightHead_Ka,lightHead_Kd,lightHead_Ks);
	scene->set_sun(2500,GL_LIGHT0,light_Ka,light_Kd,light_Ks);
	scene->set_bg(THE_BG);
	scene->set_view_distance(20,5);
	scene->add(user);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyup);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouse_move);
	glutIdleFunc((void (*)()) glutPostRedisplay);
	
	glutMainLoop();
	return 0;
}

int loadMdl(GLMmodel* pmodel)
{
    if (!pmodel) {
        throw "WARN: pmodel invalid!\n";
    }
    else{
        glmUnitize(pmodel);
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 90.0);

        return glmList(pmodel, GLM_SMOOTH | GLM_TEXTURE);
    }
}

/***************************
4101056017 第二組 邱冠喻 期末專題 Carving 6/26
4101056017 Team_2 Chiu Guan-Yu FinalProject Carving 6/26
***************************/
