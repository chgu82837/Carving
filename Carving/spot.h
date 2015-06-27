
#define SPOT_TYPE_SIMPLE 0
#define SPOT_TYPE_TRIANGLE 1
#define SPOT_TYPE_CHARACTER 2
#define SPOT_TYPE_CHUNK_BASE 5
#define SPOT_TYPE_WALL 6
#define SPOT_TYPE_HURTABLE 7
#define SPOT_TYPE_PREVIEW -1
#define SPOT_TYPE_CHUNK_BORDER -2

class spot :
	public base
{
public:
	spot(float z,float x,float y);
	spot(float z,float x,float y,int type);
	~spot(void);

	float getz();
	int getiz();
	int getniz();

	float gety();

	float getx();
	int getix();
	int getnix();

	void setzxy(float z,float x,float y);
	void setzxy(float z,float x);

	void setType(int type);
	int getType();
	bool isTmp();

	float distance(spot* s);

	float y;

// protected:

private:
	float z;
	float x;
	int type;

	void init(float z,float x,float y,int type);
};
