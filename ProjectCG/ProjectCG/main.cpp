

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
#include "RgbImage.h"

//--------------------------------- Definir cores
#define BLUE     0.0, 0.0, 1.0, 1.0
#define RED         1.0, 0.0, 0.0, 1.0
#define YELLOW     1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define PI         3.14159

//================================================================================
//===========================================================Variaveis e constantes

//------------------------------------------------------------ Sistema Coordenadas + objectos
GLint        wScreen=800, hScreen=600;        //.. janela (pixeis)
GLfloat        xC = 15.0, yC = 15.0, zC = 70.0;
//------------------------------------------------------------ Observador
GLfloat  rVisao=10, aVisao=0.5*PI, incVisao=0.05;
GLfloat  obsP[] ={rVisao*cos(aVisao), 3.0, rVisao*sin(aVisao)};
GLfloat  angZoom=90;
GLfloat  incZoom=3;


//=========================================================== FACES DA MESA
GLboolean   frenteVisivel=1;
static GLuint     cima[] = {8,11, 10,  9};
static GLuint     esquerda[]= {0, 1, 2, 3};
static GLuint     direita[ ]= {4, 7, 6, 5};;

//------------------------------------------------------------ Texturas
GLuint  texture[18];
RgbImage imag;

GLfloat tamanhoanhoY = 40.0;
GLfloat tam=2.0;
static GLfloat vertices[]={
    //�������������������������������������� x=tam (Esquerda)
    -tam,  -tam,  tam,    // 0
    -tam,   tam,  tam,    // 1
    -tam,   tam, -tam,    // 2
    -tam,  -tam, -tam,    // 3
    //�������������������� Direita
    tam,  -tam,  tam,    // 4
    tam,   tam,  tam,    // 5
    tam,   tam, -tam,    // 6
    tam,  -tam, -tam,    // 7
    //��������������������� (Cima
    -tam,  tam,  tam,    // 8
    -tam,  tam, -tam,    // 9
    tam,  tam, -tam,    // 10
    tam,  tam,  tam,    // 11
};

static GLfloat normais[] = {
    //�������������������������������������� x=tam (Esquerda)
    -1.0,  0.0,  0.0,
    -1.0,  0.0,  0.0,
    -1.0,  0.0,  0.0,
    -1.0,  0.0,  0.0,
    //�������������������������������������� x=tam (Direita)
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    //�������������������������������������� y=tam (Cima)
    0.0,  1.0,  0.0,
    0.0,  1.0,  0.0,
    0.0,  1.0,  0.0,
    0.0,  1.0,  0.0,
};
//------------------------------------------------------------ Cores
static GLfloat cores[]={
    //�������������������������������������� x=tam (Esquerda) - Red
    1.0,  0.0, 0.0,    // 0
    1.0,  0.0, 0.0,    // 1
    1.0,  1.0, 0.0,    // 2
    1.0,  1.0, 0.0,    // 3
    //�������������������������������������� x=2*tam (Direita) - Green
    0.0,  1.0, 1.0,    // 4
    0.0,  1.0, 1.0,    // 5
    0.0,  1.0, 0.0,    // 6
    0.0,  1.0, 0.0,    // 7
    //�������������������������������������� y=tam (Cima) - Blue
    0.0,  0.0, 1.0,    // 8
    0.0,  0.0, 1.0,    // 9
    1.0,  0.0, 1.0,    // 10
    1.0,  0.0, 1.0,    // 11
};




