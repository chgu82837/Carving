
class surface :
	public base
{
public:
	surface(void);
	~surface(void);

	void draw(int z1,int z2,int x1,int x2);
	void push(character* user,spot* target);
	void pull(character* user,spot* target);
private:
	
	std::set<triangle*>* triangles;
	std::set<spot*>* spots;
	
	void add(triangle* t);
	void add(spot* s);
	void init_chunk(int z,int x);
	spot* init_chunk(int z,int x,int indexToReturn);
	void pux(spot* target,float z,float x,float y);
};

