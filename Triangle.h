#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include "Vec4.h"

class Triangle
{
public:
    int vertexIds[3];
    Vec4 v1;
    Vec4 v2;
    Vec4 v3;
    Vec3 normal;
    bool isVisible;
    int id;
    bool isReject;

    Triangle();
    Triangle(int vid1, int vid2, int vid3);
    Triangle(const Triangle &other);
    Triangle(Vec4 v1,Vec4 v2,Vec4 v3,int id,int vertexID1,int vertexID2,int vertexID3);

    int getFirstVertexId();
    int getSecondVertexId();
    int getThirdVertexId();

    void setFirstVertexId(int vid);
    void setSecondVertexId(int vid);
    void setThirdVertexId(int vid);

};


#endif