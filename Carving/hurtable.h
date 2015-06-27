
#define HURTABLE_TYPE_NONE 0
#define HURTABLE_TYPE_SPHERE 1
#define HURTABLE_TYPE_CUBE 2

class hurtable :
	public wall
{
public:
    hurtable(int type,int mtl_i,float thick,float x1,float z1,float x2,float z2,int cnt,int damage);
    ~hurtable();
    bool tick(); // return false if the event is finished
private:
    int cd;
    int cd_init;
    int type;
    int damage;
};
