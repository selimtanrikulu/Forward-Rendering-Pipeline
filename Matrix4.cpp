#include "Matrix4.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

Matrix4::Matrix4()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            this->val[i][j] = 0;
        }
    }
}

Matrix4::Matrix4(double val[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            this->val[i][j] = val[i][j];
        }
    }
}

Matrix4::Matrix4(const Matrix4 &other)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            this->val[i][j] = other.val[i][j];
        }
    }
}



void Matrix4::SetIdentity()
{
    for(int i=0;i<4;i++)
    {
        val[i][i] = 1;
    }
}

ostream &operator<<(ostream &os, const Matrix4 &m)
{

    os << fixed << setprecision(6) << "|" << m.val[0][0] << "|" << m.val[0][1] << "|" << m.val[0][2] << "|" << m.val[0][3] << "|"
       << endl
       << "|" << m.val[1][0] << "|" << m.val[1][1] << "|" << m.val[1][2] << "|" << m.val[1][3] << "|"
       << endl
       << "|" << m.val[2][0] << "|" << m.val[2][1] << "|" << m.val[2][2] << "|" << m.val[2][3] << "|"
       << endl
       << "|" << m.val[3][0] << "|" << m.val[3][1] << "|" << m.val[3][2] << "|" << m.val[3][3] << "|";

    return os;
}


Matrix4::Matrix4(Vec4 v1,Vec4 v2,Vec4 v3,Vec4 v4)
{   
    vector<Vec4> vectors;
    vectors.push_back(v1);
    vectors.push_back(v2);
    vectors.push_back(v3);
    vectors.push_back(v4);
    for(int i=0;i<4;i++)
    {
        val[i][0] = vectors[i].x; 
        val[i][1] = vectors[i].y; 
        val[i][2] = vectors[i].z; 
        val[i][3] = vectors[i].t; 
    }
}
Matrix4 Matrix4::Transposed()
{
    Matrix4 result;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            result.val[i][j] = val[j][i];
        }
    }
    return result;
    
}


Matrix4 Matrix4::InverseTransposed3()
{
    Matrix4 result = *this;
    
    double a = val[0][0];
    double b = val[0][1];
    double c = val[0][2];
    double d = val[1][0];
    double e = val[1][1];
    double f = val[1][2];
    double g = val[2][0];
    double h = val[2][1];
    double i = val[2][2];



    double matrix3[3][3];


    matrix3[0][0] = (e*i-f*h);
    matrix3[1][0] = -(d*i-f*g);
    matrix3[2][0] = (d*h-e*g); 
    matrix3[0][1] = -(b*i-c*h);
    matrix3[1][1] = (a*i-c*g);
    matrix3[2][1] = -(a*h-g*b);
    matrix3[0][2] = (b*f-e*c);
    matrix3[1][2] = -(a*f-d*c);
    matrix3[2][2] = (a*e-d*b);


    double det = a*matrix3[0][0] + b*matrix3[1][0] + c*matrix3[2][0];

    for(int i = 0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            matrix3[i][j] /= det;
            result.val[j][i] = matrix3[i][j];
        }
    }




 
    return result;
}