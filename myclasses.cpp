/*#include "myclasses.h"


using namespace std;
using namespace myclasses;


Transformation transformer;

Vector3 e(0,0,0);
Vector3 gaze(0,0,-1);
Vector3 up(0,1,0);
float n = 1;
float f = 2;
int nx = 800;
int ny = 600;
float t = 3;
float b = -3;
float r = 4;
float l = -4;



int main() {
    
    Vector3 vertice1(0,7,-2);

    Camera cam(e,gaze,up,n,f,r,l,t,b,nx,ny);
    //cam.Print();

    Matrix4 t1 = transformer.PerspectiveProjection(cam);
    

    Vector4 v1(vertice1.x,vertice1.y,vertice1.z,1);

    v1 = t1*v1;



    v1.Print();

    return 0;
}*/