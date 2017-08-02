#include "widget.h"
#include <QMouseEvent>
#include <GL/glaux.h>
#include <math.h>

Widget::Widget(QWidget *p):QGLWidget(p)
{
    QObject::connect(&timer, &QTimer::timeout, this, &Widget::recalcCoords);
    timer.start(100);
}

void Widget::recalcCoords()
{
    auto delta = 0.01;
    zTimer -= delta;

    if(zTimer < -flatSize*2 + lineSize*2){
        zTimer = 0;
        xTimer = 0;
        lines.clear();
    }

    xTimer = flatSize*sin(zTimer)/2;
    alpha =  zTimer*180/3.14;

    zCamera = zTimer + flatSize -  sin(zTimer) * 0.1;
    xCamera = xTimer + cos(zTimer) * 0.1;
    yCamera = yTimer - flatSize + 0.1;

    updateGL();
}

void Widget::initializeGL()
{
    qglClearColor(Qt ::black);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0,0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1,-1, 1, 1, 10);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-3);

    glRotatef(m_xRotate, 1, 0, 0);
    glRotatef(m_yRotate, 0, 1, 0);

    gluLookAt(xCamera, yCamera, zCamera, xTimer, yTimer- flatSize, zTimer+ flatSize, 0, 1, 0);

    createFlat();
    drawTrack();
    drawLine(xTimer, yTimer , zTimer);

}

void Widget::mousePressEvent(QMouseEvent *pe)
{
    m_ptPosition = pe->pos();
}

void Widget::mouseMoveEvent(QMouseEvent *pe)
{
    m_xRotate += 180*(GLfloat)(pe->y() - m_ptPosition.y())/height();
    m_yRotate += 180*(GLfloat)(pe->x() - m_ptPosition.x())/width();

    updateGL();
    m_ptPosition = pe->pos();
}

void Widget::createFlat( )
{
    glBegin(GL_QUADS);
        qglColor(Qt::green);

        glVertex3f(-flatSize, -flatSize , flatSize);
        glVertex3f(flatSize, -flatSize , flatSize );
        glVertex3f(flatSize , -flatSize , -flatSize );
        glVertex3f(-flatSize , -flatSize, -flatSize );
    glEnd();
}

void Widget::drawLine(double x, double y, double z)
{
    z -= lineSize;

    auto x1 = x;
    auto z1 = z + flatSize;

    auto x2 = x + lineSize*sin(alpha*3.14/180);
    auto z2 = z + lineSize*cos(alpha*3.14/180)+ flatSize;

    lines.push_back(Line(x1, - flatSize + 0.01,  z1, x2, - flatSize + 0.01 , z2));

    GLUquadricObj *quadObj;
    quadObj = gluNewQuadric();

    qglColor(Qt::red);
    gluQuadricDrawStyle(quadObj, GLU_FILL);

    glTranslated(x , y - flatSize , z + flatSize );

    glRotated(alpha, 0,1,0);
    glColor3d(1,0,0);
    gluCylinder(quadObj, 0.01, 0.01, lineSize, 15, 15);

    gluDeleteQuadric(quadObj);
}

void Widget::drawTrack()
{
    glLineWidth(5);
    glBegin(GL_LINES);
    qglColor(Qt::blue);

    for(auto line:lines) {
        glVertex3f(line.x1 , line.y1 , line.z1 );
        glVertex3f(line.x2 , line.y2 , line.z2 );
    }
    glEnd();
}
