#include <cmath>
#include "Matrix4.h"
#include "Vec4.h"
#include "Vec3.h"
#include "Camera.h"
#include "Scaling.h"
#include "Translation.h"
#include "Rotation.h"
#include "Helpers.h"
#define PI 3.14159265


namespace Transformation
{
    Matrix4 Rotate(Vec3 p1,Vec3 p2,float angle);
    Matrix4 Translate(float x,float y,float z);
    Matrix4 Scale(float sx,float sy,float sz);


    Matrix4 Transform(Rotation &rotation)
    {
        return Rotate(Vec3(0,0,0,-1),Vec3(rotation.ux,rotation.uy,rotation.uz,-1),rotation.angle);
    }

    Matrix4 Transform(Scaling &scaling)
    {
        return Scale(scaling.sx,scaling.sy,scaling.sz);
    }
    Matrix4 Transform(Translation &translation)
    {
        return Translate(translation.tx, translation.ty, translation.tz );
        
    }

    float cosAngle(float angle)
    {
        return cos((angle*PI)/180);
    }
    float sinAngle(float angle)
    {
        return sin((angle*PI)/180);
    }

    Matrix4 Translate(float x,float y,float z)
    {
        Matrix4 result;
        result.SetIdentity();

        result.val[0][3] = x;
        result.val[1][3] = y;
        result.val[2][3] = z;


        return result;
    }

    

    Matrix4 Rotate(string axis,float angle)
    {
        Matrix4 result;
        if(axis == "x")
        {
            Vec4 r1(1,0,0,0,-1);
            Vec4 r2(0,cosAngle(angle),-sinAngle(angle),0,-1);
            Vec4 r3(0,sinAngle(angle),cosAngle(angle),0,-1);
            Vec4 r4(0,0,0,1,-1);
            Matrix4 temp(r1,r2,r3,r4);
            result = temp;
        }
        else if(axis == "y")
        {
            Vec4 r1(cosAngle(angle),0,sinAngle(angle),0, -1);
            Vec4 r2(0,1,0,0,-1);
            Vec4 r3(-sinAngle(angle),0,cosAngle(angle),0, -1);
            Vec4 r4(0,0,0,1,-1);
            Matrix4 temp(r1,r2,r3,r4);
            result = temp;
        }
        else if(axis == "z")
        {
            Vec4 r1(cosAngle(angle),-sinAngle(angle),0,0,-1);
            Vec4 r2(sinAngle(angle),cosAngle(angle),0,0,-1);
            Vec4 r3(0,0,1,0,-1);
            Vec4 r4(0,0,0,1,-1);
            Matrix4 temp(r1,r2,r3,r4);
            result = temp;
        }


        return result;
    }

    Matrix4 Rotate(Vec3 p1,Vec3 p2,float angle)
    {
        Vec3 orthonormalX = (p2-p1).Normalized();

        Vec3 orthonormalY;
        if(orthonormalX.y < 0.00001 && orthonormalX.y > -0.00001)
        {
            orthonormalY = Vec3(-orthonormalX.z,0,orthonormalX.x,-1); 
            orthonormalY = orthonormalY.Normalized();
        }
        else
        {
            orthonormalY = Vec3(-orthonormalX.y,orthonormalX.x,0,-1); 
            orthonormalY = orthonormalY.Normalized();
        }
      

        Vec3 orthonormalZ = (orthonormalX.Cross(orthonormalY)).Normalized();

        Matrix4 transformation1 = Translate(-p1.x,-p1.y,-p1.z);

        Vec4 r1(orthonormalX.x,orthonormalX.y,orthonormalX.z,0,-1);
        Vec4 r2(orthonormalY.x,orthonormalY.y,orthonormalY.z,0,-1);
        Vec4 r3(orthonormalZ.x,orthonormalZ.y,orthonormalZ.z,0,-1);
        Vec4 r4(0,0,0,1,-1);

        Matrix4 transformation2(r1,r2,r3,r4);

        Matrix4 transformation3 = Rotate("x",angle);


        Matrix4 transformation4 = transformation2.Transposed();

        Matrix4 transformation5 = Translate(p1.x,p1.y,p1.z);

  
       //Matrix4 result = ((((transformation5*transformation4)*transformation3)*transformation2)*transformation1);
        Matrix4 result = multiplyMatrixWithMatrix(
                            multiplyMatrixWithMatrix(
                                multiplyMatrixWithMatrix(
                                    multiplyMatrixWithMatrix(
                                        transformation5, transformation4),transformation3), transformation2),transformation1);





        return result;
    }

    Matrix4 Scale(float sx,float sy,float sz)
    {
        Matrix4 result;
        result.SetIdentity();
        result.val[0][0]=sx;result.val[1][1]=sy;result.val[2][2]=sz;result.val[3][3]=1;
        return result;
    }

    Matrix4 CameraTransformation(Camera &cam)
    {
        

        Vec3 e = cam.pos;Vec3 gaze = cam.gaze.Normalized(); Vec3 up=cam.v.Normalized();
        Matrix4 transformation1 = Translate(-e.x,-e.y,-e.z);

        Vec3 orthoX = (gaze.Cross(up)).Normalized();
        Vec3 orthoY = up.Normalized();
        Vec3 orthoZ = gaze*-1;

        Vec4 r1(orthoX.x,orthoX.y,orthoX.z,0,-1);
        Vec4 r2(orthoY.x,orthoY.y,orthoY.z,0,-1);
        Vec4 r3(orthoZ.x,orthoZ.y,orthoZ.z,0,-1);
        Vec4 r4(0,0,0,1,-1);

        Matrix4 transformation2(r1,r2,r3,r4); 

        
        Matrix4 result = multiplyMatrixWithMatrix(transformation2, transformation1);
        return result;
    }

    Matrix4 OrthographicProjection(Camera & cam)
    {
        float r = cam.right; float l= cam.left; 
        float t=cam.top; float b=cam.bottom; float n=cam.near; float f=cam.far;

        Vec4 r1(2/(r-l),0,0,-((r+l)/(r-l)),-1);
        Vec4 r2(0,2/(t-b),0,-((t+b)/(t-b)),-1);
        Vec4 r3(0,0,-(2/(f-n)),-((f+n)/(f-n)),-1);
        Vec4 r4(0,0,0,1,-1);

        Matrix4 result(r1,r2,r3,r4);
        return result;
    }

    Matrix4 PerspectiveProjection(Camera& cam)
    {
        float r = cam.right; float l= cam.left; 
        float t=cam.top; float b=cam.bottom; float n=cam.near; float f=cam.far;
        Vec4 r1((2*n)/(r-l),0,(r+l)/(r-l),0,-1);
        Vec4 r2(0,(2*n)/(t-b),(t+b)/(t-b),0,-1);
        Vec4 r3(0,0,-((f+n)/(f-n)),-((2*f*n)/(f-n)),-1);
        Vec4 r4(0,0,-1,0,-1);

        Matrix4 result(r1,r2,r3,r4);
        return result;
    }

    Matrix4 ViewportTransformation(int nx,int ny,int xmin,int ymin)//xmin ve ymin float olabilir ????
    {
        Vec4 r1((nx/2.0),0,0,((nx-1)/2.0)+xmin,-1);
        Vec4 r2(0,(ny/2.0),0,((ny-1)/2.0)+ymin,-1);
        Vec4 r3(0,0,0.5,0.5,-1);
        Vec4 r4(0,0,0,1,-1);

        Matrix4 result(r1,r2,r3,r4);
        return result;
    }
}