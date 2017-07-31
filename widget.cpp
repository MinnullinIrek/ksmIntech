#include "widget.h"
#include <QMouseEvent>
#include <GL/glaux.h>
#include <GL/gl.h>
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


    updateGL();
}

void Widget::initializeGL()
{
    qglClearColor(Qt ::black);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    m_nFlat = createFlat();
    /*
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    float sp[4] = {1,1,1,1};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT3);
        glEnable(GL_LIGHT5);
        glEnable(GL_LIGHT6);
    glLightfv(GL_LIGHT3, GL_SPECULAR, sp);
    glLightfv(GL_LIGHT5, GL_SPECULAR, sp);
    glLightfv(GL_LIGHT6, GL_SPECULAR, sp);


    float mat_specular[] = {1,1,1,1};
    float pos[4] = {3,3,3,1};
    float color[4] = {1,1,1,1};

    color[1]=color[2]=0;
     glLightfv(GL_LIGHT3, GL_DIFFUSE, color);
     color[0]=0;
         color[1]=1;
     glLightfv(GL_LIGHT5, GL_DIFFUSE, color);
     color[1]=0;
         color[2]=1;
     glLightfv(GL_LIGHT6, GL_DIFFUSE, color);
     glLightfv(GL_LIGHT3, GL_POSITION, pos);
         pos[0] = -3;
     glLightfv(GL_LIGHT5, GL_POSITION, pos);
         pos[0]=0;pos[1]=-3;
     glLightfv(GL_LIGHT6, GL_POSITION, pos);
     glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
     glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
*/
    float ambience[4] = {1.0f, 1.0f, 1.0f, 0.0};
      float diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0};
      glLightfv(GL_LIGHT0, GL_AMBIENT,  ambience);
      glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);

      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
      glEnable(GL_COLOR_MATERIAL);
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

    glCallList(m_nFlat);

    drawTrack();

    drawLine(xTimer, yTimer, zTimer);

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

GLuint Widget::createFlat( )
{
    GLuint n = glGenLists(1);

    glNewList(n, GL_COMPILE);
        glBegin(GL_QUADS);
            qglColor(Qt::green);
            glVertex3f(-flatSize, -flatSize, flatSize);
            glVertex3f(flatSize, -flatSize, flatSize);
            glVertex3f(flatSize, -flatSize, -flatSize);
            glVertex3f(-flatSize, -flatSize, -flatSize);

        glEnd();
    glEndList();
    return n;
}

GLuint Widget::createCylinder(GLfloat fsize )
{


    double delta = 0.1;



    GLuint n = glGenLists(1);
    glNewList(n, GL_COMPILE);
        glBegin(GL_LINES);
         qglColor(Qt::red);
         glVertex3f(-lineSize/2, -fsize + delta, fsize );
         glVertex3f(lineSize/2, -fsize + delta, fsize);
        glEnd();
    glEndList();


return n;
}
void Widget::drawLine(double x, double y, double z)
{
    z -= lineSize;

    //lines.push_back(Line(x-lineSize/2, - flatSize + 0.01,  z + flatSize, x+lineSize/2, - flatSize + 0.01 , z + flatSize));

    auto x1 = x;
    auto z1 = z+ flatSize;

    auto x2 = x + lineSize*sin(alpha*3.14/180);
    auto z2 = z + lineSize*cos(alpha*3.14/180)+ flatSize;

    lines.push_back(Line(x1, - flatSize + 0.01,  z1, x2, - flatSize + 0.01 , z2));


    GLUquadricObj *quadObj;
    quadObj = gluNewQuadric();

    qglColor(Qt::red);
    gluQuadricDrawStyle(quadObj, GLU_FILL);


    glPointSize(3);
    glBegin(GL_POINTS);
       glColor3d(1,1,0);
       glVertex3d(x, - flatSize, z + flatSize); // первая точка

    glEnd();

    glTranslated(x, y - flatSize, z + flatSize);

    glRotated(alpha, 0,1,0);
    glColor3d(1,0,0);
    //glRotated(alpha, 0,1,0);
    gluCylinder(quadObj, 0.05, 0.05, lineSize, 15, 15);


    gluDeleteQuadric(quadObj);




    /*
    lines.push_back(Line(x-lineSize/2, y - flatSize ,  z + flatSize, x+lineSize/2, y - flatSize , z + flatSize));

    glLineWidth(2);
    glBegin(GL_LINES);
     qglColor(Qt::red);
     glVertex3f(x-lineSize/2, y - flatSize + delta, z + flatSize );
     glVertex3f(x+lineSize/2, y - flatSize + delta, z + flatSize );
    glEnd();
    */
}

void Widget::drawTrack()
{
    glLineWidth(3);
    glBegin(GL_LINES);
     qglColor(Qt::blue);

     for(auto line:lines)
     {
        glVertex3f(line.x1, line.y1, line.z1);
        glVertex3f(line.x2, line.y2, line.z2);
     }
   glEnd();


}
