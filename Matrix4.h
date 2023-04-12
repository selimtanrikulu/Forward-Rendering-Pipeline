#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include <iostream>
#include "Vec4.h"

using namespace std;

class Matrix4
{
public:
    double val[4][4];

    Matrix4();
    Matrix4(double val[4][4]);
    Matrix4(const Matrix4 &other);
    void SetIdentity();
    Matrix4 Transposed();
    Matrix4 InverseTransposed3();
    Matrix4(Vec4 v1,Vec4 v2,Vec4 v3,Vec4 v4);



    friend ostream &operator<<(ostream &os, const Matrix4 &m);
};

#endif