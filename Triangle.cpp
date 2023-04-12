#include "Triangle.h"


Triangle::Triangle() {
    this->vertexIds[0] = -1;
    this->vertexIds[1] = -1;
    this->vertexIds[2] = -1;
}

Triangle::Triangle(int vid1, int vid2, int vid3)
{
    this->vertexIds[0] = vid1;
    this->vertexIds[1] = vid2;
    this->vertexIds[2] = vid3;
}

Triangle::Triangle(const Triangle &other)
{
    this->vertexIds[0] = other.vertexIds[0];
    this->vertexIds[1] = other.vertexIds[1];
    this->vertexIds[2] = other.vertexIds[2];
    this->v1 = other.v1;
    this->v2 = other.v2;
    this->v3 = other.v3;
    this->id = other.id;
    this->normal = other.normal;
    this->isReject = other.isReject;
}

// getters
int Triangle::getFirstVertexId()
{
    return this->vertexIds[0];
}

int Triangle::getSecondVertexId()
{
    return this->vertexIds[1];
}

int Triangle::getThirdVertexId()
{
    return this->vertexIds[2];
}

// setters
void Triangle::setFirstVertexId(int vid)
{
    this->vertexIds[0] = vid;
}

void Triangle::setSecondVertexId(int vid)
{
    this->vertexIds[1] = vid;
}

void Triangle::setThirdVertexId(int vid)
{
    this->vertexIds[2] = vid;
}


Triangle::Triangle(Vec4 v1,Vec4 v2,Vec4 v3,int id,int vertexID1,int vertexID2,int vertexID3)
{
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    this->id = id;
    this->vertexIds[0] = vertexID1;
    this->vertexIds[1] = vertexID2;
    this->vertexIds[2] = vertexID3;
}