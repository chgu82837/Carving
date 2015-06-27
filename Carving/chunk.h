
class chunk
{
public:
	chunk(void);
	~chunk(void);

	std::set<spot*>::iterator iter_spots(int type);
	std::set<spot*>::iterator iter_spots();
	bool iter_ends(int type,std::set<spot*>::iterator* i);
	bool iter_ends(std::set<spot*>::iterator* i);
	int size();
	int size(int type);
	void insert(spot* s);
	void erase(spot* s);

	void static hightlight(int x,int z,float size);

private:
	std::map<int,std::set<spot*>*>* spots;
	//std::set<spot*>* spots;
	void prepare();
	void prepare(int type);
};

