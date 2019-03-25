#ifndef QGLVIEW_H
#define QGLVIEW_H

// www.amin-ahmadi.com

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class QGLView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit QGLView(QWidget *parent = 0);

    void setRotation(float x, float y, float z);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void graphicAxes();

signals:

public slots:

private:
    float xrot, yrot, zrot;

};

#endif // QGLVIEW_H
