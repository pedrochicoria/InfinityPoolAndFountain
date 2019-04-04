
// run with g++ -o main main.cpp -framework OpenGL -framework GLUT

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>

#include "RgbImage.h"

//--------------------------------- Definir cores
#define BLUE 0.0, 0.0, 1.0, 1.0
#define RED 1.0, 0.0, 0.0, 1.0
#define YELLOW 1.0, 1.0, 0.0, 1.0
#define GREEN 0.0, 1.0, 0.0, 1.0
#define WHITE 1.0, 1.0, 1.0, 1.0
#define BLACK 0.0, 0.0, 0.0, 1.0
#define PI 3.14159
#define MAX_PARTICLES 1000

using namespace std;

typedef struct {
    bool alive;
    float life;
    float fade;
    
    float red;
    float green;
    float blue;
    
    float xpos;
    float ypos;
    float zpos;
    
    float vel;
    float gravity;
}particles;

particles par_sys[MAX_PARTICLES];
int chuva = 0;
float slowdown = 2.0;
float velocity = 3.0;


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
GLfloat obsP[] = {-200, 0, -200}; //estava a 320
GLfloat obsF[] = {0.0, 0.0, 0.0};
GLfloat angZoom = 80;
GLfloat incZoom = 3;
GLuint movement = -1; //-1 para fora do edificio | 0  o res de chão | 1 para o primeiro movement
GLfloat movimentoX, movimentoY, movimentoZ, movimentoNormal;


GLuint  texture[10];
GLuint  tex;
RgbImage imag;

void createAndDefineTextures();
void drawSkybox();

// Função responsável pela especificação dos parâmetros de iluminação
void init(void)
{
    glClearColor(WHITE);     //Apagar
    glEnable(GL_DEPTH_TEST); //Profundidade
    glEnable(GL_BLEND);
    glShadeModel(GL_SMOOTH);
    createAndDefineTextures();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL); // Habilita a definição da cor do material a partir da cor corrente
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void initParticles(int i) {
    par_sys[i].alive = true;
    par_sys[i].life = 1.0;
    par_sys[i].fade = float(rand() % 100) / 1000.0f + 0.003f;
    
    
    par_sys[i].xpos = (float)(rand() % 16);
    par_sys[i].ypos = 15.0;
    par_sys[i].zpos = (float)(rand() % 16);
    
    par_sys[i].red = 0.5;
    par_sys[i].green = 0.5;
    par_sys[i].blue = 1.0;
    
    par_sys[i].vel = velocity;
    par_sys[i].gravity = -0.8;
    
}
void drawRain() {
    float x, y, z;
    for (int loop = 0; loop < MAX_PARTICLES; loop = loop + 1) {
        if (par_sys[loop].alive == true) {
            x = par_sys[loop].xpos;
            y = par_sys[loop].ypos;
            z = par_sys[loop].zpos;
            
            // Draw particles
            glColor3f(0.5, 0.5, 1.0);
            glBegin(GL_LINES);
            glVertex3f(x, y, z);
            glVertex3f(x, y + 0.5, z);
            glEnd();
            
            // Update values
            //Move
            // Adjust slowdown for speed!
            par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 1000);
            par_sys[loop].vel += par_sys[loop].gravity;
            // Decay
            par_sys[loop].life -= par_sys[loop].fade;
            
            if (par_sys[loop].ypos <= 0) {
                par_sys[loop].life = -1.0;
            }
            //Revive
            if (par_sys[loop].life < 0.0) {
                initParticles(loop);
            }
        }
    }
}

