
class world
{
public:
	world(void);
	world(int size);
	// world(std::string obj_file);
	~world(void);

	void set_sun(int frames_per_cycle,GLenum light_index,GLfloat* ambient,GLfloat* diffuse,GLfloat* specular);
	void set_bg(float r,float g,float b);
	void set_view_distance(int distance,int foging_range);
	
	void add(wall* c);
	void add(hurtable* h);
	void add(character* c);

	int get_size();
	chunk* get_chunk(int z,int x);
	chunk* get_chunk(float z,float x);

	void init_frame();
	void reshape(int w, int h);
	void draw(character* user);

	int normalize_position(float i);
	int normalize_position(int i);
	
	std::vector<character*>* characters;
	surface* earch;

	triangle* toUpdate;
	hurtable* toRemove;
private:
	looper* sun;
	chunk* chunks;
	std::set<hurtable*> hurtables;
	std::set<wall*> walls;
	
	int size;
	float* bg;
	int distance;

    void init(int size);
};

