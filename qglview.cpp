#include "qglview.h"

// based on the code from 'www.amin-ahmadi.com'

QGLView::QGLView(QWidget *parent) : QOpenGLWidget(parent)
{
    xrot = yrot = zrot = 0.0;
}

void QGLView::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
}

void QGLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -7.0);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

     // FRONT
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0); // white
    glVertex3f(1.0, -0.25, -1.0);
    glVertex3f(1.0, 0.25, -1.0);
    glVertex3f(-1.0, 0.25, -1.0);
    glVertex3f(-1.0, -0.25, -1.0);
    glEnd();

    // BACK
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 0.0); // green
    glVertex3f(1.0, -0.25, 1.0);
    glVertex3f(1.0, 0.25, 1.0);
    glVertex3f(-1.0, 0.25, 1.0);
    glVertex3f(-1.0, -0.25, 1.0);
    glEnd();

    // RIGHT
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 1.0); // pink
    glVertex3f(1.0, -0.25, -1.0);
    glVertex3f(1.0, 0.25, -1.0);
    glVertex3f(1.0, 0.25, 1.0);
    glVertex3f(1.0, -0.25, 1.0);
    glEnd();

    // LEFT
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0); // yellow
    glVertex3f(-1.0, -0.25, 1.0);
    glVertex3f(-1.0, 0.25, 1.0);
    glVertex3f(-1.0, 0.25, -1.0);
    glVertex3f(-1.0, -0.25, -1.0);
    glEnd();

    // TOP
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0); // blue
    glVertex3f(1.0, 0.25, 1.0);
    glVertex3f(1.0, 0.25, -1.0);
    glVertex3f(-1.0, 0.25, -1.0);
    glVertex3f(-1.0, 0.25, 1.0);
    glEnd();

    // Red side - BOTTOM
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0); // red
    glVertex3f(1.0, -0.25, -1.0);
    glVertex3f(1.0, -0.25, 1.0);
    glVertex3f(-1.0, -0.25, 1.0);
    glVertex3f(-1.0, -0.25, -1.0);
    glEnd();

    graphicAxes();
    glFlush();
    this->makeCurrent();
}

void QGLView::resizeGL(int width, int height)
{
    //int side = qMin(width, height);
    //glViewport((width - side) / 2, (height - side) / 2, side, side);
	glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}

void QGLView::graphicAxes()
{
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(20,0,0);

    glColor3f(1,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,20,0);

    glColor3f(0,1,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,20);
    glEnd();
}

void QGLView::setRotation(float x, float y, float z)
{
    xrot = x;
    yrot = y;
    zrot = z;
    update();
}
