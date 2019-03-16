
// run with g++ -o main main.cpp -framework OpenGL -framework GLUT

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>

//--------------------------------- Definir cores
#define BLUE 0.0, 0.0, 1.0, 1.0
#define RED 1.0, 0.0, 0.0, 1.0
#define YELLOW 1.0, 1.0, 0.0, 1.0
#define GREEN 0.0, 1.0, 0.0, 1.0
#define WHITE 1.0, 1.0, 1.0, 1.0
#define BLACK 0.0, 0.0, 0.0, 1.0
#define PI 3.14159

using namespace std;

GLfloat tamanho = 70.0;
GLfloat tamanhoY = 32.5;
GLfloat heightstairs = 10;
GLfloat widthstairs = 10;

//------------------------------------------------------------ Sistema Coordenadas + objectos
GLint wScreen = 800, hScreen = 600; //.. janela (pixeis)
GLuint height = 600, width = 600;
GLfloat xC = 15.0, yC = 15.0, zC = 70.0; //.. Mundo  (unidades mundo)

//------------------------------------------------------------ Observador
GLfloat rVisao = 20, aVisao = PI + PI / 2, incVisao = 0.05;
GLfloat obsP[] = {0, -20, 20}; //estava a 320
GLfloat obsF[] = {0.0, -20, 0.0};
GLfloat angZoom = 80;
GLfloat incZoom = 3;
GLuint movement = -1; //-1 para fora do edificio | 0  o res de chão | 1 para o primeiro movement
GLfloat movimentoX, movimentoY, movimentoZ, movimentoNormal;

void criaDefineTexturas();
void drawSkybox();

// Função responsável pela especificação dos parâmetros de iluminação
void init(void)
{
    glClearColor(WHITE);     //Apagar
    glEnable(GL_DEPTH_TEST); //Profundidade
    glEnable(GL_BLEND);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_COLOR_MATERIAL); // Habilita a definição da cor do material a partir da cor corrente
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
void drawTerrace()
{
    glTranslated(50, -10, -45);
    //Lado multicolorido - FRENTE
    glBegin(GL_POLYGON);
    glColor4f(0.8,0.8,0.8,1);
    glVertex3f(tamanho, -tamanhoY, -tamanho);
    glVertex3f(tamanho, tamanhoY, -tamanho);
    glVertex3f(-tamanho + 70, tamanhoY, -tamanho);
    glVertex3f(-tamanho + 70, -tamanhoY, -tamanho);
    glEnd();

    glBegin(GL_POLYGON);
     glColor4f(0.8,0.8,0.8,1);
    glVertex3f(0, -tamanhoY, -tamanho);
    glVertex3f(0, 0, -tamanho);
    glVertex3f(-tamanho, 0, -tamanho);
    glVertex3f(-tamanho, -tamanhoY, -tamanho);
    glEnd();

    // Lado branco - TRASEIRA
    glBegin(GL_POLYGON);
     glColor4f(0.8,0.8,0.8,1);
    glVertex3f(tamanho, -tamanhoY, tamanho);
    glVertex3f(tamanho, tamanhoY, tamanho);
    glVertex3f(-tamanho, tamanhoY, tamanho);
    glVertex3f(-tamanho, -tamanhoY, tamanho);
    glEnd();

    // Lado roxo - DIREITA
    glBegin(GL_POLYGON);
 glColor4f(0.8,0.8,0.8,1);
     glVertex3f(tamanho, -tamanhoY, -tamanho);
    glVertex3f(tamanho, tamanhoY, -tamanho);
    glVertex3f(tamanho, tamanhoY, tamanho);
    glVertex3f(tamanho, -tamanhoY, tamanho);
    glEnd();

    // Lado verde - ESQUERDA
    glBegin(GL_POLYGON);
     glColor4f(0.8,0.8,0.8,1);
    glVertex3f(-tamanho, -tamanhoY, tamanho);
    glVertex3f(-tamanho, tamanhoY, tamanho);
    glVertex3f(-tamanho, tamanhoY, -tamanho + 70);
    glVertex3f(-tamanho, -tamanhoY, -tamanho + 70);
    glEnd();

    glBegin(GL_POLYGON);
     glColor4f(0.8,0.8,0.8,1);
    glVertex3f(-tamanho, -tamanhoY, 0);
    glVertex3f(-tamanho, 0, 0);
    glVertex3f(-tamanho, 0, -tamanho);
    glVertex3f(-tamanho, -tamanhoY, -tamanho);
    glEnd();

    // Lado azul - TOPO
    glBegin(GL_POLYGON);
    glColor4f(0.8,0.8,0.8,1);
    glVertex3f(tamanho, tamanhoY, tamanho);
    glVertex3f(tamanho, tamanhoY, -tamanho);
    glVertex3f(-tamanho + 70, tamanhoY, -tamanho);
    glVertex3f(-tamanho + 70, tamanhoY, tamanho);
    glEnd();

    glBegin(GL_POLYGON);
     glColor4f(0.8,0.8,0.8,1);
    glVertex3f(0, tamanhoY, tamanho);
    glVertex3f(0, tamanhoY, -tamanho + 70);
    glVertex3f(-tamanho, tamanhoY, -tamanho + 70);
    glVertex3f(-tamanho, tamanhoY, tamanho);
    glEnd();

    // Lado vermelho - BASE
    glBegin(GL_POLYGON);
    glColor4f(0.8,0.8,0.8,1);
    glVertex3f(tamanho, -tamanhoY, -tamanho);
    glVertex3f(tamanho, -tamanhoY, tamanho);
    glVertex3f(-tamanho, -tamanhoY, tamanho);
    glVertex3f(-tamanho, -tamanhoY, -tamanho);
    glEnd();
}

