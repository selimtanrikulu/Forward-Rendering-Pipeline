#ifndef __COLOR_H__
#define __COLOR_H__

#include <iostream>
#include <cmath>

class Color
{
public:
    double r, g, b;

    Color();
    Color(double r, double g, double b);
    Color(const Color &other);
    Color operator+(const Color &rhs);
    Color operator-(const Color &rhs);
    Color operator/(const float);
    Color operator*(const float);
    Color Round();
    void Clamp();
    friend std::ostream& operator<<(std::ostream& os, const Color& c);
};

#endif