//
//  main.cpp
//  Matrix
//
//  Created by Dov Nimratz on 12.10.2020.
//
#include <iostream>
#include <map>
#include <array>
#include <utility>
using namespace std;

template<typename T, int def=-1>
class Matrix{
public:
    using pointDef = array<size_t, 2>;
private:
    int defVal = def;
    class Proxy;
    map<pointDef,T> matrix;
    
    pointDef point_create(int i, int j) const{
        pointDef p;
        p[0] = i; p[1] = j;
        return p;
    }
public:

    Matrix(){};
    Matrix(int d):defVal(d){};
    size_t size() const{
        return matrix.size();
    }

    Proxy operator()(const pointDef &p){
        return {*this,p};
    }
    Proxy operator()(int i, int j){
        return{*this, point_create(i,j)};
    }
    Proxy operator[](int i){
        return {*this,i};
    }
    T operator()(const pointDef& p) const{
        auto ip = matrix.find(p);
        if(ip == matrix.end()){
            return (T)def;
        }
        return matrix.at(p);
    }
    
private:
    class Proxy{
        Matrix& pMatrix;
        pointDef point;
        int index;
    public:
        Proxy(Matrix &m, const pointDef &p):pMatrix(m),point(p){};
        Proxy(Matrix &m, int first):pMatrix(m),index(0){
            point[0]= first;
        }
        Proxy(const Proxy&) = default;
        Proxy(Proxy&&) = default;
        ~Proxy() = default;
        
        T operator=(T val){
            auto im = pMatrix.matrix.find(point);
            if(im == pMatrix.matrix.end()){
                pMatrix.matrix[point] = val;
            }
            return val;
        }
        Proxy operator[](int i){
            if(index < 2){
                point[1] = i;
                return *this;
            }
            return *this;
        }
        operator T(){
            return as_const(pMatrix)(point);
        }
    };

};

int main(){
    Matrix<int, -1> matrix;
    assert(matrix.size() == 0);
    auto a = matrix(0, 0);
    assert(a == -1);
    assert(matrix.size() == 0);
    matrix(100, 200) = 314;
    assert(matrix(100, 200) == 314);
    assert(matrix.size() == 1);
    matrix[50][10] = 30;
    assert(matrix[50][10] == 30);
    return 0;
}