void drawPool()
{
    //INFINITY POOL FRONT
    glBegin(GL_POLYGON);
    glColor4f(BLUE);
    glVertex3f(0, -tamanhoY + tamanhoY, -tamanho);
    glVertex3f(0, tamanhoY, -tamanho);
    glVertex3f(-tamanho, -tamanho + tamanhoY, -tamanho);
    glVertex3f(-tamanho, tamanhoY, -tamanho);
    glEnd();

    //LEFT SIDE
    glBegin(GL_POLYGON);
    glColor4f(BLUE);
    glVertex3f(-tamanho, -tamanhoY + tamanhoY, 0);
    glVertex3f(-tamanho, tamanhoY, 0);
    glVertex3f(-tamanho, tamanhoY, -tamanho);
    glVertex3f(-tamanho, 0, -tamanho);
    glEnd();

    //TOP SIDE
    glBegin(GL_POLYGON);
    glColor4f(BLUE);
    glVertex3f(0, tamanhoY, tamanho - 70);
    glVertex3f(0, tamanhoY, -tamanho);
    glVertex3f(-tamanho, tamanhoY, -tamanho);
    glVertex3f(-tamanho, tamanhoY, tamanho - 70);
    glEnd();
}

void drawSkybox()
{

    //atrás

    glPushMatrix();
    glColor4f(BLUE);
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glVertex3i(-800, 600, -250);
    glVertex3i(800, 600, -250);
    glVertex3i(800, -600, -250);
    glVertex3i(-800, -600, -250);
    glEnd();
    glPopMatrix();

    //esquerda

    glPushMatrix();
    glColor4f(BLUE);
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glVertex3i(-250, -600, 800);
    glVertex3i(-250, 600, 800);
    glVertex3i(-250, 600, -800);
    glVertex3i(-250, -600, -800);
    glEnd();
    glPopMatrix();

    //direita

    glPushMatrix();
    glColor4f(BLUE);
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glVertex3i(250, -600, 800);
    glVertex3i(250, 600, 800);
    glVertex3i(250, 600, -800);
    glVertex3i(250, -600, -800);
    glEnd();
    glPopMatrix();

    //chão

    glPushMatrix();
    glColor4f(WHITE);
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glVertex3i(-800, -tamanhoY - 10, -800);
    glVertex3i(-800, -tamanhoY - 10, 800);
    glVertex3i(800, -tamanhoY - 10, 800);
    glVertex3i(800, -tamanhoY - 10, -800);
    glEnd();
    glPopMatrix();

    //Frente

    glPushMatrix();
    glColor4f(BLUE);
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glVertex3i(-800, 600, 650);
    glVertex3i(800, 600, 650);
    glVertex3i(800, -600, 650);
    glVertex3i(-800, -600, 650);
    glEnd();
    glPopMatrix();

    // -------------------------Ceu-------------------

    glPushMatrix();
    glColor4f(BLUE);
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glVertex3i(-800, 250, 800);
    glVertex3i(-800, 250, -800);
    glVertex3i(800, 250, -800);
    glVertex3i(800, 250, 800);
    glEnd();
    glPopMatrix();
}

