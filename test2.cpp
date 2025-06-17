#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include "3d.hpp"
#include "draw.hpp"
#include <string>
#include <fstream>
#include <cstdint>

using namespace std;
using namespace cpp3d;
vector<vector<vec3d>> STL2vec3d(const string &file_path){
    ifstream file(file_path, ios::binary);
    char header[80];
    file.read(header, 80);

    uint32_t triangle_count;
    file.read(reinterpret_cast<char*>(&triangle_count), 4);

    vector<vec3d> normals;
    vector<vec3d> vectors;
    for(uint32_t i = 0; i < triangle_count; ++i){
        float nx;
        float ny;
        float nz;
        file.read(reinterpret_cast<char*>(&nx), 4);
        file.read(reinterpret_cast<char*>(&ny), 4);
        file.read(reinterpret_cast<char*>(&nz), 4);
        vec3d normal_vector(nx, ny, nz);
        normals.push_back(normal_vector);
        float x;
        float y;
        float z;
        for(int j = 0; j < 3; ++j){
            file.read(reinterpret_cast<char*>(&x), 4);
            file.read(reinterpret_cast<char*>(&y), 4);
            file.read(reinterpret_cast<char*>(&z), 4);
            vec3d vector(x, y, z);
            vectors.push_back(vector);
        }
        uint16_t attribute_byte_count;
        file.read(reinterpret_cast<char*>(&attribute_byte_count), 2);
    }
    return {normals, vectors};
}

int main(){
    string file_path = "cover.stl";
    vector<vector<vec3d>> vectors = STL2vec3d(file_path);
    cout << "vectors_size: "<< vectors.size() << endl;
    cout << "normals_size: "<< vectors[0].size() << endl;
    cout << "vectors_size: "<< vectors[1].size() << endl;
    return 0;
}