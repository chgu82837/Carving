
#define CHARACTER_TYPE_NONE 10
#define CHARACTER_TYPE_SPHERE 11
#define CHARACTER_TYPE_CUBE 12
#define CHARACTER_TYPE_OBJ1 13
#define CHARACTER_TYPE_OBJ2 14
#define CHARACTER_TYPE_OBJ3 15

class character :
	public spot
{
private:
	int draw_type;
	int r;
	int h;
	int lowered;
	int jumpping;

	int mtl_i;
	int walker_cnt;
	int walker_sta;
	int inj_cd;
	int specialAtk_cd;

	bool light_enabled;
	GLenum light_index;
	int pmodel_list_index;

	bool mounted;
	float y_floor;

public:
	character(int type,float z,float x,float r,int leftHandType,int rightHandType,int mtl_i);
	~character();
	spot* lockTarget();
	void useLeft();
	void useRight();
	void turn(int r,int h);
	void move(float z,float x,int r);
	void mount();
	void ui();
	void jump();
	void walk();
	void draw();
	void injured(int damage);
	bool valid(wall* w);
	int get_r();
	float get_height();

	void setLight(GLenum light_index,GLfloat* ambient,GLfloat* diffuse,GLfloat* specular);
	void enableLight();
	void disableLight();
	void toggleLight();
	void lower(bool enable);
	void update();
	void unmount();

	hand* left;
	hand* right;
	int health;
};
