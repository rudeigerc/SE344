/* 
 *
 * hw1.cpp
 * Author: Yuchen Cheng <rudeigerc@gmail.com>
 *
 */

#include "stdlib.h"
#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glut.h>
#include "math.h"
#include "time.h"
#include "stdio.h"
#include "config.h"
#include "objLoader.h"

double v0 = 0;	// initial speed velocity
double v = 0;	// instantaneous speed velocity 

float height = 0.0;	// height
float max_height = 0.0; // the max height of each moving-up
float pos = 0.0;
int status = 0; // 0 - static | 1 - up | 2 - down

double t = 0;	// time
clock_t start, end;

ObjLoader objLoader;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0, height - 5, -5.0);
	glRotated(45, 1.0, 1.0, 1.0);
	if (!objLoader.loader) {
		glutSolidSphere(RADIUS, 50, 50);
	}
	else {
		objLoader.calculate();
		//glTranslatef(-objLoader.delta_x, -objLoader.delta_y, -objLoader.delta_z);
		if (objLoader.scale < RADIUS) {
			GLfloat scale_coef = RADIUS / objLoader.scale;
			glScalef(scale_coef, scale_coef, scale_coef);
		}
		objLoader.draw();
	}
	glPopMatrix();
	glutSwapBuffers();
}

void move_up() {
	end = clock();
	t = (double)(end - start) / CLOCKS_PER_SEC;
	v = v0 - g * t;
	height = v0 * t - 0.5 * g * t * t + pos;
	// printf("UP: v - %f, h - %f\n", v, height);
	if (v <= 0) {
		status = 2;	// turn direction to down
		v = 0;	// reset v
		max_height = height;	// set max_height
		pos = 0;	// reset pos
		start = clock();
	}
	display();
	glLoadIdentity();
}

void move_down() {
	end = clock();
	t = (double)(end - start) / CLOCKS_PER_SEC;
	v = g * t;
	height = max_height - 0.5 * g * t * t;
	// printf("DOWN: v - %f, h - %f\n", v, height);
	if (height <= 0) {
		height = 0;	// reset height
		pos = 0;	// reset pos
		if (max_height <= 0.001) {
			// approximatively static, break the bounce loop
			status = 0;
		}
		else {
			status = 1;	// turn direction to up
			v0 = (1 - COEFFICIENT) * v;	// supposed energy loss
			start = clock();
		}
	}
	display();
	glLoadIdentity();
}

void idle() {
	display();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glOrtho(-15 * (GLfloat)(w) / (GLfloat)(h), 15 * (GLfloat)(w) / (GLfloat)(h), -15, 15, -15.0, 15.0);
	//gluLookAt(0.0, 0.0, -5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;
	case KEY_SPACE:
		v0 = 10;
		status = 1;
		pos = height;
		start = clock();
		break;
	case 'W':
		break;
	case 'A':
		break;
	case 'S':
		break;
	case 'D':
		break;
	}

}

void mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		v0 = 10;
		status = 1;
		pos = height;
		start = clock();
	}
}

void timer(int value)
{
	switch (status) {
	case 0:
		break;
	case 1:
		move_up();
		break;
	case 2:
		move_down();
		break;
	}
	glutTimerFunc(10, timer, 0);	// loop
}

void menu_options(int option) {
	char* path;
	switch (option) {
	case MENU_EXIT:
		exit(0);
		break;
	case MENU_IMPORT_CUBE:
		objLoader.loader = true;
		path = "object\\cube.obj";
		if (!objLoader.load(path)) {
			printf("load object failed\n");
		}
		break;
	case MENU_IMPORT_BUNNY:
		objLoader.loader = true;
		path = "object\\bunny.obj";
		if (!objLoader.load(path)) {
			printf("load object failed\n");
		}
		break;
	case MENU_IMPORT_COMPLEX:
		objLoader.loader = true;
		path = "object\\complex.obj";
		if (!objLoader.load(path)) {
			printf("load object failed\n");
		}
		break;
	case MENU_IMPORT_FLOWER:
		objLoader.loader = true;
		path = "object\\flower.obj";
		if (!objLoader.load(path)) {
			printf("load object failed\n");
		}
		break;
	case MENU_IMPORT_CUSTOM:
		objLoader.loader = true;
		path = "";	// please input the path manually
		if (path == "") {
			objLoader.loader = false;
			break;
		}
		if (!objLoader.load(path)) {
			printf("load object failed\n");
		}
		break;
	case MENU_RESET:
		objLoader.loader = false;
		height = 0;
		status = 0;
		display();
		break;
	}
}

void create_menu() {
	int menu, import_submenu;
	import_submenu = glutCreateMenu(menu_options);
	glutAddMenuEntry("Cube", MENU_IMPORT_CUBE);
	glutAddMenuEntry("Bunny", MENU_IMPORT_BUNNY);
	glutAddMenuEntry("Complex", MENU_IMPORT_COMPLEX);
	glutAddMenuEntry("Flower", MENU_IMPORT_FLOWER);
	glutAddMenuEntry("Custom", MENU_IMPORT_CUSTOM);

	menu = glutCreateMenu(menu_options);
	glutAddSubMenu("Import...", import_submenu);
	glutAddMenuEntry("Reset", MENU_RESET);
	glutAddMenuEntry("Exit", MENU_EXIT);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init() {
	glClearColor(1, 1, 1, 1);

	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, material_shininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	create_menu();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("OpenGL");
	init();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutTimerFunc(500, timer, 0);
	glutMainLoop();
	return 0;
}