void createAndDefineTextures(){
    //=======================================Floor
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("/Users/JoaoFigueiredo/Projectos/ProjectCG/CG_Project/CG_Project/texturas/grass.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    // SKY
    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    imag.LoadBmpFile("/Users/JoaoFigueiredo/Projectos/ProjectCG/CG_Project/CG_Project/texturas/sky.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    // SKY
    glGenTextures(1, &texture[2]);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("/Users/JoaoFigueiredo/Projectos/ProjectCG/CG_Project/CG_Project/texturas/pool.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    // SKY
    glGenTextures(1, &texture[3]);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("/Users/JoaoFigueiredo/Projectos/ProjectCG/CG_Project/CG_Project/texturas/glass.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    // SKY
    glGenTextures(1, &texture[4]);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("/Users/JoaoFigueiredo/Projectos/ProjectCG/CG_Project/CG_Project/texturas/water.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    //=======================================Parede casa
    glGenTextures(1, &texture[5]);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    imag.LoadBmpFile("/Users/JoaoFigueiredo/Projectos/ProjectCG/CG_Project/CG_Project/texturas/wall.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    //=======================================Parede casa
    glGenTextures(1, &texture[6]);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    imag.LoadBmpFile("/Users/JoaoFigueiredo/Projectos/ProjectCG/CG_Project/CG_Project/texturas/marble.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    //=======================================Parede casa
    glGenTextures(1, &texture[7]);
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    imag.LoadBmpFile("/Users/JoaoFigueiredo/Projectos/ProjectCG/CG_Project/CG_Project/texturas/madeira.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
}

void drawTerrace()
{
    glTranslated(50, -10, -45);
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    //Lado multicolorido - FRENTE
    glBegin(GL_POLYGON);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(tamanho, -tamanhoY, -tamanho);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(tamanho, tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-tamanho + 70, tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-tamanho + 70, -tamanhoY, -tamanho);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(0, -tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, -tamanho);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(-tamanho, 0, -tamanho);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(-tamanho, -tamanhoY, -tamanho);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    // Lado branco - TRASEIRA
    glBegin(GL_POLYGON);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(tamanho, -tamanhoY, tamanho);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(tamanho, tamanhoY, tamanho);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-tamanho, tamanhoY, tamanho);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-tamanho, -tamanhoY, tamanho);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    // Lado roxo - DIREITA
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(tamanho, -tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(tamanho, tamanhoY, -tamanho);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(tamanho, tamanhoY, tamanho);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(tamanho, -tamanhoY, tamanho);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    // Lado verde - ESQUERDA
    glBegin(GL_POLYGON);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-tamanho, -tamanhoY, tamanho);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-tamanho, tamanhoY, tamanho);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-tamanho, tamanhoY, -tamanho + 70);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-tamanho, -tamanhoY, -tamanho + 70);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.5f, 0.0f);glVertex3f(-tamanho, -tamanhoY, 0);
    glTexCoord2f(0.5f, 0.5f);glVertex3f(-tamanho, 0, 0);
    glTexCoord2f(0.0f, 0.5f);glVertex3f(-tamanho, 0, -tamanho);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-tamanho, -tamanhoY, -tamanho);
    glEnd();
    
    // Lado azul - TOPO
    glBegin(GL_POLYGON);
    glVertex3f(tamanho, tamanhoY, tamanho);
    glVertex3f(tamanho, tamanhoY, -tamanho);
    glVertex3f(-tamanho + 70, tamanhoY, -tamanho);
    glVertex3f(-tamanho + 70, tamanhoY, tamanho);
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3f(0, tamanhoY, tamanho);
    glVertex3f(0, tamanhoY, -tamanho + 70);
    glVertex3f(-tamanho, tamanhoY, -tamanho + 70);
    glVertex3f(-tamanho, tamanhoY, tamanho);
    glEnd();
    
    // Lado vermelho - BASE
    glBegin(GL_POLYGON);
    glVertex3f(tamanho, -tamanhoY, -tamanho);
    glVertex3f(tamanho, -tamanhoY, tamanho);
    glVertex3f(-tamanho, -tamanhoY, tamanho);
    glVertex3f(-tamanho, -tamanhoY, -tamanho);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
}

void drawPool()
{
 
    //INFINITY POOL FRONT
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0,0.81,0.82,0.4);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(-tamanho, 0, -tamanho);
    glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 0, -tamanho);
    glTexCoord2f(1.0f, 1.0f); glVertex3i(0, tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 1.0f); glVertex3i(-tamanho, tamanhoY, -tamanho);
    glEnd();
    glDisable(GL_BLEND);

    
    //TRASEIRA
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBegin(GL_POLYGON);
    
    // Back Face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-tamanho, -tamanhoY + tamanhoY, 0);    // Bottom Right
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-tamanho, tamanhoY, 0);    // Top Right
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0, tamanhoY, 0);    // Top Left
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0, -tamanhoY + tamanhoY, 0);    // Bottom Left
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
   
    
    //LEFT SIDE
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0,0.81,0.82,0.4);
    glBegin(GL_POLYGON);

    glVertex3i(-tamanho, 0, 0);
    glVertex3i(-tamanho, tamanhoY, 0);
    glVertex3i(-tamanho, tamanhoY, -tamanho);
    glVertex3i(-tamanho, 0, -tamanho);
    glEnd();
    
    glDisable(GL_BLEND);
    
    //RIGHT SIDE
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    
    glBegin(GL_POLYGON);
    
   glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(0, tamanhoY, 0);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(0, tamanhoY, -tamanho);
   glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, -tamanho);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_2D);
    //TOP SIDE
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 1.0f); glVertex3i(-tamanho, tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(-tamanho, tamanhoY, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3i(0, tamanhoY, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex3i(0, tamanhoY, -tamanho);
    
    
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    //DOWN SIDE
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBegin(GL_POLYGON);
    glColor4f(BLUE);
     glTexCoord2f(0.0f, 0.0f);glVertex3f(0, 0, tamanho - 70);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(0, 0, -tamanho);
     glTexCoord2f(1.0f, 1.0f);glVertex3f(-tamanho, 0, -tamanho);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-tamanho, 0, tamanho - 70);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawSkybox()
{
    //atrás
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glPushMatrix();
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
   
    // Back Face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-800, -600, -250);    // Bottom Right
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-800,  600, -250);    // Top Right
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 800,  600, -250);    // Top Left
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 800, -600, -250);    // Bottom Left
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    
    //esquerda
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glPushMatrix();
    
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(-250, -600, -800);
    glTexCoord2f(1.0f, 0.0f); glVertex3i(-250, -600, 800);
    glTexCoord2f(1.0f, 1.0f); glVertex3i(-250, 600, 800);
    glTexCoord2f(0.0f, 1.0f); glVertex3i(-250, 600, -800);
    
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    //direita
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glPushMatrix();
    
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3i(250, -600, -800);
    glTexCoord2f(1.0f, 1.0f); glVertex3i(250, 600, -800);
    glTexCoord2f(0.0f, 1.0f); glVertex3i(250, 600, -800);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(250, -600, -800);
    
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    
    //chão
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glPushMatrix();
    
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(-800, -tamanhoY - 10, -800);
    glTexCoord2f(1.0f, 0.0f); glVertex3i(-800, -tamanhoY - 10, 800);
    glTexCoord2f(1.0f, 1.0f); glVertex3i(800, -tamanhoY - 10, 800);
    glTexCoord2f(0.0f, 1.0f); glVertex3i(800, -tamanhoY - 10, -800);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    
    
    
    //Frente
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glPushMatrix();
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(-800, -600, 650);
    glTexCoord2f(1.0f, 0.0f); glVertex3i(800, -600, 650);
    glTexCoord2f(1.0f, 1.0f); glVertex3i(800, 600, 650);
    glTexCoord2f(0.0f, 1.0f); glVertex3i(-800, 600, 650);
   
    
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    
    
    // -------------------------Ceu-------------------
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glPushMatrix();
    
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3i(-800, 250, -800);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(-800, 250, 800);
    glTexCoord2f(1.0f, 0.0f); glVertex3i(800, 250, 800);
    glTexCoord2f(1.0f, 1.0f); glVertex3i(800, 250, -800);
   
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    
    
}

