#include "Vec3.h"
#include <iomanip>
#include <cmath>

using namespace std;

Vec3::Vec3()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->colorId = -1;
}

Vec3::Vec3(double x, double y, double z, int colorId)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->colorId = colorId;
}



Vec3::Vec3(const Vec3 &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->colorId = other.colorId;
}

double Vec3::getElementAt(int index)
{
    switch (index)
    {
    case 0:
        return this->x;

    case 1:
        return this->y;

    case 2:
        return this->z;

    default:
        return this->z;
    }
}

ostream& operator<<(ostream& os, const Vec3& v) {
    
    os << fixed << setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << "]";

    return os;
}


Vec3 Vec3::operator+(const Vec3& second)
{
    Vec3 result;
    result.x = this->x + second.x;
    result.y = this->y + second.y;
    result.z = this->z + second.z;
    return result;
}
Vec3 Vec3::operator-(const Vec3& second)
{
    Vec3 result;
    result.x = this->x - second.x;
    result.y = this->y - second.y;
    result.z = this->z - second.z;
    return result;
}
Vec3 Vec3::operator*(const float& coef)
{
    Vec3 result;
    result.x = this->x * coef;
    result.y = this->y * coef;
    result.z = this->z * coef;
    return result;
}

Vec3 Vec3::Normalized()
{   
    float len = sqrt(x*x+y*y+z*z);
    Vec3 result(x/len,y/len,z/len,-1);
    return result;
}


float Vec3::Dot(const Vec3 &  second)
{
    return x*second.x+y*second.y+z*second.z;
}



Vec3 Vec3::Cross(const Vec3 & second)
{
    Vec3 result(y*second.z-z*second.y,z*second.x-x*second.z,x*second.y-y*second.x,-1);
    return result;
}