void drawSteps()
{
     glColor4f(0.8,0.8,0.8,1);

    // Lado branco - TRASEIRA
    glBegin(GL_POLYGON);

    glVertex3f(heightstairs, -heightstairs, heightstairs);
    glVertex3f(heightstairs, heightstairs, heightstairs);
    glVertex3f(-heightstairs, heightstairs, heightstairs);
    glVertex3f(-heightstairs, -heightstairs, heightstairs);
    glEnd();

    // Lado roxo - DIREITA
    glBegin(GL_POLYGON);

    glVertex3f(heightstairs, -heightstairs, -heightstairs);
    glVertex3f(heightstairs, heightstairs, -heightstairs);
    glVertex3f(heightstairs, heightstairs, heightstairs);
    glVertex3f(heightstairs, -heightstairs, heightstairs);
    glEnd();

    // Lado verde - ESQUERDA
    glBegin(GL_POLYGON);

    glVertex3f(-heightstairs, -heightstairs, heightstairs);
    glVertex3f(-heightstairs, heightstairs, heightstairs);
    glVertex3f(-heightstairs, heightstairs, -heightstairs);
    glVertex3f(-heightstairs, -heightstairs, -heightstairs);
    glEnd();

    // Lado azul - TOPO
    glBegin(GL_POLYGON);

    glVertex3f(heightstairs, heightstairs, heightstairs);
    glVertex3f(heightstairs, heightstairs, -heightstairs);
    glVertex3f(-heightstairs, heightstairs, -heightstairs);
    glVertex3f(-heightstairs, heightstairs, heightstairs);
    glEnd();

    // Lado vermelho - BASE
    glBegin(GL_POLYGON);

    glVertex3f(heightstairs, -heightstairs, -heightstairs);
    glVertex3f(heightstairs, -heightstairs, heightstairs);
    glVertex3f(-heightstairs, -heightstairs, heightstairs);
    glVertex3f(-heightstairs, -heightstairs, -heightstairs);
    glEnd();
}

void drawStairs()
{
    GLdouble yscale = 0.5;
    glPushMatrix();
    glTranslated((-20), -tamanhoY, -tamanho - tamanho-2);
    glScaled(0.5, yscale, 2);

    drawSteps();
    glPopMatrix();

    glPushMatrix();
    glTranslated((-20) + heightstairs, -tamanhoY, -tamanho - tamanho-2);
    

    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(0.5, yscale + 0.5, 2);
    drawSteps();

    glPopMatrix();

    glPushMatrix();
    glTranslated((-20) + heightstairs + 10, -tamanhoY, -tamanho - tamanho-2);

    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(0.5, yscale + 1, 2);
    drawSteps();

    glPopMatrix();

    glPushMatrix();
    glTranslated((-20) + heightstairs + 20, -tamanhoY, -tamanho - tamanho-2);

    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(0.5, yscale + 1.5, 2);
    drawSteps();

    glPopMatrix();

    glPushMatrix();
    glTranslated((-20 )+ heightstairs + 30, -tamanhoY, -tamanho - tamanho-1);

    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(0.5, yscale + 2, 2);
    drawSteps();

    glPopMatrix();

    glPushMatrix();
    glTranslated((-20) + heightstairs + 55, -tamanhoY, -tamanho - tamanho-1);

    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(2, yscale + 2.5, 2);
    drawSteps();

    glPopMatrix();


    glPushMatrix();
    glTranslated((-20) + heightstairs + 55, -tamanhoY, -tamanho - tamanho-heightstairs + 25);

    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(2, yscale +3 , 0.5);
    drawSteps();

    glPopMatrix();
    glPushMatrix();
    glTranslated((-20) + heightstairs + 55, -tamanhoY, -tamanho - tamanho-heightstairs + 35);

    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(2, yscale +3.5 , 0.5);
    drawSteps();

    glPopMatrix();

 glPushMatrix();
    glTranslated((-20) + heightstairs + 55, -tamanhoY, -tamanho - tamanho-heightstairs + 45);

    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(2, yscale +4 , 0.5);
    drawSteps();

    glPopMatrix();
    glPushMatrix();
    glTranslated((-20) + heightstairs + 55, -tamanhoY, -tamanho - tamanho-heightstairs + 55);

    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(2, yscale +4.5 , 0.5);
    drawSteps();

    glPopMatrix();

     glPushMatrix();
    glTranslated((-20 ) + heightstairs + 55, -tamanhoY, -tamanho - tamanho-heightstairs + 65);

    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(2, yscale +5 , 0.5);
    drawSteps();

    glPopMatrix();

     glPushMatrix();
    glTranslated((-20) + heightstairs + 55, -tamanhoY, -tamanho - tamanho-heightstairs + 75);

    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(2, yscale +5.5 , 0.5);
    drawSteps();

    glPopMatrix();
}

