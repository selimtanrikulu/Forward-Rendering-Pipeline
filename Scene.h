#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Line.h"
#include "Matrix4.h"

using namespace std;

class Scene
{
public:
	Color backgroundColor;
	bool cullingEnabled;

	vector< vector<Color> > image;
	vector< Camera* > cameras;
	vector< Vec3* > vertices;
	vector< Color* > colorsOfVertices;
	vector< Scaling* > scalings;
	vector< Rotation* > rotations;
	vector< Translation* > translations;
	vector< Mesh* > meshes;

	Scene(const char *xmlPath);

	
	bool Visible(float den,float num,float &tE,float &tL);
	void LiangBarsky(Line &line);
	void CalculateNormals(Mesh*);
	void TransformNormals(Matrix4 matrix,Mesh *mesh);
	void Transformations(vector<Triangle>&,vector<Line>&);
	float f_x_y(int x, int y, int x0, int x1, int y0, int y1);
	void Rasterize(Line &line);
	void Rasterize(Triangle &triangle);
	void RasterizeLine(int x0, int x1, int y0, int y1, int step ,Color c0, Color c1, bool);
	void Draw(int, int, Color);
	void initializeImage(Camera* camera);
	void forwardRenderingPipeline(Camera* camera, vector<Triangle>, vector<Line>);
	int makeBetweenZeroAnd255(double value);
	void writeImageToPPMFile(Camera* camera);
	void convertPPMToPNG(string ppmFileName, int osType);
};

#endif
