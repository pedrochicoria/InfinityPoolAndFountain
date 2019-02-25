#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

GLfloat angle,angle2;
int moving,startx,starty;
void wall(double thickness)
{
    glPushMatrix();
    glTranslated(0.5,0.5*thickness,0.5);
    glScaled(1.0,thickness,1.0);
    glutSolidCube(1.0);
    glPopMatrix();
}
void myinit()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100,100,-100,100,-100,100);
    glMatrixMode(GL_MODELVIEW);
}
void displaySolid(void)
{
    GLfloat mat_ambient[] = { 0.0f,1.0f,0.0f,1.0f};// gray
    GLfloat mat_diffuse[] = { 0.0f,1.0f,0.0f,1.0f};
    GLfloat mat_specular[] = { 1.0f,1.0f,1.0f,1.0f};
    GLfloat mat_shininess[]={50.0f};
    
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
    
    // set the light sources propertis
    GLfloat lightIntensity[] ={ 1.0f,1.0f,1.0f,1.0f};
    GLfloat light_position[] ={ 2.0f,6.0f,3.0f,0.0f};
    glLightfv(GL_LIGHT0 ,GL_POSITION,light_position);
    glLightfv(GL_LIGHT0 ,GL_DIFFUSE,lightIntensity);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.33,1.33, -1,1,-100,100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(2.3,1.3,2.0,0.0,0.25,0.0,0.0,1.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(angle,1,0,0);
    glRotatef(angle2,0,1,0);
    glPushMatrix();
    glTranslated(0.4,0,0.4);
    glColor3f(0.0,1.0,1.0);
    glPopMatrix();
    glPushMatrix();
    mat_ambient[2]= 1.0f;
    mat_ambient[1]= 0.7f;// gray
    mat_diffuse[2]= 1.0f;
    mat_diffuse[1]= 0.7f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glTranslated(0.6,0.38,0.5);
    glRotated(30,0,1,0);
    glColor3f(1.0,1.0,0.0);
    glPopMatrix();
    mat_ambient[0]= 1.0f;
    mat_ambient[1]= 0.0f;
    mat_ambient[2]= 0.0f;// gray
    mat_diffuse[0]= 1.0f;
    mat_diffuse[1]= 0.0f;
    mat_diffuse[2]= 0.0f;
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    
    wall(0.02);
    
    glPushMatrix();
    glColor3f(0.0,1.0,0.0);
    glRotated(90.0,0.0,0.0,1.0);
    wall(0.02);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0.0,1.0,0.0);
    glRotated(0.0,90.0,90.0,1.0);
    wall(0.02);
    glPopMatrix();
    
    glPushMatrix();
    glRotated(-90.0,1.0,0.0,0.0);
    glColor3f(0.0,0.0,1.0);
    wall(0.02);
    glPopMatrix();
    
    glFlush();
    glutSwapBuffers();
}
void mouse(int btn,int state,int x,int y)
{
    if(btn==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
    {
        moving=1;
        startx=x;
        starty=y;
    }
    if(btn==GLUT_LEFT_BUTTON&&state==GLUT_UP)
    {
        moving=0;
    }
}
void motion(int x,int y)
{
    if(moving)
    {
        angle=angle+(x-startx);
        angle2=angle2+(y-starty);
        startx=x;
        starty=y;
        glutPostRedisplay();
    }
}
int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(1920,1080);
    glutInitWindowPosition(400,400);
    glutCreateWindow("simple shaded scene consisting of a tea pot on a table ");
    glutDisplayFunc(displaySolid);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1,0.1,0.1,0.0);
    glViewport(0,0,640,480);
    myinit();
    glutMainLoop();
    return 0;
}
