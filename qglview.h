#ifndef QGLVIEW_H
#define QGLVIEW_H

// www.amin-ahmadi.com

#include <QtWidgets>

class QGLView : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit QGLView(QWidget *parent = 0);

    void setRotation(float x, float y, float z);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

signals:

public slots:

private:
    float xrot, yrot, zrot;

};

#endif // QGLVIEW_H
