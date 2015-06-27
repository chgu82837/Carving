
class border
{
public:
    border(float x1,float z1,float x2,float z2,float x_invalid,float z_invalid);
    bool valid(float x,float z);
    float distance(float x,float z);
    float* closest(float x,float z); // return float[2]
    
private:
    // for the formula ax + by + c
    float a;
    float b;
    float c;
    bool invalid_area_positive;
    float ab;
    float sqrt_ab;

    float calculate(float x,float z);
};
