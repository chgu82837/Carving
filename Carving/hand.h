
#define HAND_WEAPONTYPE_MON_CREATOR 8
#define HAND_WEAPONTYPE_SPECIAL_ATTACK_TYPE_0 7
#define HAND_WEAPONTYPE_SPECIAL_ATTACK_TYPE_1 6
#define HAND_WEAPONTYPE_WALL_CREATOR 5
#define HAND_WEAPONTYPE_PULLER 4
#define HAND_WEAPONTYPE_PUSHER 3
#define HAND_WEAPONTYPE_STICK 2
#define HAND_WEAPONTYPE_SOLAR 1
#define HAND_WEAPONTYPE_NONE 0

#define HAND_WEAPONTYPE_MAX 8
#define HAND_WEAPONTYPE_LONG_RANGE_BASE 5

class hand :
	public base
{
public:
    hand(int toolType,bool is_sphere,int mtl_i);
	void use(character* user,spot* target,surface* range);
	void use(character* user,float xd,float zd,surface* range);
	void change(int type);
	void change();
    void draw();
	bool long_range();

    looper* sun_r;
    looper* planet_d;
    looper* planet_r;
    looper* sate_d;
    looper* sate_r;

    looper* shoulder;
    looper* elbow;
    looper* wrist;
    looper* thumb;

private:
    void draw_arm(GLfloat d1, GLfloat d2, GLfloat d3, GLfloat thick_d);
	void init(int toolType);

    int toolType;
    int attackState;
    int mtl_i;
    bool is_sphere;
    int cd;
};
