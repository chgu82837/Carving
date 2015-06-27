
class wall :
	public spot{
public:
	wall(int mtl_i,float thick);
	wall(int mtl_i,float thick,float x1,float z1,float x2,float z2);
	wall(int mtl_i,float thick,float x1,float z1,float x2,float z2,int type);
	~wall();

    void draw();
    bool valid(float x,float z);
    float* closest(float x,float z);

private:
    int mtl_i;
    float thick;
    //float x;
    //float z;
    float l;
    float r;
    border* b0;
    border* b1;
    border* b2;
    border* b3;

	void init(int mtl_i,float thick,float x1,float z1,float x2,float z2,int type);
};
