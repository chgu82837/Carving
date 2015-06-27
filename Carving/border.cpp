
#include "stdafx.h"

border::border(float x1,float z1,float x2,float z2,float x_invalid,float z_invalid){
    a = z1 - z2;
    b = x2 - x1;
    c = x1 * z2 - x2 * z1;
    // printf(" (%f,%f) ============ (%f,%f) >>> a = %f, b = %f, c = %f\n",x1,z1,x2,z2 ,a,b,c);
    ab = a*a + b*b;
    if(ab == 0) throw "invalid x1,z1,x2,z2 in border init";
    sqrt_ab = sqrt(ab);

    invalid_area_positive = calculate(x_invalid,z_invalid) >= 0.0;
}

float border::calculate(float x,float z){
    return a * x + b * z + c;
}

bool border::valid(float x,float z){
    return (calculate(x,z) >= 0.0) != invalid_area_positive;
}

float border::distance(float x,float z){
    return abs(calculate(x,z)) / sqrt_ab;
}

float* border::closest(float x,float z){
    float* result = new float[2];
    result[0] = (x * b * b - a * b * z - a * c) / ab;
    if(b == 0) result[1] = z;
    else result[1] = - result[0] * a / b - c / b;
    return result;
}
