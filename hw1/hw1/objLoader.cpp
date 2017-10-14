/*
*
* objLoader.cpp
* Author: Yuchen Cheng <rudeigerc@gmail.com>
*
*/
#include "objLoader.h"
#include <iostream>
#include <fstream>
#include <string>

std::vector<std::string> split(std::string str, char pattern)
{
    int pos;
    std::vector<std::string> result;
    str += pattern;
    int size = str.size();
	for (int i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if (pos < size) {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
			i = pos;
        }
    }
	return result;
}

ObjLoader::ObjLoader() { }

ObjLoader::~ObjLoader() { }

void ObjLoader::initLoader() {
	vertices.clear();
	normalVectors.clear();
	indexes.clear();
	normalIndexes.clear();
	min_x = FLT_MAX;
	min_y = FLT_MAX;
	min_z = FLT_MAX;
	max_x = FLT_MIN;
	max_y = FLT_MIN;
	max_z = FLT_MIN;
	scale = 0.0f;

	Vertex vertex_padding;
	NormalVector normal_padding;
	vertices.push_back(vertex_padding);
	normalVectors.push_back(normal_padding);
}

void ObjLoader::minmax(GLfloat x, GLfloat y, GLfloat z) {
	min_x = (x < min_x) ? x : min_x;
	min_y = (y < min_y) ? y : min_y;
	min_z = (z < min_z) ? z : min_z;

	max_x = (x > max_x) ? x : max_x;
	max_y = (y > max_y) ? y : max_y;
	max_z = (z > max_z) ? z : max_z;
}

bool ObjLoader::load(char *path) {
	initLoader();
	std::ifstream file(path);
	std::string line;
	if (!file) {
		printf("Cannot open file %s\n", path);
		return false;
	}
	while (std::getline(file, line)) {
		std::vector<std::string> elements;
		switch (line[0]) {
		case '#':
			break;
		case 'v':
			switch (line[1]) {
			case ' ':
				elements = split(line.substr(2), ' ');
				Vertex vertex;
				vertex.x = atof(elements[0].c_str());
				vertex.y = atof(elements[1].c_str());
				vertex.z = atof(elements[2].c_str());
				minmax(vertex.x, vertex.y, vertex.z);
				vertices.push_back(vertex);
				break;
			case 'n':
				elements = split(line.substr(3), ' ');
				NormalVector normalVector;
				normalVector.x = atof(elements[0].c_str());
				normalVector.y = atof(elements[1].c_str());
				normalVector.z = atof(elements[2].c_str());
				normalVectors.push_back(normalVector);
				break;
			}
			break;
		case 'f':
			elements = split(line.substr(2), ' ');
			std::vector<std::string> params;
			std::vector<unsigned int> tmp_indexes;
			std::vector<unsigned int> tmp_normalIndexes;

			for (std::string param : elements) {
				params = split(param, '/');
				if (params.size() == 3) {
					tmp_indexes.push_back(atoi(params[0].c_str()));
					tmp_normalIndexes.push_back(atoi(params[2].c_str()));
				}
			}
			if (tmp_indexes.size()) {
				for (int i = 1; i <= tmp_indexes.size() - 2; i++) {
					indexes.push_back(tmp_indexes[0]);
					indexes.push_back(tmp_indexes[i]);
					indexes.push_back(tmp_indexes[i+1]);
					normalIndexes.push_back(tmp_normalIndexes[0]);
					normalIndexes.push_back(tmp_normalIndexes[i]);
					normalIndexes.push_back(tmp_normalIndexes[i+1]);
				}
			}
			else {
				for (int i = 1; i <= elements.size() - 2; i++) {
					indexes.push_back(atoi(elements[0].c_str()));
					indexes.push_back(atoi(elements[i].c_str()));
					indexes.push_back(atoi(elements[i+1].c_str()));
				}
			}
			break;
		}
	}
	return true;
}

void ObjLoader::calculate() {
	delta_x = (max_x - min_x) / 2.0;
	delta_y = (max_y - min_y) / 2.0;
	delta_z = (max_z - min_z) / 2.0;
	scale = (abs(delta_x) < abs(delta_y)) ? abs(delta_x) : abs(delta_y);
	scale = (scale < abs(delta_z)) ? scale : abs(delta_z);
}

void ObjLoader::draw() {
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	if (normalVectors.size() > 1) {
		for (int i = 0; i < normalIndexes.size(); i += 3) {
			glNormal3f(normalVectors[normalIndexes[i]].x, normalVectors[normalIndexes[i]].y, normalVectors[normalIndexes[i]].z);
			glNormal3f(normalVectors[normalIndexes[i + 1]].x, normalVectors[normalIndexes[i + 1]].y, normalVectors[normalIndexes[i + 1]].z);
			glNormal3f(normalVectors[normalIndexes[i + 2]].x, normalVectors[normalIndexes[i + 2]].y, normalVectors[normalIndexes[i + 2]].z);
		}
	}
	for (int i = 0; i < indexes.size(); i += 3) {
		glVertex3f(vertices[indexes[i]].x, vertices[indexes[i]].y, vertices[indexes[i]].z);
		glVertex3f(vertices[indexes[i + 1]].x, vertices[indexes[i + 1]].y, vertices[indexes[i + 1]].z);
		glVertex3f(vertices[indexes[i + 2]].x, vertices[indexes[i + 2]].y, vertices[indexes[i + 2]].z);
	}
	glEnd();
	glPopMatrix();
}