void criaDefineTexturas() {
    
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("/Users/pedrochicoria/Desktop/UC/LEI/CG/Project/ProjectCG/ProjectCG/ProjectCG/textures/Back.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("/Users/pedrochicoria/Desktop/UC/LEI/CG/Project/ProjectCG/ProjectCG/ProjectCG/textures/Down.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
   
    glGenTextures(1, &texture[2]);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("/Users/pedrochicoria/Desktop/UC/LEI/CG/Project/ProjectCG/ProjectCG/ProjectCG/textures/Front.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    glGenTextures(1, &texture[3]);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("/Users/pedrochicoria/Desktop/UC/LEI/CG/Project/ProjectCG/ProjectCG/ProjectCG/textures/Top.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    glGenTextures(1, &texture[4]);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("/Users/pedrochicoria/Desktop/UC/LEI/CG/Project/ProjectCG/ProjectCG/ProjectCG/textures/Right.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    glGenTextures(1, &texture[5]);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("/Users/pedrochicoria/Desktop/UC/LEI/CG/Project/ProjectCG/ProjectCG/ProjectCG/textures/Left.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
   
    
}




//================================================================================
//=========================================================================== INIT
void inicializa(void)
{
    glClearColor(WHITE);        //Apagar
    glEnable(GL_DEPTH_TEST);    //Profundidade
    glEnable(GL_BLEND);
    glShadeModel(GL_SMOOTH);
    criaDefineTexturas();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);// Habilita a definição da cor do material a partir da cor corrente
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}



void drawEixos()
{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo X
    glColor4f(RED);
    glBegin(GL_LINES);
    glVertex3i( 0, 0, 0);
    glVertex3i(10, 0, 0);
    glEnd();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Y
    glColor4f(GREEN);
    glBegin(GL_LINES);
    glVertex3i(0,  0, 0);
    glVertex3i(0, 10, 0);
    glEnd();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Z
    glColor4f(BLUE);
    glBegin(GL_LINES);
    glVertex3i( 0, 0, 0);
    glVertex3i( 0, 0,10);
    glEnd();
    
}

void drawSkybox() {
    
    
    //atrás
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glPushMatrix();
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(-800, 600, -250);
    glTexCoord2f(1.0f, 0.0f); glVertex3i(800, 600, -250);
    glTexCoord2f(1.0f, 1.0f); glVertex3i(800, -600, -250);
    glTexCoord2f(0.0f, 1.0f); glVertex3i(-800, -600, -250);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    //esquerda
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glPushMatrix();
    
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(-250, -600, 800);
    glTexCoord2f(1.0f, 0.0f); glVertex3i(-250, 600, 800);
    glTexCoord2f(0.0f, 1.0f); glVertex3i(-250, 600, -800);
    glTexCoord2f(1.0f, 1.0f); glVertex3i(-250, -600, -800);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    //direita
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glPushMatrix();
    
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(250, -600, 800);
    glTexCoord2f(1.0f, 0.0f); glVertex3i(250, 600, 800);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(250, 600, -800);
    glTexCoord2f(1.0f, 1.0f); glVertex3i(250, -600, -800);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    
    //chão
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glPushMatrix();
    
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(-800, -tamanhoanhoY - 10, -800);
    glTexCoord2f(1.0f, 0.0f); glVertex3i(-800, -tamanhoanhoY - 10, 800);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(800, -tamanhoanhoY - 10, 800);
    glTexCoord2f(1.0f, 1.0f); glVertex3i(800, -tamanhoanhoY - 10, -800);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    
    
    
    //Frente
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glPushMatrix();
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(-800, 600, 650);
    glTexCoord2f(1.0f, 0.0f); glVertex3i(800, 600, 650);
    glTexCoord2f(1.0f, 1.0f); glVertex3i(800, -600, 650);
    glTexCoord2f(0.0f, 1.0f); glVertex3i(-800, -600, 650);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    
    
    // -------------------------Ceu-------------------
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glPushMatrix();
    
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(-800, 250, 800);
    glTexCoord2f(1.0f, 0.0f); glVertex3i(-800, 250, -800);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(800, 250, -800);
    glTexCoord2f(1.0f, 1.0f); glVertex3i(800, 250, 800);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    
    
}


void drawScene(){
    
    //=================================================== Qual o lado ?
/*if (frenteVisivel)
        glCullFace(GL_BACK);  //glFrontFace(GL_CW);
    else
        glCullFace(GL_FRONT);  //glFrontFace(GL_CCW);
    */
    /*
    //==================================== MESA
    //glColorPointer(3, GL_FLOAT, 0, cor);     podia ser modificada a cor !
    glPushMatrix();
    //?? esquerda
    //?? direita
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
    glPopMatrix();
    */
    drawSkybox();
    /*
    //==================================== PAralelipipedo Amarelo
    glColor4f(YELLOW);
    glPushMatrix();
    //?? escala, rotacao, translacao ??
    glTranslated(0, 3, 0);
    glutWireTeapot(1);
    glPopMatrix();*/
    
}

void display(void){
    
    //================================================================= APaga ecran/profundidade
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    
    //================================================================= NAO MOFIFICAR
    glViewport (0, 0, wScreen, hScreen);                                // ESQUECER PoR AGORA
    glMatrixMode(GL_PROJECTION);                                        // ESQUECER PoR AGORA
    glLoadIdentity();                                                    // ESQUECER PoR AGORA
    gluPerspective(angZoom, (float)wScreen/hScreen, 0.1, 3*zC);            // ESQUECER PoR AGORA
    glMatrixMode(GL_MODELVIEW);                                            // ESQUECER PoR AGORA
    glLoadIdentity();                                                    // ESQUECER PoR AGORA
    //================================================================= NAO MOFIFICAR
    
    glRotatef(aVisao, 0, 1, 0);
   
    //-------------------------------------------------------------- observador
    gluLookAt(obsP[0], obsP[1], obsP[2], 0,0,0, 0, 1, 0);
    glEnable(GL_NORMALIZE);
    //����������������������������������������������������������Objectos/modelos
    //drawEixos();
    drawScene();
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Actualizacao
    glutSwapBuffers();
}


//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y){
    
    
    switch (key) {
        case 'f':
        case 'F':
            frenteVisivel=!frenteVisivel;
            glutPostRedisplay();
            break;
            
        case 'A':
        case 'a':
            //??
            glutPostRedisplay();
            break;
            
        case 'S':
        case 's':
            //??
            glutPostRedisplay();
            break;
            
        case 'e':
        case 'E':
            //??
            glutPostRedisplay();
            break;
            
        case 'd':
        case 'D':
            //??
            glutPostRedisplay();
            break;
            
            
            
            
            
            //--------------------------- Escape
        case 27:
            exit(0);
            break;
    }
    
}



void teclasNotAscii(int key, int x, int y){
    
    if(key == GLUT_KEY_UP)
        obsP[1] = (obsP[1]+ 0.1) ;
    if(key == GLUT_KEY_DOWN)
        obsP[1] = (obsP[1]- 0.1) ;
    
    if (obsP[1]>yC)
        obsP[1]=yC;
    if (obsP[1]<-yC)
        obsP[1]=-yC;
    
    if(key == GLUT_KEY_LEFT)
        aVisao = (aVisao + 0.1) ;
    if(key == GLUT_KEY_RIGHT)
        aVisao = (aVisao - 0.1) ;
    
    obsP[0]=rVisao*cos(aVisao);
    obsP[2]=rVisao*sin(aVisao);
    
    
    glutPostRedisplay();
    
}


//======================================================= MAIN
//======================================================= MAIN
int main(int argc, char** argv){
    
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize (wScreen, hScreen);
    glutInitWindowPosition (300, 100);
    glutCreateWindow ("Projeto Computação Gráfica 2018/2019");
    
    inicializa();
    
    glutSpecialFunc(teclasNotAscii);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    
    return 0;
}



