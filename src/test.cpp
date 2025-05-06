#include "3d.hpp"
#include <iostream>
#include <vector>
#include <iomanip>
#define _USE_MATH_DEFINES
#include <cmath>

// ベクトル表示用の出力演算子
std::ostream& operator<<(std::ostream& os, const vec3d& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

// 行列表示用の出力演算子
std::ostream& operator<<(std::ostream& os, const matrix& m) {
    os << "\n";
    for (int i = 0; i < m.rows; i++) {
        os << "  [ ";
        for (int j = 0; j < m.cols; j++) {
            os << std::setw(8) << std::fixed << std::setprecision(4) << m.data[i][j];
            if (j < m.cols - 1) os << " ";
        }
        os << " ]";
        if (i < m.rows - 1) os << "\n";
    }
    return os;
}

// クォータニオン表示用の出力演算子
std::ostream& operator<<(std::ostream& os, const quaternion& q) {
    os << "(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
    return os;
}

int main() {
    // 初期値の設定
    vec3d vec_a = vec3d(1, 2, 3);
    vec3d vec_b = vec3d(4, 5, 6);
    matrix a = matrix(vector<vector<float>>{{1, 2}, {3, 4}});
    matrix b = matrix(vector<vector<float>>{{5, 6}, {7, 8}});
    quaternion quat_a = quaternion(1, 2, 3, 4);
    quaternion quat_b = quaternion(5, 6, 7, 8);

    // 計算実行
    vec3d vec_c = vec_a + vec_b;
    vec3d vec_d = vec_a - vec_b;  // 修正: 元のコードでは間違いがあったようです
    vec3d vec_e = vec_a * 2.0f;
    float dot = vec_a.dot(vec_b);
    vec3d vec_f = vec_a.cross(vec_b);
    float abs_a = vec_a.abs();
    vec3d vec_g = vec_a.normalize();
    float cos_a = vec_a.get_cos(vec_b);

    matrix matrix_c = a + b;
    matrix matrix_d = a - b;
    matrix matrix_e = a * 2.0f;
    matrix matrix_f = a * b;
    matrix matrix_g = a.transpose();
    matrix inv_a = a.inverse();
    float det_a = a.determinant();
    matrix matrix_h = solve(a, b);
    
    // PI/2ではなく浮動小数点で計算するように修正
    float half_pi = M_PI / 2.0f;  // 整数の1/2ではなく浮動小数点の割り算
    vec3d vec_h = move(vec_a, vec_b, 0.0f, 0.0f, 0.0f);
    vec3d vec_i = move(vec_a, vec3d(0, 0, 0), half_pi, half_pi, half_pi);
    vec3d vec_j = move(vec_a, vec_b, half_pi, half_pi, half_pi);

    quaternion quat_c = quat_a + quat_b;
    quaternion quat_d = quat_a - quat_b;
    quaternion quat_e = quat_a * 2.0f;
    quaternion quat_f = quat_a * quat_b;
    quaternion quat_g = quat_a.conjugate();
    float abs_b = quat_a.abs();
    quaternion quat_h = quat_a.normalize();
    vec3d vec_k = rotate_with_quaternion(vec_a, vec_b, half_pi, half_pi, half_pi);

    // 結果出力
    std::cout << "===== ベクトル演算 =====" << std::endl;
    std::cout << "vec_a = " << vec_a << std::endl;
    std::cout << "vec_b = " << vec_b << std::endl;
    std::cout << "vec_a + vec_b = " << vec_c << std::endl;
    std::cout << "vec_a - vec_b = " << vec_d << std::endl;
    std::cout << "vec_a * 2.0f = " << vec_e << std::endl;
    std::cout << "vec_a.dot(vec_b) = " << dot << std::endl;
    std::cout << "vec_a.cross(vec_b) = " << vec_f << std::endl;
    std::cout << "vec_a.abs() = " << abs_a << std::endl;
    std::cout << "vec_a.normalize() = " << vec_g << std::endl;
    std::cout << "vec_a.get_cos(vec_b) = " << cos_a << std::endl;

    std::cout << "\n===== 行列演算 =====" << std::endl;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "a + b = " << matrix_c << std::endl;
    std::cout << "a - b = " << matrix_d << std::endl;
    std::cout << "a * 2.0f = " << matrix_e << std::endl;
    std::cout << "a * b = " << matrix_f << std::endl;
    std::cout << "a.transpose() = " << matrix_g << std::endl;
    std::cout << "a.inverse() = " << inv_a << std::endl;
    std::cout << "a.determinant() = " << det_a << std::endl;
    std::cout << "solve(a, b) = " << matrix_h << std::endl;

    std::cout << "\n===== 3D変換 =====" << std::endl;
    std::cout << "move(vec_a, vec_b, 0, 0, 0) = " << vec_h << std::endl;
    std::cout << "move(vec_a, origin, π/2, π/2, π/2) = " << vec_i << std::endl;
    std::cout << "move(vec_a, vec_b, π/2, π/2, π/2) = " << vec_j << std::endl;

    std::cout << "\n===== クォータニオン =====" << std::endl;
    std::cout << "quat_a = " << quat_a << std::endl;
    std::cout << "quat_b = " << quat_b << std::endl;
    std::cout << "quat_a + quat_b = " << quat_c << std::endl;
    std::cout << "quat_a - quat_b = " << quat_d << std::endl;
    std::cout << "quat_a * 2.0f = " << quat_e << std::endl;
    std::cout << "quat_a * quat_b = " << quat_f << std::endl;
    std::cout << "quat_a.conjugate() = " << quat_g << std::endl;
    std::cout << "quat_a.abs() = " << abs_b << std::endl;
    std::cout << "quat_a.normalize() = " << quat_h << std::endl;
    std::cout << "rotate_with_quaternion(vec_a, vec_b, π/2, π/2, π/2) = " << vec_k << std::endl;

    return 0;
}