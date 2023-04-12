/*#ifndef __CLASSES__
#define __CLASSES__

#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

#define PI 3.14159265


namespace myclasses
{
    class Vector4
    {
        public : 
            float x;
            float y;
            float z;
            float h;

            Vector4(float x,float y,float z, float h){this->x=x;this->y=y;this->z=z;this->h=h;}
            Vector4(){};


            void Print()
            {
                cout << x << endl;
                cout << y << endl;
                cout << z << endl;
                cout << h << endl;

            }

            Vector4 PerspectiveDivided()
            {
                Vector4 result(x/h,y/h,z/h,1);
                return result;
            }


    };
    class Vector3
    {
        public : 
            float x, y, z;
            Vector3(float x,float y,float z){this->x=x; this->y=y; this->z=z;}
            Vector3(){x = 0; y = 0; z = 0;}

            //methods
            float Dot(Vector3 second){return x*second.x+y*second.y+z*second.z;}
            Vector3 Cross(Vector3 second){
                Vector3 result(y*second.z-z*second.y,z*second.x-x*second.z,x*second.y-y*second.x);
                return result;
            }
            Vector3 Normalized()
            {   
                float len = sqrt(x*x+y*y+z*z);
                Vector3 result(x/len,y/len,z/len);
                return result;
            }

            Vector3 ColorProduct(Vector3 second)
            {
                Vector3 result;
                result.x = x*second.x;
                result.y = y*second.y;
                result.z = z*second.z;
                return result;
            }

            float Length()
            {
                return sqrt(x*x+y*y+z*z);
            }


            //operator overloadings
            Vector3 operator+(const Vector3& second)
            {
            Vector3 result;
            result.x = this->x + second.x;
            result.y = this->y + second.y;
            result.z = this->z + second.z;
            return result;
            }
            Vector3 operator-(const Vector3& second)
            {
            Vector3 result;
            result.x = this->x - second.x;
            result.y = this->y - second.y;
            result.z = this->z - second.z;
            return result;
            }
            Vector3 operator*(const float& coef)
            {
            Vector3 result;
            result.x = this->x * coef;
            result.y = this->y * coef;
            result.z = this->z * coef;
            return result;
            }

            void Clamp()
                {
                    if(x<0)x=0;if(y<0)y=0;if(z<0)z=0;
                    if(x>255)x=255;if(y>255)y=255;if(z>255)z=255;
                }

            void Print()
            {
                cout << x << endl;
                cout << y << endl;
                cout << z << endl;

            }

            
    };
    class Matrix4
    {
        public : 

            vector<vector<float>> matrix;

            Matrix4(Vector4 v1,Vector4 v2,Vector4 v3,Vector4 v4)
            {   
                vector<Vector4> vectors;
                vectors.push_back(v1);vectors.push_back(v2);vectors.push_back(v3);vectors.push_back(v4);
                for(int i=0;i<4;i++)
                {
                    vector<float> row;
                    row.push_back(vectors[i].x);
                    row.push_back(vectors[i].y);
                    row.push_back(vectors[i].z);
                    row.push_back(vectors[i].h);
                    matrix.push_back(row);
                }
            }

            Matrix4()
            {
                for(int i=0;i<4;i++)
                {
                    vector<float> row;
                    matrix.push_back(row);
                    for(int j=0;j<4;j++)
                    {
                        matrix[i].push_back(0);
                    }
                }
            }

            Matrix4 Transposed()
            {
                Matrix4 result;
                for(int i=0;i<4;i++)
                {
                    for(int j=0;j<4;j++)
                    {
                        result.matrix[i][j] = matrix[j][i];
                    }
                }
                return result;
                
            }


            void Print()
            {
                for(int i=0;i<4;i++)
                {
                    for(int j=0;j<4;j++)
                    {
                        cout << matrix[i][j] << " ";
                    }
                    cout << endl;
                }
            }

            Matrix4 operator*(Matrix4 right)
            {
                Matrix4 result;
                for(int i=0;i<4;i++)
                {
                    for(int j=0;j<4;j++)
                    {
                        for(int k=0;k<4;k++)
                        {
                            result.matrix[i][j] += matrix[i][k]*right.matrix[k][j];      
                        }
                        
                    }
                }
                
                return result;
            }

            Vector4 operator*(Vector4 vec)
            {
                Vector4 result;
                result.x = vec.x*matrix[0][0] +  vec.y*matrix[0][1] + vec.z*matrix[0][2] + vec.h*matrix[0][3];
                result.y = vec.x*matrix[1][0] +  vec.y*matrix[1][1] + vec.z*matrix[1][2] + vec.h*matrix[1][3];
                result.z = vec.x*matrix[2][0] +  vec.y*matrix[2][1] + vec.z*matrix[2][2] + vec.h*matrix[2][3];
                result.h = vec.x*matrix[3][0] +  vec.y*matrix[3][1] + vec.z*matrix[3][2] + vec.h*matrix[3][3];
                return result;
            }

            void SetIdentity()
            {
                for(int i=0;i<4;i++)
                {
                    for(int j=0;j<4;j++)
                    {
                        if(i==j)matrix[i][j]=1;
                        else matrix[i][j]=0;
                    }
                }
            }


            



    };
    

    class Camera
    {
        public : 
            //given
            Vector3 gaze,up,e;
            float n,f,r,l,t,b;
            int nx,ny;


            //to be calculated
            Vector3 u,v,w;



            Camera(Vector3 e,Vector3 gaze,Vector3 up,float n,float f,float r,float l,float t,float b,int nx,int ny)
            {
                this->e=e;this->gaze=gaze;this->up=up;this->n=n;this->f=f;this->nx=nx;this->ny=ny;
                this->r=r;this->l=l;this->t=t;this->b=b;
                gaze = gaze.Normalized();up=up.Normalized();
                w = gaze*-1;
                u = gaze.Cross(up);
                v = up;
            }




            void Print()
            {
                cout << "u : (" << u.x << "," << u.y << "," << u.z << ")\n"; 
                cout << "v : (" << v.x << "," << v.y << "," << v.z << ")\n";
                cout << "w : (" << w.x << "," << w.y << "," << w.z << ")\n";
                cout << "Resolution : " << nx << " x " << ny << "\n";
                cout << "n:" << n << " f:" << f << " r:" << r << " l:" << l << " t:" << t << " b:" << b;
            }

    };



    class Transformation
    {
        public : 
            
            float cosAngle(float angle)
            {
                return cos((angle*PI)/180);
            }
            float sinAngle(float angle)
            {
                return sin((angle*PI)/180);
            }

            Matrix4 Translation(float x,float y,float z)
            {
                Matrix4 result;
                result.SetIdentity();

                result.matrix[0][3] = x;
                result.matrix[1][3] = y;
                result.matrix[2][3] = z;


                return result;
            }

            Matrix4 Rotation(Vector3 p1,Vector3 p2,float angle)
            {
                Vector3 orthonormalX = (p2-p1).Normalized();

                Vector3 orthonormalY(orthonormalX.y,-orthonormalX.x,0); 
                orthonormalY = orthonormalY.Normalized();

                Vector3 orthonormalZ = (orthonormalX.Cross(orthonormalY)).Normalized();

                Matrix4 transformation1 = Translation(-p1.x,-p1.y,-p1.z);

                Vector4 r1(orthonormalX.x,orthonormalX.y,orthonormalX.z,0);
                Vector4 r2(orthonormalY.x,orthonormalY.y,orthonormalY.z,0);
                Vector4 r3(orthonormalZ.x,orthonormalZ.y,orthonormalZ.z,0);
                Vector4 r4(0,0,0,1);

                Matrix4 transformation2(r1,r2,r3,r4);

                Matrix4 transformation3 = Rotation("x",angle);


                Matrix4 transformation4 = transformation2.Transposed();

                Matrix4 transformation5 = Translation(p1.x,p1.y,p1.z);


                Matrix4 result = ((((transformation5*transformation4)*transformation3)*transformation2)*transformation1);


                return result;
            }

            Matrix4 Rotation(string axis,float angle)
            {
                Matrix4 result;
                if(axis == "x")
                {
                    Vector4 r1(1,0,0,0);
                    Vector4 r2(0,cosAngle(angle),-sinAngle(angle),0);
                    Vector4 r3(0,sinAngle(angle),cosAngle(angle),0);
                    Vector4 r4(0,0,0,1);
                    Matrix4 temp(r1,r2,r3,r4);
                    result = temp;
                }
                else if(axis == "y")
                {
                    Vector4 r1(cosAngle(angle),0,sinAngle(angle),0);
                    Vector4 r2(0,1,0,0);
                    Vector4 r3(-sinAngle(angle),0,cosAngle(angle),0);
                    Vector4 r4(0,0,0,1);
                    Matrix4 temp(r1,r2,r3,r4);
                    result = temp;
                }
                else if(axis == "z")
                {
                    Vector4 r1(cosAngle(angle),-sinAngle(angle),0,0);
                    Vector4 r2(sinAngle(angle),cosAngle(angle),0,0);
                    Vector4 r3(0,0,1,0);
                    Vector4 r4(0,0,0,1);
                    Matrix4 temp(r1,r2,r3,r4);
                    result = temp;
                }


                return result;
            }

            Matrix4 Scaling(float s)
            {
                Matrix4 result;
                result.SetIdentity();
                result.matrix[0][0]=s;result.matrix[1][1]=s;result.matrix[2][2]=s;result.matrix[3][3]=1;
                return result;
            }

            Matrix4 CameraTransformation(Vector3 e,Vector3 gaze,Vector3 up)
            {
                Matrix4 transformation1 = Translation(-e.x,-e.y,-e.z);

                Vector3 orthoX = (gaze.Cross(up)).Normalized();
                Vector3 orthoY = up.Normalized();
                Vector3 orthoZ = gaze*-1;

                Vector4 r1(orthoX.x,orthoX.y,orthoX.z,0);
                Vector4 r2(orthoY.x,orthoY.y,orthoY.z,0);
                Vector4 r3(orthoZ.x,orthoZ.y,orthoZ.z,0);
                Vector4 r4(0,0,0,1);

                Matrix4 transformation2(r1,r2,r3,r4); 

                Matrix4 result = transformation2*transformation1;
                return result;
            }

            Matrix4 OrthographicProjection(Camera cam)
            {
                float r=cam.r;float l=cam.l;float t=cam.t;float b=cam.b;float n=cam.n;float f=cam.f;
                Vector4 r1(2/(r-l),0,0,-((r+l)/(r-l)));
                Vector4 r2(0,2/(t-b),0,-((t+b)/(t-b)));
                Vector4 r3(0,0,-(2/(f-n)),-((f+n)/(f-n)));
                Vector4 r4(0,0,0,1);

                Matrix4 result(r1,r2,r3,r4);
                return result;
            }
    
            Matrix4 PerspectiveProjection(Camera cam)
            {
                float r = cam.r; float l= cam.l; float t=cam.t; float b=cam.b; float n=cam.n; float f=cam.f;
                Vector4 r1((2*n)/(r-l),0,(r+l)/(r-l),0);
                Vector4 r2(0,(2*n)/(t-b),(t+b)/(t-b),0);
                Vector4 r3(0,0,-((f+n)/(f-n)),-((2*f*n)/(f-n)));
                Vector4 r4(0,0,-1,0);

                Matrix4 result(r1,r2,r3,r4);
                return result;
            }

            Matrix4 ViewportTransformation(int nx,int ny,int xmin,int ymin)//xmin ve ymin float olabilir ????
            {
                Vector4 r1(nx/2,0,0,((nx-1)/2)+xmin);
                Vector4 r2(0,ny/2,0,((ny-1)/2)+ymin);
                Vector4 r3(0,0,0.5,0.5);
                Vector4 r4(0,0,0,1);

                Matrix4 result(r1,r2,r3,r4);
                return result;
            }

    };

}
#endif
*/