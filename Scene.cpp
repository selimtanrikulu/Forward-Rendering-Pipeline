#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cmath>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"
#include "Transformation.h"
#include "Line.h"

using namespace tinyxml2;
using namespace std;

/*
	Transformations, clipping, culling, rasterization are done here.
	You may define helper functions.
*/

Camera currentCam;

bool Scene::Visible(float den,float num,float &tE,float &tL)
{
	if(den > 0)
	{
		float t = num/den;
		if(t > tL)
		{
			return false;
		}
		if(t >tE)
		{
			tE = t;
		}
	}
	else if(den < 0)
	{
		float t = num/den;
		if(t < tE)
		{
			return false;
		}

		if(t < tL)
		{
			tL = t;
		}

	}
	else
	{
		if(num > 0)
		{
			return false;
		}
	}

	return true;


}



void Scene::LiangBarsky(Line &line)
{
	float tE=0;
	float tL=1;
	float t = 1;
	

	float xMin = -t;
	float xMax = t;
	float yMin = -t;
	float yMax = t;
	float zMin = -t;
	float zMax = t;
	float dx = line.p2.x-line.p1.x;
	float dy = line.p2.y-line.p1.y;
	float dz = line.p2.z-line.p1.z;

	line.isVisible = false;

	if(
		Visible(dx,xMin-line.p1.x,tE,tL) &&
		Visible(-dx,line.p1.x-xMax,tE,tL) &&
		Visible(dy,yMin-line.p1.y,tE,tL) && 
		Visible(-dy,line.p1.y-yMax,tE,tL) &&
		Visible(dz,zMin-line.p1.z,tE,tL) &&
		Visible(-dz,line.p1.z-zMax,tE,tL)
	)
		{
			line.isVisible = true;

			Color temp1 = line.color1;
			Color temp2 = line.color2;
			float divider = line.p2.x-line.p1.x;

			if(tL < 1)
			{
				float temp = line.p2.x;
				line.p2.x = line.p1.x + dx*tL;
				line.p2.y = line.p1.y + dy*tL;
				line.p2.z = line.p1.z + dz*tL;
				float alpha = (temp-line.p2.x)/ divider;
				line.color2 = temp2*(1-alpha) + temp1*alpha;
			}
			if(tE > 0)
			{	
				float temp = line.p1.x;
				line.p1.x += dx * tE;
				line.p1.y += dy * tE;
				line.p1.z += dy * tE;
				float alpha = (temp - line.p1.x)/ -divider;
				line.color1 = temp1*(1-alpha) + temp2*alpha;
			}
			return;
		}
					
}


void Scene::CalculateNormals(Mesh *mesh)
{

	for(Triangle &triangle : mesh->triangles)
	{
		Vec3 v1 = *(vertices[triangle.getFirstVertexId()-1]);
		Vec3 v2 = *(vertices[triangle.getSecondVertexId()-1]);
		Vec3 v3 = *(vertices[triangle.getThirdVertexId()-1]);

		triangle.normal = ((v2-v1).Cross(v3-v1)).Normalized();
	}

}


void Scene::TransformNormals(Matrix4 matrix,Mesh *mesh)
{
	for(Triangle &triangle : mesh->triangles)
	{
		Vec4 temp(triangle.normal);
		temp.t = 0;
		temp = multiplyMatrixWithVec4(matrix,temp);


		triangle.normal.x = temp.x;
		triangle.normal.y = temp.y;
		triangle.normal.z = temp.z;

		triangle.normal = triangle.normal.Normalized();
	}
}


/*draws on pixel*/
void Scene::Draw(int x,int y, Color c)
{

	//image[x][y] = RGB;
	//cout << x << " " << y << "\n";
	//cout <<c.r << " " << c.g << " " << c.b << "\n\n";

	if(x < 0 || y<0 || x > currentCam.horRes-1 || y>currentCam.verRes-1)
	{
	//cout << x << " "	<< y << "\n";
		return;
	}
	c.Clamp();
	
	//cout <<"here"<<"\n";
	image[x][y] = c;
	
}


