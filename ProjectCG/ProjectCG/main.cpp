

#include <stdlib.h>
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

GLfloat tamanho = 70.0;
GLfloat tamanhoY = 40.0;

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
GLfloat movimentoX, movimentoZ, movimentoNormal;

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

void drawTerrace (){
    //-------------------------Parede Direita----------

    glPushMatrix();
    glColor4f(WHITE);
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glVertex3i(tamanho, -tamanho, tamanho);
    glVertex3i(tamanho, tamanho, tamanho);
    glVertex3i(tamanho, tamanho, -tamanho);
    glVertex3i(tamanho, -tamanho, -tamanho);
    glEnd();
    glPopMatrix();
  
    
    
    //-------------------------Parede Esquerda----------
    
    
    
    glPushMatrix();
    glColor4f(WHITE);
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glVertex3i(-tamanho, -tamanho, tamanho);
    glVertex3i(-tamanho, tamanho, tamanho);
    glVertex3i(-tamanho, tamanho, -tamanho);
    glVertex3i(-tamanho, -tamanho, -tamanho);
    glEnd();
    glPopMatrix();
    
    
    
    //-------------------------Parede Atras----------
    

    
    

    glPushMatrix();
    glColor4f(WHITE);
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glVertex3i(-tamanho, tamanho, -tamanho);
    glVertex3i(tamanho, tamanho, -tamanho);
    glVertex3i(tamanho, -tamanho, -tamanho);
    glVertex3i(-tamanho, -tamanho, -tamanho);
    glEnd();
    glPopMatrix();
    
    
    
    
    //-------------------------Parede Frente----------
    
    glPushMatrix();
    glColor4f(WHITE);
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glVertex3i(-tamanho, tamanho, tamanho);
    glVertex3i(-tamanho , tamanho, tamanho);
    glVertex3i(-tamanho , -tamanho, tamanho);
    glVertex3i(-tamanho, -tamanho, tamanho);
    glEnd();
    glPopMatrix();
    
    
    
    
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
    glColor4f(GREEN);
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

void drawScene()
{
    drawSkybox();
    drawTerrace();
}

void display(void)
{
    
    //================================================================= Apaga ecran/profundidade
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    //================================================================= No modificar
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
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
