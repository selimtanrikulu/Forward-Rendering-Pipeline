#pragma once
#include "Vec4.h"




class Line
{
    public : 
        Vec4 p1;
        Vec4 p2;
        bool isVisible;
        int triangleID;
        Color color1;
        Color color2;

        Line(Vec4 p1,Vec4 p2,int id,Color color1,Color color2)
        {
            this->p1 = p1;
            this->p2 = p2;
            this->triangleID = id;
            this->color1 = color1;
            this->color2 = color2;
        }
        
        void Print()
        {
            std::cout << p1 << "\n";
            std::cout << p2  << "\n";

        }
        




};

