#include <3d.hpp>
#include <iostream>
#include <cmath>

vec2d::vec2d(float x, float y){
    this->x = x;
    this->y = y;
    this->base_x = x;
    this->base_y = y;
}

vec2d vec2d::add(const vec2d &a) const{
    return vec2d(this->x + a.x, this=>y + a.y);
}

vec2d vec2d::sub(const vec2d &a) const{
    return vec2d(this->x - a.x, this->y - a.y);
}

vec2d vec2d::scalar(float k) const{
    return vec2d(this->x * k, this->y * k);
}

vec2d vec2d::abs() const{
    return sqrt(this->x * this->x + this->y * this->y);
}

vec2d vec2d::normalize() const{
    float abs = this->abs();
    return vec2d(this->x / this->abs, this->y / this->abs);
}

vec2d vec2d::dot(const vec2d &a) const{
    return this->x * a.x + this->y * a.y;
}

float vec2d::get_cos(const vec2d &a) const{
    float dot = this->dot(a);
    float this_abs = tihs->abs();
    float a_abs = a.abs();
    float cos = dot / (this_abs * a_abs);
    return cos;
}

vec3d::vec3d(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->base_x = x;
    this->base_y = y;
    this->base_z = z;
}

vec3d vec3d::add(const vec3d &a) const
{
    return vec3d(this->x + a.x, this->y + a.y, this->z + a.z);
}

vec3d vec3d::sub(const vec3d &a) const
{
    return vec3d(this->x - a.x, this->y - a.y, this->z - a.z);
}

vec3d vec3d::scalar(float k) const
{
    return vec3d(this->x * k, this->y * k, this->z * k);
}

float vec3d::dot(const vec3d &a) const
{
    return this->x * a.x + this->y * a.y + this->z * a.z;
}

vec3d vec3d::cross(const vec3d &a) const
{
    return vec3d(
        this->y * a.z - this->z * a.y,
        this->z * a.x - this->x * a.z,
        this->x * a.y - this->y * a.x);
}

float vec3d::abs() const
{
    return sqrt(
        this->x * this->x +
        this->y * this->y +
        this->z * this->z);
}

vec3d vec3d::normalize() const
{
    float abs = this->abs();
    return vec3d(
        this->x / abs,
        this->y / abs,
        this->z / abs);
}

float vec3d::get_cos(const vec3d &a) const
{
    float dot = this->dot(a);
    float this_abs = this->abs();
    float a_abs = a.abs();
    return dot / (this_abs * a_abs);
}

vec3d vec3d::operator+(const vec3d &a) const
{
    return this->add(a);
}

vec3d vec3d::operator-(const vec3d &a) const
{
    return this->sub(a);
}

vec3d vec3d::operator*(float k) const
{
    return this->scalar(k);
}

bool vec3d::operator==(const vec3d &a) const
{
    const float epsilon = 1e-6f;
    return (std::abs(this->x - a.x) < epsilon &&
            std::abs(this->y - a.y) < epsilon &&
            std::abs(this->z - a.z) < epsilon);
}

matrix::matrix(vector<vector<float>> data)
{
    this->rows = data.size();
    this->cols = data[0].size();
    this->data.resize(this->rows, vector<float>(this->cols, 0));
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            this->data[i][j] = data[i][j];
        }
    }
}

matrix matrix::add(const matrix &a) const
{
    if (this->rows != a.rows || this->cols != a.cols)
    {
        throw invalid_argument("Matrix dimensions do not match for addition.");
    }
    vector<vector<float>> result(this->rows, vector<float>(this->cols, 0));
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            result[i][j] = this->data[i][j] + a.data[i][j];
        }
    }
    return matrix(result);
}

matrix matrix::sub(const matrix &a) const
{
    if (this->rows != a.rows || this->cols != a.cols)
    {
        throw invalid_argument("Matrix dimensions do not match for subtraction.");
    }
    vector<vector<float>> result(this->rows, vector<float>(this->cols, 0));
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            result[i][j] = this->data[i][j] - a.data[i][j];
        }
    }
    return matrix(result);
}

matrix matrix::scalar(const float k) const
{
    vector<vector<float>> result(this->rows, vector<float>(this->cols, 0));
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            result[i][j] = this->data[i][j] * k;
        }
    }
    return matrix(result);
}

matrix matrix::mul(const matrix &a) const
{
    if (this->cols != a.rows)
    {
        throw invalid_argument("Matrix dimensions do not match for multiplication.");
    }
    vector<vector<float>> result(this->rows, vector<float>(a.cols, 0));
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            for (int k = 0; k < this->cols; k++)
            {
                result[i][j] += this->data[i][k] * a.data[k][j];
            }
        }
    }
    return matrix(result);
}

matrix matrix::transpose() const
{
    vector<vector<float>> result(this->cols, vector<float>(this->rows, 0));
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            result[j][i] = this->data[i][j];
        }
    }
    return matrix(result);
}

