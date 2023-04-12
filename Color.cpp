#include "Color.h"
#include <iostream>
#include <iomanip>

using namespace std;

Color::Color() {}

Color::Color(double r, double g, double b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

Color::Color(const Color &other)
{
    this->r = other.r;
    this->g = other.g;
    this->b = other.b;
}
Color Color::operator+(const Color & rhs)
{
    Color result;
    result.r = this->r + rhs.r;
    result.b = this->b + rhs.b;
    result.g = this->g + rhs.g;
    return result;
}
Color Color::operator-(const Color & rhs)
{
    Color result;
    result.r = this->r - rhs.r;
    result.b = this->b - rhs.b;
    result.g = this->g - rhs.g;
    return result;

}
Color Color::operator/(const float c )
{
    Color result;
    result.r = this->r / c;
    result.g = this->g / c;
    result.b = this->b / c;
    return result;
}
Color Color::operator*(const float c)
{
    Color result;
    result.r = this->r * c;
    result.g = this->g * c;
    result.b = this->b *c;
    return result;
}

ostream& operator<<(ostream& os, const Color& c)
{
    os << fixed << setprecision(0) << "rgb(" << c.r << ", " << c.g << ", " << c.b << ")";
    return os;
}


Color Color::Round()
{
    Color result;
    result.r = round(r);
    result.g = round(g);
    result.b = round(b);
    return result;
}


void Color::Clamp()
{
    if(g<0)g=0;else if(g>255)g=255;
    if(r<0)r=0;else if(r>255)r=255;
    if(b<0)b=0;else if(b>255)b=255;
}