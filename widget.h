#ifndef Widget_H
#define Widget_H

#include <QGLWidget>
#include <QTimer>

struct Line
{
    Line(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2):
        x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2)
    {

    }

    GLfloat x1, y1, z1;
    GLfloat x2, y2, z2;

};

class Widget : public QGLWidget
{
    Q_OBJECT
private:
    GLuint m_nFlat;
    GLuint m_nCylinder;

    GLfloat m_xRotate = 0;
    GLfloat m_yRotate = 0;
    QPoint m_ptPosition;

    GLfloat flatSize = 1.2f;
    GLfloat lineSize = 0.2f;

    GLfloat xTimer = 0;
    GLfloat yTimer = 0.05;
    GLfloat zTimer = 0;
    double alpha = 0;
    QTimer timer;

    QList<Line> lines;

protected:
    virtual void initializeGL() override;
    virtual  void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual void mousePressEvent(QMouseEvent *pe) override;
    virtual void mouseMoveEvent(QMouseEvent *pe) override;

    GLuint createFlat();

    GLuint createCylinder(GLfloat fsize = 1.0f);

    void drawLine(double x, double y, double z);
    void drawTrack();

public:
    Widget(QWidget *p = nullptr);
public slots:
    void recalcCoords();
};

#endif
