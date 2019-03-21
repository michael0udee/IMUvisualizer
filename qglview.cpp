#include "qglview.h"

// based on the code from 'www.amin-ahmadi.com'

QGLView::QGLView(QWidget *parent) : QOpenGLWidget(parent)
{
    xrot = yrot = zrot = 0.0;
}

void QGLView::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);

    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
}

void QGLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -7.0);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

    glBegin(GL_QUADS);
    glNormal3d(0,0,+1);
    glColor3d(1, 0, 1);
    glVertex3d(-1,-1,0);
    glVertex3d(-1,1,0);
    glVertex3d(1,1,0);
    glVertex3d(1,-1,0);
    glEnd();

}

void QGLView::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}

void QGLView::setRotation(float x, float y, float z)
{
    xrot = x;
    yrot = y;
    zrot = z;
    update();
}
