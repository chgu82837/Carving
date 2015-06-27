
#define LOOPER_TO_LOWER true
#define LOOPER_TRUN_BACK false
#define LOOPER_AUTO_ENABLE 1
#define LOOPER_AUTO_ENABLE_STOP 2
#define LOOPER_AUTO_DISABLE 0

class looper{
private:
    float lower;
    float upper;
    float inteval;
	float init_inteval;
	float init;
    float cur;
    bool behavior;
    int a_loop;

public:
    looper(float lower,float upper,int step,int init,bool behavior);
    looper(float lower,float upper,int step,int init,bool behavior,int a_loop);
    float next();
    operator float();
    void auto_loop(int mode);
    void set_speed(float inteval);
};