float matrix::determinant() const
{
    if (this->rows != this->cols)
    {
        throw invalid_argument("Matrix is not square.");
    }

    if (this->rows == 2)
    {
        return this->data[0][0] * this->data[1][1] - this->data[0][1] * this->data[1][0];
    }
    if (this->rows == 3)
    {
        return this->data[0][0] * this->data[1][1] * this->data[2][2] +
               this->data[0][2] * this->data[1][0] * this->data[2][1] +
               this->data[0][1] * this->data[1][2] * this->data[2][0] -
               this->data[0][2] * this->data[1][1] * this->data[2][0] -
               this->data[0][1] * this->data[1][0] * this->data[2][2] -
               this->data[0][0] * this->data[1][2] * this->data[2][1];
    }
    if (this->rows == 4)
    {
        return this->data[0][0] * this->data[1][1] * this->data[2][2] * this->data[3][3] +
               this->data[0][1] * this->data[1][2] * this->data[2][3] * this->data[3][0] +
               this->data[0][2] * this->data[1][3] * this->data[2][0] * this->data[3][1] +
               this->data[0][3] * this->data[1][0] * this->data[2][1] * this->data[3][2] -
               this->data[0][3] * this->data[1][2] * this->data[2][1] * this->data[3][0] -
               this->data[0][2] * this->data[1][1] * this->data[2][0] * this->data[3][3] -
               this->data[0][1] * this->data[1][0] * this->data[2][3] * this->data[3][2] -
               this->data[0][0] * this->data[1][3] * this->data[2][2] * this->data[3][1];
    }
    throw invalid_argument("Matrix size not supported for determinant calculation.");
}

matrix matrix::inverse() const
{
    if (this->rows != this->cols)
    {
        throw invalid_argument("Matrix is not square.");
    }
    float det = this->determinant();
    if (det == 0)
    {
        throw invalid_argument("Matrix is singular and cannot be inverted.");
    }
    if (this->rows == 2)
    {
        return matrix(vector<vector<float>>{{this->data[1][1], -this->data[0][1]},
                                            {-this->data[1][0], this->data[0][0]}})
            .scalar(1 / det);
    }
    if (this->rows == 3)
    {
        return matrix(vector<vector<float>>{{this->data[1][1] * this->data[2][2] - this->data[1][2] * this->data[2][1],
                                             this->data[0][2] * this->data[2][1] - this->data[0][1] * this->data[2][2],
                                             this->data[0][1] * this->data[1][2] - this->data[0][2] * this->data[1][1]},
                                            {this->data[1][2] * this->data[2][0] - this->data[1][0] * this->data[2][2],
                                             this->data[0][0] * this->data[2][2] - this->data[0][2] * this->data[2][0],
                                             this->data[0][2] * this->data[1][0] - this->data[0][0] * this->data[1][2]},
                                            {this->data[1][0] * this->data[2][1] - this->data[1][1] * this->data[2][0],
                                             this->data[0][1] * this->data[2][0] - this->data[0][0] * this->data[2][1],
                                             this->data[0][0] * this->data[1][1] - this->data[0][1] * this->data[1][0]}})
            .scalar(1 / det);
    }
    throw invalid_argument("Matrix size not supported for inversion.");
}

matrix matrix::operator+(const matrix &a) const
{
    return this->add(a);
}

matrix matrix::operator-(const matrix &a) const
{
    return this->sub(a);
}

matrix matrix::operator*(float k) const
{
    return this->scalar(k);
}

matrix matrix::operator*(const matrix &a) const
{
    return this->mul(a);
}

bool matrix::operator==(const matrix &a) const
{
    const float epsilon = 1e-6f;
    if (this->rows != a.rows || this->cols != a.cols)
    {
        return false;
    }
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            if (std::abs(this->data[i][j] - a.data[i][j]) > epsilon)
            {
                return false;
            }
        }
    }
    return true;
}

quaternion::quaternion(float w, float x, float y, float z)
{
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
    this->base_w = w;
    this->base_x = x;
    this->base_y = y;
    this->base_z = z;
}

quaternion quaternion::add(const quaternion &a) const
{
    return quaternion(
        this->w + a.w,
        this->x + a.x,
        this->y + a.y,
        this->z + a.z);
}

quaternion quaternion::sub(const quaternion &a) const
{
    return quaternion(
        this->w - a.w,
        this->x - a.x,
        this->y - a.y,
        this->z - a.z);
}

quaternion quaternion::scalar(float k) const
{
    return quaternion(
        this->w * k,
        this->x * k,
        this->y * k,
        this->z * k);
}

quaternion quaternion::mul(const quaternion &a) const
{
    return quaternion(
        this->w * a.w - this->x * a.x - this->y * a.y - this->z * a.z,
        this->w * a.x + this->x * a.w + this->y * a.z - this->z * a.y,
        this->w * a.y + this->y * a.w + this->z * a.x - this->x * a.z,
        this->w * a.z + this->z * a.w + this->x * a.y - this->y * a.x);
}

quaternion quaternion::conjugate() const
{
    return quaternion(
        this->w,
        -this->x,
        -this->y,
        -this->z);
}

