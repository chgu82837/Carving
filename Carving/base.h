
class base
{
public:
	base(void);
	~base(void);

	void static set_scene(world* sc);
    void static debug_mode(bool toEnable);
	static int chara_0_obj_li;
	static int chara_1_obj_li;
	static int chara_2_obj_li;
protected:
	static world* scene;
    static bool debug;
};