void drawSteps()
{
    glEnable(GL_TEXTURE_2D);
    
    // Lado branco - TRASEIRA
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_POLYGON);
    
     glTexCoord2f(0.0f, 1.0f);glVertex3f(heightstairs, -heightstairs, heightstairs);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(heightstairs, heightstairs, heightstairs);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-heightstairs, heightstairs, heightstairs);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-heightstairs, -heightstairs, heightstairs);
    glEnd();
    
    // Lado roxo - DIREITA
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_POLYGON);
    
    glTexCoord2f(0.0f, 1.0f);glVertex3f(heightstairs, -heightstairs, -heightstairs);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(heightstairs, heightstairs, -heightstairs);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(heightstairs, heightstairs, heightstairs);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(heightstairs, -heightstairs, heightstairs);
    glEnd();
    
    
    // Lado verde - ESQUERDA
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_POLYGON);
    
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-heightstairs, -heightstairs, heightstairs);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-heightstairs, heightstairs, heightstairs);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-heightstairs, heightstairs, -heightstairs);
     glTexCoord2f(1.0f, 1.0f);glVertex3f(-heightstairs, -heightstairs, -heightstairs);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    // Lado azul - TOPO
    glBegin(GL_POLYGON);
    
    glTexCoord2f(1.0f, 0.0f);glVertex3f(heightstairs, heightstairs, heightstairs);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(heightstairs, heightstairs, -heightstairs);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-heightstairs, heightstairs, -heightstairs);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-heightstairs, heightstairs, heightstairs);
    glEnd();
    /*
    // Lado vermelho - BASE
    glBegin(GL_POLYGON);
    
    glVertex3f(heightstairs, -heightstairs, -heightstairs);
    glVertex3f(heightstairs, -heightstairs, heightstairs);
    glVertex3f(-heightstairs, -heightstairs, heightstairs);
    glVertex3f(-heightstairs, -heightstairs, -heightstairs);
    glEnd();*/
   
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_POLYGON);
    
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-heightstairs, -heightstairs, heightstairs);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(heightstairs, heightstairs, heightstairs);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-heightstairs, heightstairs, heightstairs);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-heightstairs, -heightstairs, -heightstairs);
    glEnd();
}