void drawScene()
{
    drawSkybox();
    drawTerrace();
    drawPool();
    //glPushMatrix();
   // glTranslated((tamanho /4), -tamanhoY, -tamanho - tamanho);
    drawStairs();
    //glPopMatrix();
}

void display(void)
{

    //================================================================= Apaga ecran/profundidade
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //================================================================= No modificar

    glViewport(0, 0, wScreen, hScreen);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angZoom, (float)wScreen / hScreen, 0.1, 10 * zC);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(aVisao, 0, 1, 0);
    gluLookAt(obsP[0], obsP[1], obsP[2], obsF[0], obsF[1], obsF[2], 0, 1, 0);

    glEnable(GL_NORMALIZE);
    //codigo baseado no trabalho de reflexoes de CG

    drawScene();

    //Actualizacao
    glutSwapBuffers();
}

//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y)
{

    switch (key)
    {

    case 'S':
    case 's':
        movimentoX = obsF[0] - obsP[0];
        movimentoZ = obsF[2] - obsP[2];
        movimentoNormal = sqrt(pow(movimentoX, 2) + pow(movimentoZ, 2));
        movimentoX = movimentoX / movimentoNormal;
        movimentoZ = movimentoZ / movimentoNormal;
        obsP[0] = obsP[0] - movimentoX;
        obsP[2] = obsP[2] - movimentoZ;
        obsF[0] = obsF[0] - movimentoX;
        obsF[2] = obsF[2] - movimentoZ;
        glutPostRedisplay();
        break;

    case 'w':
    case 'W':
        movimentoX = obsF[0] - obsP[0];
        movimentoZ = obsF[2] - obsP[2];

        movimentoNormal = sqrt(pow(movimentoX, 2) + pow(movimentoZ, 2));
        movimentoX = movimentoX / movimentoNormal;
        movimentoZ = movimentoZ / movimentoNormal;

        obsP[0] = obsP[0] + movimentoX * 2;
        obsP[2] = obsP[2] + movimentoZ * 2;
        obsF[0] = obsF[0] + movimentoX * 2;
        obsF[2] = obsF[2] + movimentoZ * 2;

        glutPostRedisplay();
        break;
        //--------------------------- Escape
    case 27:
        exit(0);
        break;
    }
}
void idle(void)
{
    glutPostRedisplay();
}

void teclasNotAscii(int key, int x, int y)
{

    if (key == GLUT_KEY_LEFT)
        aVisao = (aVisao - 0.05);
    if (aVisao > 2 * PI)
        aVisao = 0;
    if (key == GLUT_KEY_RIGHT)
        aVisao = (aVisao + 0.05);

    if (aVisao < -2 * PI)
        aVisao = 0;

    obsF[0] = obsP[0] + rVisao * cos(aVisao);
    obsF[2] = obsP[2] + rVisao * sin(aVisao);

    glutPostRedisplay();
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(wScreen, hScreen);
    glutInitWindowPosition(300, 100);
    glutCreateWindow(" Projeto Computação Gráfica ");
    init();
    glutSpecialFunc(teclasNotAscii); //para as setas
    glutDisplayFunc(display);        //display callback
    glutKeyboardFunc(keyboard);      //para as letras
    glutIdleFunc(display);

    glutMainLoop();

    return 0;
}
