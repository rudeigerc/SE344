/*
*
* objLoader.h
* Author: Yuchen Cheng <rudeigerc@gmail.com>
*
*/
#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <vector>
#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glut.h>

class ObjLoader {
private:
	typedef struct {
		GLfloat x;
		GLfloat y;
		GLfloat z;
	} Vertex;	// v

	typedef struct {
		GLfloat x;
		GLfloat y;
		GLfloat z;
	} NormalVector;	// vn

	std::vector<Vertex> vertices;
	std::vector<NormalVector> normalVectors;
	std::vector<unsigned int> indexes;	// f1
	std::vector<unsigned int> normalIndexes;	// f3

	GLfloat min_x = FLT_MAX;
	GLfloat min_y = FLT_MAX;
	GLfloat min_z = FLT_MAX;
	GLfloat max_x = FLT_MIN;
	GLfloat max_y = FLT_MIN;
	GLfloat max_z = FLT_MIN;

public:
	ObjLoader();
	~ObjLoader();

	bool loader = false;
	GLfloat delta_x, delta_y, delta_z;
	GLfloat scale = 0.0f;

	void initLoader();
	void minmax(GLfloat x, GLfloat y, GLfloat z);
	bool load(char *path);
	void calculate();
	void draw();
};
#endif