#ifndef _GHSGRAPHICS_H_
#define _GHSGRAPHICS_H_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#ifdef __gnu_linux__
#include <GL/gl.h>
#include <GL/freeglut.h>
#endif

#include <ghsdata.h>
#include <smartgoku.h>

// Init openGL
void init_opengl();

#endif