float quaternion::abs() const
{
    return sqrt(
        this->w * this->w +
        this->x * this->x +
        this->y * this->y +
        this->z * this->z);
}

quaternion quaternion::normalize() const
{
    float abs = this->abs();
    return quaternion(
        this->w / abs,
        this->x / abs,
        this->y / abs,
        this->z / abs);
}

quaternion quaternion::operator+(const quaternion &a) const
{
    return this->add(a);
}

quaternion quaternion::operator-(const quaternion &a) const
{
    return this->sub(a);
}

quaternion quaternion::operator*(float k) const
{
    return this->scalar(k);
}

quaternion quaternion::operator*(const quaternion &a) const
{
    return this->mul(a);
}

bool quaternion::operator==(const quaternion &a) const
{
    const float epsilon = 1e-6f;
    return (
        std::abs(this->w - a.w) < epsilon &&
        std::abs(this->x - a.x) < epsilon &&
        std::abs(this->y - a.y) < epsilon &&
        std::abs(this->z - a.z) < epsilon);
}

matrix solve(const matrix &a, const matrix &b)
{
    if (a.rows != b.rows || a.cols != b.cols)
    {
        throw invalid_argument("Matrix dimensions do not match for solving.");
    }
    matrix a_inv = a.inverse();
    return a_inv.mul(b);
}

vec3d move(const vec3d &a, const vec3d &translation, float anglex, float angley, float anglez)
{
    float x = a.x;
    float y = a.y;
    float z = a.z;

    float sx = 0.0f, sy = 0.0f, sz = 0.0f;
    float cx = 1.0f, cy = 1.0f, cz = 1.0f;
    float tx = translation.x;
    float ty = translation.y;
    float tz = translation.z;
    if (anglex != 0.0f)
    {
        sx = sin(anglex);
        cx = cos(anglex);
    }
    if (angley != 0.0f)
    {
        sy = sin(angley);
        cy = cos(angley);
    }
    if (anglez != 0.0f)
    {
        sz = sin(anglez);
        cz = cos(anglez);
    }

    matrix afine = matrix(vector<vector<float>>{
        {cy * cz, cy * sz, -sy, tx},
        {sx * sy * cz - cx * sz, sx * sy * sz + cx * cz, sx * cy, ty},
        {cx * sy * cz + sx * sz, cx * sy * sz - sx * cz, cx * cy, tz},
        {0, 0, 0, 1}});
    matrix b = matrix(vector<vector<float>>{
        {x},
        {y},
        {z},
        {1}});

    matrix result = afine.mul(b);
    return vec3d(result.data[0][0], result.data[1][0], result.data[2][0]);
}

vec3d rotate_with_quaternion(const vec3d &a, const vec3d &axis, float angles[3])
{   
    float anglex = 0.0f, angley = 0.0f, anglez = 0.0f;
    float angle = 0.0f;
    
    if (angles[1] != 0 || angles[2] != 0) {
        anglex = angles[0];
        angley = angles[1];
        anglez = angles[2];
    }
    else {
        angle = angles[0];
    }

    float x = a.x;
    float y = a.y;
    float z = a.z;
    float axis_x = axis.x;
    float axis_y = axis.y;
    float axis_z = axis.z;
    float half_anglex = anglex / 2;
    float half_angley = angley / 2;
    float half_anglez = anglez / 2;
    float half_angle = angle / 2;
    float sin_x, sin_y, sin_z;
    float cos_x, cos_y, cos_z;
    float sine, cosine;
    if (anglex != 0)
    {
        sin_x = sin(half_anglex);
        cos_x = cos(half_anglex);
    }
    if (angley != 0)
    {
        sin_y = sin(half_angley);
        cos_y = cos(half_angley);
    }
    if (anglez != 0)
    {
        sin_z = sin(half_anglez);
        cos_z = cos(half_anglez);
    }
    if (angle != 0){
        sine = sin(half_angle);
        cosine = cos(half_angle);
    }
    
    if(axis == vec3d(0, 0, 0)){

        quaternion q1 = quaternion(cos_x, sin_x * axis_x, 0, 0);
        quaternion q2 = quaternion(cos_y, 0, sin_y * axis_y, 0);
        quaternion q3 = quaternion(cos_z, 0, 0, sin_z * axis_z);
        quaternion q = q1.mul(q2).mul(q3);
        quaternion q_conjugate = q.conjugate();
        quaternion q_a = quaternion(0, x, y, z);
        quaternion q_result = q.mul(q_a).mul(q_conjugate);
        return vec3d(q_result.x, q_result.y, q_result.z);
    }else{
        quaternion q = quaternion(cosine, sine * axis_x, sine * axis_y, sine * axis_z);
        quaternion q_conjugate = q.conjugate();
        quaternion q_a = quaternion(0, x, y, z);
        quaternion q_result = q.mul(q_a).mul(q_conjugate);
        return vec3d(q_result.x, q_result.y, q_result.z);
    }
}