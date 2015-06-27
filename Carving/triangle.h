
class triangle :
    public spot
{
public:
	triangle(spot* spot0,spot* spot1,spot* spot2);
	~triangle(void);

	void draw();
	spot* get_spot(int i);
	void update();
	void use_mtl(int mtl_i);
	void next_mtl();

private:
	spot* spot0;
	spot* spot1;
	spot* spot2;
	int mtl;
};