void Scene::Rasterize(Line &line)
{
	// cout << line.color1 << "   " << line.color2 << "\n";

	if(line.p1.x > line.p2.x)
	{
		std::swap(line.p1, line.p2);
		//std::cout << line.color1 << " " << line.color2  << "\n";
		std::swap(line.color1, line.color2);
		//std::cout << line.color1 << " " << line.color2  << "\n";
	}

	float m = (line.p2.y-line.p1.y)/(line.p2.x-line.p1.x);
	
	if(m>1)
	{
		RasterizeLine(line.p1.y, line.p2.y, line.p1.x, line.p2.x, 1, line.color1, line.color2, true);
	}
	else if(m>0)
	{
		RasterizeLine(line.p1.x, line.p2.x, line.p1.y, line.p2.y, 1, line.color1, line.color2,false);
		
	}
	else if(m>-1)
	{
		RasterizeLine(line.p1.x, line.p2.x, line.p1.y, line.p2.y, -1, line.color1, line.color2,false);
	}
	else
	{
		RasterizeLine(line.p2.y, line.p1.y, line.p2.x, line.p1.x, -1, line.color2, line.color1, true);

	}



}
void Scene::RasterizeLine(int x0, int x1, int y0, int y1, int step ,Color c0, Color c1,bool swapped)
{
	int y = y0;
	int A = y0 - y1;
	int B = x1 - x0;
	int d;

	if(step == -1)
		A *= -1;

	d = 2*A + B;
	
	
	Color c = c0;
	
	Color dc = (c1 - c0)/ (x1 - x0);
	for(int x = x0; x <= x1;x++)
	{

		if(swapped)Draw(y,x,c.Round());
		else 
			Draw(x,y,c.Round());
		
		if(d < 0)
		{
			y += step;
			d += 2*(A+B);
			
		}
		else
		{
			d += 2*A;
		}
		c = c + dc;
	}
}
float Scene::f_x_y(int x, int y, int x0, int x1, int y0, int y1)
{
	return x*(y0 - y1) + y*(x1 - x0) + x0*y1 - y0*x1;
}

void Scene::Rasterize(Triangle &triangle)
{
	
	int minX = std::min(triangle.v1.x, triangle.v2.x);	
		minX = std::min(minX, (int)triangle.v3.x);
		minX = std::max(0,minX);
	int maxX = std::max(triangle.v1.x,triangle.v2.x);
		maxX = std::max(maxX, (int)triangle.v3.x);
	int minY = std::min(triangle.v2.y, triangle.v3.y);
		minY = std::min(minY, (int)triangle.v1.y);
		minY = std::max(minY, 0);
	int maxY = std::max(triangle.v2.y,triangle.v3.y);
		maxY = std::max((int)triangle.v1.y,maxY);

	
	float f01 = f_x_y(triangle.v3.x, triangle.v3.y, triangle.v1.x, triangle.v2.x, triangle.v1.y, triangle.v2.y);
	float f12 = f_x_y(triangle.v1.x, triangle.v1.y, triangle.v2.x, triangle.v3.x, triangle.v2.y, triangle.v3.y);
	float f20 = f_x_y(triangle.v2.x, triangle.v2.y, triangle.v3.x, triangle.v1.x, triangle.v3.y, triangle.v1.y);
	

	Color c1 = *colorsOfVertices[triangle.getFirstVertexId()-1];
	Color c2 = *colorsOfVertices[triangle.getSecondVertexId()-1];
	Color c3 = *colorsOfVertices[triangle.getThirdVertexId()-1];
	

	//cout <<triangle.v1.y << "   " << triangle.v2.y << "\n";
	//bool temp = false;
	//cout << minY << " " << maxY << "\n";
	for(int y = minY; y <= maxY ; y++)
	{
		
		//cout <<"here";
		for(int x = minX; x <= maxX; x++)
		{
				
			float alpha = f_x_y(x, y, triangle.v2.x, triangle.v3.x, triangle.v2.y, triangle.v3.y)/f12;
			float beta = f_x_y(x, y, triangle.v3.x, triangle.v1.x, triangle.v3.y, triangle.v1.y)/f20;
			float gama = f_x_y(x, y, triangle.v1.x, triangle.v2.x, triangle.v1.y, triangle.v2.y)/f01;
		
			if(alpha >= 0 && beta >= 0 && gama >= 0)
			{
				Color c = (c1*alpha) + (c2*beta) + (c3*gama);
				//std::cout << c.r << " " << c.g << " " << c.b << "\n";
				Draw(x,y,c.Round());
			}
			
		}
		//temp = true;
	}

	//cout << temp <<"\n";

}




