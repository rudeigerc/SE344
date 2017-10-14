/*
 *
 * config.h
 * Author: Yuchen Cheng <rudeigerc@gmail.com>
 *
 */
#ifndef CONFIG_H
#define CONFIG_H

#define g 9.8	// gravitational acceleration
#define RADIUS 1.0f
#define COEFFICIENT 0.2 // supposed velocity loss of collision

#define KEY_ESC 27
#define KEY_SPACE 32

enum MENU_LIST {
	MENU_EXIT = 0,
	MENU_RESET,
	MENU_IMPORT_CUBE,
	MENU_IMPORT_BUNNY,
	MENU_IMPORT_COMPLEX,
	MENU_IMPORT_FLOWER,
	MENU_IMPORT_CUSTOM
};

const GLfloat material_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
// reference: http://www.it.hiof.no/~borres/j3d/explain/light/p-materials.html
// ruby
const GLfloat material_ambient[] = { 0.1745f, 0.01175f, 0.01175f, 0.55f };
const GLfloat material_diffuse[] = { 0.61424f, 0.04136f, 0.04136f, 0.55f };
const GLfloat material_specular[] = { 0.727811f, 0.626959f, 0.626959f, 0.55f };
const GLfloat material_shininess = 76.8f;

const GLfloat light_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light0_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
const GLfloat light1_position[] = { 1.0f, 1.0f, -1.0f, 1.0f };

#endif