// Включаем стандартные заголовки
#include <stdio.h>
#include <stdlib.h>

#include<X11/X.h>
#include<X11/Xlib.h>

// Включаем GLEW. Всегда включайте его ДО gl.h и glfw.h
#include "GL/glew.h"

// Включаем GLFW
#include "GLFW/glfw3.h"

#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>


Display                 *dpy;
Window                  root;
GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo             *vi;
Colormap                cmap;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XWindowAttributes       gwa;
XEvent                  xev;

void DrawAQuad() {
 glClearColor(1.0, 1.0, 1.0, 1.0);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-1., 1., -1., 1., 1., 20.);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

 glBegin(GL_QUADS);
  glColor3f(1., 0., 0.); glVertex3f(-.75, -.75, 0.);
  glColor3f(0., 1., 0.); glVertex3f( .75, -.75, 0.);
  glColor3f(0., 0., 1.); glVertex3f( .75,  .75, 0.);
  glColor3f(1., 1., 0.); glVertex3f(-.75,  .75, 0.);
 glEnd();
}

int main(){
    // Инициализируем GLFW
    /*if( !glfwInit() )
    {
        fprintf( stderr, "Ошибка при инициализации GLFWn" );
        return -1;
    }*/

   return 0;
}