void Scene::Transformations( vector<Triangle> &triangles, vector<Line> &lines )
{
	
	int idCounter = 1;
	
	//int temp = 0;

	for(Mesh* mesh : meshes)
	{	
		//temp++;
		CalculateNormals(mesh);
		Matrix4 global;global.SetIdentity();
		for(int i = 0;i<mesh->numberOfTransformations;i++)
		{
			//cout << mesh->transformationTypes[i] << "\n";

			if(mesh->transformationTypes[i] == 's')
			{
				Scaling s = *(scalings[mesh->transformationIds[i]-1]);
				//cout << " in";
				//cout << s.sx << " " << s.sy << " " << s.sz << "\n";
				Matrix4 mat = Transformation::Transform(s);
				global = multiplyMatrixWithMatrix(mat,global);
			}
			else if(mesh->transformationTypes[i] == 'r')
			{
				
				Rotation r = *(rotations[mesh->transformationIds[i]-1]);
				Matrix4 mat = Transformation::Transform(r);
				global = multiplyMatrixWithMatrix(mat,global);
			}
			else if(mesh->transformationTypes[i] == 't')
			{
				
				Translation t = *(translations[mesh->transformationIds[i]-1]);
				//cout << t.tx << " "<< t.ty << " " << t.tz << "\n";
				Matrix4 mat = Transformation::Transform(t);
				global = multiplyMatrixWithMatrix(mat,global);
				
			}
		}


		for(int i=0;i<mesh->numberOfTriangles;i++)
		{
			mesh->triangles[i].id = idCounter;

	
			Vec4 v1(*vertices[mesh->triangles[i].vertexIds[0]-1]);
			Vec4 v2(*vertices[mesh->triangles[i].vertexIds[1]-1]);
			Vec4 v3(*vertices[mesh->triangles[i].vertexIds[2]-1]);
			

			

			v1 = multiplyMatrixWithVec4(global,v1);
			v2 = multiplyMatrixWithVec4(global,v2);
			v3 = multiplyMatrixWithVec4(global,v3);

			
			
			// std::cout << v1 << std::endl;
				/*(vertices[mesh->triangles[i].vertexIds[0]-1])->x = v1.x;
				(vertices[mesh->triangles[i].vertexIds[0]-1])->y = v1.y;
				(vertices[mesh->triangles[i].vertexIds[0]-1])->z = v1.z;

				(vertices[mesh->triangles[i].vertexIds[1]-1])->x = v2.x;
				(vertices[mesh->triangles[i].vertexIds[1]-1])->y = v2.y;
				(vertices[mesh->triangles[i].vertexIds[1]-1])->z = v2.z;

				(vertices[mesh->triangles[i].vertexIds[2]-1])->x = v3.x;
				(vertices[mesh->triangles[i].vertexIds[2]-1])->y = v3.y;
				(vertices[mesh->triangles[i].vertexIds[2]-1])->z = v3.z;


				mesh->triangles[i].isTransformed = true;*/
			


			if(mesh->type == 0)
			{
				Color color1 = *colorsOfVertices[mesh->triangles[i].vertexIds[0]-1];
				Color color2 = *colorsOfVertices[mesh->triangles[i].vertexIds[1]-1];
				Color color3 = *colorsOfVertices[mesh->triangles[i].vertexIds[2]-1];


				//cout << color1 << "   " << color2 << "    " << color3 << "\n";


				lines.push_back(Line(v1,v2,idCounter,color1,color2));
				lines.push_back(Line(v2,v3,idCounter,color2,color3));
				lines.push_back(Line(v3,v1,idCounter,color3,color1));

				

				idCounter++;
			}
			else 
			{
				//cout << v1 << "\n";
					
				triangles.push_back(Triangle(v1,v2,v3,idCounter++,
					mesh->triangles[i].vertexIds[0]
					,mesh->triangles[i].vertexIds[1]
					,mesh->triangles[i].vertexIds[2]));

				//cout << triangles.back().v1 << "-" <<  triangles.back().v2 << "-" << triangles.back().v3 << "\n";

			}
		}
		
		
		// for(Triangle triangle : mesh->triangles)
		// {
			// cout << triangle.normal << "\n";
		// }
		
		TransformNormals(global.InverseTransposed3(),mesh);
		
		// for(Triangle triangle : mesh->triangles)
		// {
		// 	cout << triangle.normal << "\n";
		// }
	}
	//cout << temp << "\n";
	
}

