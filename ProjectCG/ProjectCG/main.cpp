

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>

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
GLfloat        xC=10.0, yC=10.0, zC=10.0;        //.. Mundo  (unidades mundo)

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









//================================================================================
//=========================================================================== INIT
void inicializa(void)
{
    glClearColor(BLACK);        //������������������������������Apagar
    glEnable(GL_DEPTH_TEST);    //������������������������������Profundidade
    glShadeModel(GL_SMOOTH);    //������������������������������Interpolacao de cores
    
    glEnable(GL_CULL_FACE);        //������������������������������Faces visiveis
    glCullFace(GL_BACK);        //������������������������������Mostrar so as da frente
    
    glVertexPointer(3, GL_FLOAT, 0, vertices); //���������������Vertex arrays
    glEnableClientState(GL_VERTEX_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normais);
    glEnableClientState(GL_NORMAL_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, cores);
    glEnableClientState(GL_COLOR_ARRAY);
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




void drawScene(){
    
    //=================================================== Qual o lado ?
    if (frenteVisivel)
        glCullFace(GL_BACK);  //glFrontFace(GL_CW);
    else
        glCullFace(GL_FRONT);  //glFrontFace(GL_CCW);
    
    
    //==================================== MESA
    //glColorPointer(3, GL_FLOAT, 0, cor);     podia ser modificada a cor !
    glPushMatrix();
    //?? esquerda
    //?? direita
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
    glPopMatrix();
    
    
    //==================================== PAralelipipedo Amarelo
    glColor4f(YELLOW);
    glPushMatrix();
    //?? escala, rotacao, translacao ??
    glTranslated(0, 3, 0);
    glutWireTeapot(1);
    glPopMatrix();
    
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
    
    
    //-------------------------------------------------------------- observador
    gluLookAt(obsP[0], obsP[1], obsP[2], 0,0,0, 0, 1, 0);
    
    //����������������������������������������������������������Objectos/modelos
    drawEixos();
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
    glutCreateWindow ("Projeto Computação Gráfica 2019/2019 | João Figueiredo | Pedro Chicória");
    
    inicializa();
    
    glutSpecialFunc(teclasNotAscii);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    
    return 0;
}