void drawStairs()
{
    GLdouble yscale = 0.5;
    glPushMatrix();
    glTranslated((-20), -tamanhoY, -tamanho - tamanho - 2);
    glScaled(0.5, yscale, 2);
    
    drawSteps();
    glPopMatrix();
    
    glPushMatrix();
    glTranslated((-20) + heightstairs, -tamanhoY, -tamanho - tamanho - 2);
    
    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(0.5, yscale + 0.5, 2);
    drawSteps();
    
    glPopMatrix();
    
    glPushMatrix();
    glTranslated((-20) + heightstairs + 10, -tamanhoY, -tamanho - tamanho - 2);
    
    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(0.5, yscale + 1, 2);
    drawSteps();
    
    glPopMatrix();
    
    glPushMatrix();
    glTranslated((-20) + heightstairs + 20, -tamanhoY, -tamanho - tamanho - 2);
    
    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(0.5, yscale + 1.5, 2);
    drawSteps();
    
    glPopMatrix();
    
    glPushMatrix();
    glTranslated((-20) + heightstairs + 30, -tamanhoY, -tamanho - tamanho - 1);
    
    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(0.5, yscale + 2, 2);
    drawSteps();
    
    glPopMatrix();
    
    glPushMatrix();
    glTranslated((-20) + heightstairs + 55, -tamanhoY, -tamanho - tamanho - 1);
    
    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(2, yscale + 2.5, 2);
    drawSteps();
    
    glPopMatrix();
    
    glPushMatrix();
    glTranslated((-20) + heightstairs + 55, -tamanhoY, -tamanho - tamanho - heightstairs + 25);
    
    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(2, yscale + 3, 0.5);
    drawSteps();
    
    glPopMatrix();
    glPushMatrix();
    glTranslated((-20) + heightstairs + 55, -tamanhoY, -tamanho - tamanho - heightstairs + 35);
    
    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(2, yscale + 3.5, 0.5);
    drawSteps();
    
    glPopMatrix();
    
    glPushMatrix();
    glTranslated((-20) + heightstairs + 55, -tamanhoY, -tamanho - tamanho - heightstairs + 45);
    
    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(2, yscale + 4, 0.5);
    drawSteps();
    
    glPopMatrix();
    glPushMatrix();
    glTranslated((-20) + heightstairs + 55, -tamanhoY, -tamanho - tamanho - heightstairs + 55);
    
    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(2, yscale + 4.5, 0.5);
    drawSteps();
    
    glPopMatrix();
    
    glPushMatrix();
    glTranslated((-20) + heightstairs + 55, -tamanhoY, -tamanho - tamanho - heightstairs + 65);
    
    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(2, yscale + 5, 0.5);
    drawSteps();
    
    glPopMatrix();
    
    glPushMatrix();
    glTranslated((-20) + heightstairs + 55, -tamanhoY, -tamanho - tamanho - heightstairs + 75);
    
    //glTranslated((tamanho / 2) + heightstairs, -tamanhoY, -tamanho - tamanho);
    glScaled(2, yscale + 5.5, 0.5);
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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glColor4f(1, 0, 1, 0.5f);
    if (chuva == 1)
        drawRain();
    glDisable(GL_BLEND);

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
        case 'c':
        case 'C':
            if (chuva == 1)
                chuva = 0;
            else
                chuva = 1;
            glutPostRedisplay();
            break;    //--------------------------- Escape
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