void Scene::forwardRenderingPipeline(Camera *camera,vector<Triangle> triangles,vector<Line> lines)
{
	currentCam = *camera;
	//backface culling
	if(cullingEnabled)
	{
		for(Mesh* mesh : meshes)
		{
			for(Triangle triangle : mesh->triangles)
			{
				for(auto  it=lines.begin() ; it != lines.end();)
				{
					Vec3 v(it->p1.x,it->p1.y,it->p1.z,-1);
				
					float temp =  (v - camera->pos).Dot(triangle.normal);
					if(camera->projectionType != 1) temp*=-1;
					if((triangle.id == it->triangleID) && temp > 0)
					{
						//cout << "deleted" << endl;
						lines.erase(it);
					}
					else
					{
						it++;
					}
				}
			}
		}

		

		
		for(Mesh* mesh : meshes)
		{
			for(Triangle triangle : mesh->triangles)
			{
				for(auto  it=triangles.begin() ; it != triangles.end();)
				{
					Vec3 v(it->v1.x,it->v1.y,it->v1.z,-1);
					//cout << v;
					float temp =  (v - camera->pos).Dot(triangle.normal);
					if(camera->projectionType != 1) temp*=-1;
					if((triangle.id == it->id) &&temp  > 0)
					{
						//cout << "deleted" << endl;
						triangles.erase(it);
					}
					else
					{
						it++;
					}
				}
			}
		}
	}
	
	
	


	Matrix4 cameraTransformation = Transformation::CameraTransformation(*camera);
	Matrix4 projection;
	if(camera->projectionType == 1) // perspective
	{
		projection = Transformation::PerspectiveProjection(*camera);
	}
	else
	{
		projection = Transformation::OrthographicProjection(*camera);
		//cout <<projection<<"\n";
	}
	
	Matrix4 viewingTransformation = multiplyMatrixWithMatrix(projection,cameraTransformation); //multiplyMatrixWithMatrix(projection,cameraTransformation);
	//std :: cout << viewingTransformation;
	// apply viewing transformations to vertices

	

	for(Triangle &triangle: triangles)
	{
		
		triangle.v1 = multiplyMatrixWithVec4(viewingTransformation,triangle.v1);
		triangle.v2 = multiplyMatrixWithVec4(viewingTransformation,triangle.v2);
		triangle.v3 = multiplyMatrixWithVec4(viewingTransformation,triangle.v3);
		
		

		triangle.v1.PerspectiveDivide();
		triangle.v2.PerspectiveDivide();
		triangle.v3.PerspectiveDivide();
	}

	
	
	for(Line &line : lines)
	{
		
		line.p1 = multiplyMatrixWithVec4(viewingTransformation,line.p1);
		line.p2 = multiplyMatrixWithVec4(viewingTransformation,line.p2);
		

		line.p1.PerspectiveDivide();
		line.p2.PerspectiveDivide();
		
	}

	/*if(cullingEnabled)
	{
		for(Mesh* mesh : meshes)
		{
			for(Triangle triangle : mesh->triangles)
			{
				for(auto  it=lines.begin() ; it != lines.end();)
				{
					Vec3 v(it->p1.x,it->p1.y,it->p1.z,-1);
				
					float temp =  (v - camera->pos).Dot(triangle.normal);
					if(camera->projectionType != 1) temp*=-1;
					if((triangle.id == it->triangleID) && temp > 0)
					{
						//cout << "deleted" << endl;
						lines.erase(it);
					}
					else
					{
						it++;
					}
				}
			}
		}

		

		
		for(Mesh* mesh : meshes)
		{
			for(Triangle triangle : mesh->triangles)
			{
				for(auto  it=triangles.begin() ; it != triangles.end();)
				{
					Vec3 v(it->v1.x,it->v1.y,it->v1.z,-1);
					//cout << v;
					float temp =  (v - camera->pos).Dot(triangle.normal);
					if(camera->projectionType != 1) temp*=-1;
					if((triangle.id == it->id) &&temp  > 0)
					{
						//cout << "deleted" << endl;
						triangles.erase(it);
					}
					else
					{
						it++;
					}
				}
			}
		}
	}*/



	// clip and cull for lines

	vector<Line> visibleLines;

	for(Line &line : lines)
	{

		LiangBarsky(line);
		if(line.isVisible)
		{
			
			
			visibleLines.push_back(line);
			//line.Print();
			
		}

	}

	Matrix4 viewportTransformation = Transformation::ViewportTransformation(camera->horRes,camera->verRes,0,0);

	for(Line line : visibleLines)
	{
		line.p1 = multiplyMatrixWithVec4(viewportTransformation,line.p1);
		line.p2 = multiplyMatrixWithVec4(viewportTransformation,line.p2);
		//line.Print();
		
		//line.Print();
		Rasterize(line);
	}

	for(Triangle triangle : triangles)
	{
		//cout << triangle.v1 << "\n";
		triangle.v1 = multiplyMatrixWithVec4(viewportTransformation,triangle.v1);
		triangle.v2 = multiplyMatrixWithVec4(viewportTransformation,triangle.v2);
		triangle.v3 = multiplyMatrixWithVec4(viewportTransformation,triangle.v3);
		//cout << triangle.v1 << "\n\n";

		Rasterize(triangle);
	}

	
}

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *pElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	// read background color
	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	pElement = pRoot->FirstChildElement("Culling");
	if (pElement != NULL) {
		str = pElement->GetText();
		
		if (strcmp(str, "enabled") == 0) {
			cullingEnabled = true;
		}
		else {
			cullingEnabled = false;
		}
	}

	// read cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != NULL)
	{
		Camera *cam = new Camera();

		pCamera->QueryIntAttribute("id", &cam->cameraId);

		// read projection type
		str = pCamera->Attribute("type");

		if (strcmp(str, "orthographic") == 0) {
			cam->projectionType = 0;
		}
		else {
			cam->projectionType = 1;
		}

		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

		cam->gaze = normalizeVec3(cam->gaze);
		cam->u = crossProductVec3(cam->gaze, cam->v);
		cam->u = normalizeVec3(cam->u);

		cam->w = inverseVec3(cam->gaze);
		cam->v = crossProductVec3(cam->u, cam->gaze);
		cam->v = normalizeVec3(cam->v);

		camElement = pCamera->FirstChildElement("ImagePlane");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &cam->left, &cam->right, &cam->bottom, &cam->top,
			   &cam->near, &cam->far, &cam->horRes, &cam->verRes);

		camElement = pCamera->FirstChildElement("OutputName");
		str = camElement->GetText();
		cam->outputFileName = string(str);

		cameras.push_back(cam);

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// read vertices
	pElement = pRoot->FirstChildElement("Vertices");
	XMLElement *pVertex = pElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (pVertex != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = pVertex->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = pVertex->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		vertices.push_back(vertex);
		colorsOfVertices.push_back(color);

		pVertex = pVertex->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	pElement = pRoot->FirstChildElement("Translations");
	XMLElement *pTranslation = pElement->FirstChildElement("Translation");
	while (pTranslation != NULL)
	{
		Translation *translation = new Translation();

		pTranslation->QueryIntAttribute("id", &translation->translationId);

		str = pTranslation->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		translations.push_back(translation);

		pTranslation = pTranslation->NextSiblingElement("Translation");
	}

	// read scalings
	pElement = pRoot->FirstChildElement("Scalings");
	XMLElement *pScaling = pElement->FirstChildElement("Scaling");
	while (pScaling != NULL)
	{
		Scaling *scaling = new Scaling();

		pScaling->QueryIntAttribute("id", &scaling->scalingId);
		str = pScaling->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		scalings.push_back(scaling);

		pScaling = pScaling->NextSiblingElement("Scaling");
	}

	// read rotations
	pElement = pRoot->FirstChildElement("Rotations");
	XMLElement *pRotation = pElement->FirstChildElement("Rotation");
	while (pRotation != NULL)
	{
		Rotation *rotation = new Rotation();

		pRotation->QueryIntAttribute("id", &rotation->rotationId);
		str = pRotation->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		rotations.push_back(rotation);

		pRotation = pRotation->NextSiblingElement("Rotation");
	}

	// read meshes
	pElement = pRoot->FirstChildElement("Meshes");

	XMLElement *pMesh = pElement->FirstChildElement("Mesh");
	XMLElement *meshElement;
	while (pMesh != NULL)
	{
		Mesh *mesh = new Mesh();

		pMesh->QueryIntAttribute("id", &mesh->meshId);

		// read projection type
		str = pMesh->Attribute("type");

		if (strcmp(str, "wireframe") == 0) {
			mesh->type = 0;
		}
		else {
			mesh->type = 1;
		}

		// read mesh transformations
		XMLElement *pTransformations = pMesh->FirstChildElement("Transformations");
		XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

		while (pTransformation != NULL)
		{
			char transformationType;
			int transformationId;

			str = pTransformation->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			mesh->transformationTypes.push_back(transformationType);
			mesh->transformationIds.push_back(transformationId);

			pTransformation = pTransformation->NextSiblingElement("Transformation");
		}

		mesh->numberOfTransformations = mesh->transformationIds.size();

		// read mesh faces
		char *row;
		char *clone_str;
		int v1, v2, v3;
		XMLElement *pFaces = pMesh->FirstChildElement("Faces");
        str = pFaces->GetText();
		clone_str = strdup(str);

		row = strtok(clone_str, "\n");
		while (row != NULL)
		{
			sscanf(row, "%d %d %d", &v1, &v2, &v3);
			mesh->triangles.push_back(Triangle(v1, v2, v3));
			row = strtok(NULL, "\n");
		}
		mesh->numberOfTriangles = mesh->triangles.size();
		meshes.push_back(mesh);

		pMesh = pMesh->NextSiblingElement("Mesh");
	}
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				this->image[i][j].r = this->backgroundColor.r;
				this->image[i][j].g = this->backgroundColor.g;
				this->image[i][j].b = this->backgroundColor.b;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFileName.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFileName << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// call command on Windows
	else if (osType == 2)
	{
		command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// default action - don't do conversion
	else
	{
	}
}