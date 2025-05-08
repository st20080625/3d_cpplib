#ifndef D_CPP_3D_HPP
#define D_CPP_3D_HPP
#include <vector>
using namespace std;

class vec2d{
    public:
        float x, y, base_x, base_y;
        vec2d(float x, float y);
        vec2d add(const vec2d &a) const;
        vec2d sub(const vec2d &a) const;
        float abs() const;
        vec2d scalar(float k) const;
        float dot(const vec2d &a) const;
        vec2d normalize() const;
        float get_cos(const vec2d &a) const;
};

class vec3d
{
public:
    float x, y, z, base_x, base_y, base_z;
    vec3d(float x, float y, float z);
    vec3d add(const vec3d &a) const;
    vec3d sub(const vec3d &a) const;
    vec3d scalar(float k) const;
    float dot(const vec3d &a) const;
    vec3d cross(const vec3d &a) const;
    float abs() const;
    vec3d normalize() const;
    float get_cos(const vec3d &a) const;
    float scale();
    float calc_point();


    vec3d operator+(const vec3d &a) const;
    vec3d operator-(const vec3d &a) const;
    vec3d operator*(float k) const;
    bool operator==(const vec3d &a) const;
};

class matrix
{
public:
    int rows, cols;
    vector<vector<float>> data;
    matrix(vector<vector<float>> data);
    matrix add(const matrix &a) const;
    matrix sub(const matrix &a) const;
    matrix scalar(float k) const;
    matrix mul(const matrix &a) const;
    matrix transpose() const;
    matrix inverse() const;
    float determinant() const;

    matrix operator+(const matrix &a) const;
    matrix operator-(const matrix &a) const;
    matrix operator*(float k) const;
    matrix operator*(const matrix &a) const;
    bool operator==(const matrix &a) const;
};

class quaternion
{
public:
    float w, x, y, z, base_w, base_x, base_y, base_z;
    quaternion(float w, float x, float y, float z);
    quaternion add(const quaternion &a) const;
    quaternion sub(const quaternion &a) const;
    quaternion scalar(float k) const;
    quaternion mul(const quaternion &a) const;
    quaternion conjugate() const;
    float abs() const;
    quaternion normalize() const;

    quaternion operator+(const quaternion &a) const;
    quaternion operator-(const quaternion &a) const;
    quaternion operator*(float k) const;
    quaternion operator*(const quaternion &a) const;
    bool operator==(const quaternion &a) const;
};

matrix solve(const matrix &a, const matrix &b);
vec3d move(const vec3d &a, const vec3d &b, float anglex = 0.0f, float angley = 0.0f, float anglez = 0.0f);
vec3d rotate_with_quaternion(const vec3d &a, const vec3d &axis = vec3d(0, 0, 0), float angles[3] = {0});

#endif