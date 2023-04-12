
#include "Vec4.h"
#include <iomanip>


using namespace std;

Vec4::Vec4()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->t = 0.0;
    this->colorId = -1;
}

Vec4::Vec4(const Vec3  &v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->t = 1;
    this->colorId = v.colorId;
}

Vec4::Vec4(double x, double y, double z, double t, int colorId)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->t = t;
    this->colorId = colorId;
}
Vec4::Vec4(const Vec4 &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->t = other.t;
    this->colorId = other.colorId;
}

double Vec4::getElementAt(int index)
{
    switch (index)
    {
    case 0:
        return this->x;

    case 1:
        return this->y;

    case 2:
        return this->z;

    case 3:
        return this->t;

    default:
        return this->t;
    }
}
void Vec4::PerspectiveDivide()
{
    //std::cout << this->x << "\n";
    //std::cout << this->t << "\n";
    this->x /= this->t;
    this->y /= this->t;
    this->z /= this->t;
    this-> t = 1;
    //std::cout << this->x << "\n";
    //std::cout << this->t << "\n";

}
void Vec4::Round()
{
    // Rounds to nearest integer x,y,z
    this->x = round(this->x);
    this->y = round(this->y);
    this->z = round(this->z);
}
ostream& operator<<(ostream& os, const Vec4& v) {
    
    os << fixed << setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.t << "]";

    return os;
}