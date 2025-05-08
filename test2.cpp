#include <iostream>
#define PI 3.14159265358979323846
#include <cmath>
#include <vector>
#include "3d.hpp"

int main(){
    float angle1 = 1/2* PI;
    float angle2 = 1/2* PI;
    float angle3 = 1/2* PI;
    vec3d axis = vec3d(0,0,0);
    angle1 = 1.0/2.0* PI / 2.0;
    angle2 = 1.0/2.0* PI / 2.0;
    angle3 = 1.0/2.0* PI / 2.0;
    
    vec3d vec1 = vec3d(1,1,1);
    float angles[3] = {angle1, angle2, angle3};
    vec3d rotated_vec = rotate_with_quaternion(vec1, axis, angles);
    std::cout << "rotated_vec: (" << rotated_vec.x << ", " << rotated_vec.y << ", " << rotated_vec.z << ")" << std::endl;
    return 0;
}