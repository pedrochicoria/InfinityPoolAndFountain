//Librairies
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
#include "materiais.h"
#include "RgbImage.h"
#define   PI 3.14159

//------------------------------------------------------------ Sistema Coordenadas
GLfloat  xC=16.0, zC=15.0;
GLint    wScreen=800, hScreen=500;
char     texto[30];
GLint    msec     = 100;

//------------------------------------------------------------ Observador
GLfloat  rVisao=3.0, aVisao=0.5*PI, incVisao=0.1;
GLfloat  angPersp=109.0;
GLfloat  obsPini[] ={-8, -3.0, -14.0};
GLfloat  obsPfin[] ={obsPini[0]-rVisao*cos(aVisao), obsPini[1], obsPini[2]-rVisao*sin(aVisao)};

//------------------------------------------------------------ Skybox
GLUquadricObj*  sky  = gluNewQuadric ( );
GLfloat         sizeSky  = 30;

//------------------------------------------------------------ Texturas
GLuint   texture[10];
RgbImage imag;

//------------------------------------------------------------ Mateirais
GLint material=10;

//------------------------------------------------------------ Iluminacao Ambiente
GLint   dia=0;
GLfloat intensidade=10.0;
GLfloat luzGlobalCorAmb[4]={intensidade,intensidade,intensidade,1.0};

//------------------------------------------------------------ Lanterna (local) - FOCO
GLint   ligaFoco=0;
GLfloat rFoco=1.1, aFoco=aVisao;
GLfloat incH =0.0, incV=0.0;
GLfloat incMaxH =0.5, incMaxV=0.35;
GLfloat focoPini[]= { obsPini[0], obsPini[1], obsPini[2], 1.0 };
GLfloat focoPfin[]= { obsPini[0]-rFoco*cos(aFoco), obsPini[1], obsPini[2]-rFoco*sin(aFoco), 1.0};
GLfloat focoDir[] = { focoPfin[0]-focoPini[0], 0, focoPfin[2]-focoPini[2]};
GLfloat focoExp   = 10.0;
GLfloat focoCut   = 60.0;

GLfloat focoCorDif[4] ={ 0.9, 0.9, 0.9, 1.0};
GLfloat focoCorEsp[4] ={ 1.0, 1.0, 1.0, 1.0};

//------------------------------------------------------------ Variaveis globais
GLfloat tamanho = 7.0;
GLfloat tamanhoY = 3;
GLfloat heightstairs = 1;
GLfloat widthstairs = 1;

//------------------------------------------------------------ Declaração e inicialização das texturas
void initTexturas(){
    //----------------------------------------- SkyBox
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("/Users/JoaoFigueiredo/Projectos/ProjectCG/CG_Project/CG_Project/texturas/skyBox.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    //----------------------------------------- Chao
    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    imag.LoadBmpFile("/Users/JoaoFigueiredo/Projectos/ProjectCG/CG_Project/CG_Project/texturas/grass.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    
    //----------------------------------------- Paredes da piscina
    glGenTextures(1, &texture[3]);
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
    
    //----------------------------------------- Agua da piscina
    glGenTextures(1, &texture[4]);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
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
    
   //----------------------------------------- Paredes da casa
    glGenTextures(1, &texture[5]);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
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
}

//------------------------------------------------------------ ILUMINACAO
void initLights(void){
    //…………………………………………………………………………………………………………………………………………… Ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);

    //…………………………………………………………………………………………………………………………………………… Foco
    glLightfv(GL_LIGHT1, GL_POSITION,      focoPini );
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION,focoDir );
    glLightf (GL_LIGHT1, GL_SPOT_EXPONENT ,focoExp);
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF,   focoCut);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,       focoCorDif );
    glLightfv(GL_LIGHT1, GL_SPECULAR,      focoCorEsp  );
    
    if (ligaFoco)
        glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);
    
}

//------------------------------------------------------------ MATERIAIS
void initMaterials(int material) {
    
    switch (material){
        case 0: //……………………………………………………………………………………………whitePlastic
            glMaterialfv(GL_FRONT,GL_AMBIENT,  whitePlasticAmb  );
            glMaterialfv(GL_FRONT,GL_DIFFUSE,  whitePlasticDif );
            glMaterialfv(GL_FRONT,GL_SPECULAR, whitePlasticSpec);
            glMaterialf (GL_FRONT,GL_SHININESS,whitePlasticCoef);
            break;
    }
}

//------------------------------------------------------------ SkyBox
void drawSkySphere(){
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    initMaterials(0);
    
    glPushMatrix();
    glRotatef (       90, -1, 0, 0);
    gluQuadricDrawStyle ( sky, GLU_FILL   );
    gluQuadricNormals   ( sky, GLU_SMOOTH );
    gluQuadricTexture   ( sky, GL_TRUE    );
    gluSphere ( sky, sizeSky*1, 150, 150);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawTerrace(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    initMaterials(0);
    
    glPushMatrix();
    glTranslatef (-4.0, -3.0, 4);
    
    glBegin(GL_POLYGON);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(tamanho, -tamanhoY, -tamanho);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(tamanho, tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-tamanho + 7.0, tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-tamanho + 7.0, -tamanhoY, -tamanho);
    glEnd();
    
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(0, -tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, -tamanho);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(-tamanho, 0, -tamanho);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(-tamanho, -tamanhoY, -tamanho);
    glEnd();
    
    glBegin(GL_POLYGON);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(tamanho, -tamanhoY, tamanho);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(tamanho, tamanhoY, tamanho);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-tamanho, tamanhoY, tamanho);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-tamanho, -tamanhoY, tamanho);
    glEnd();
    
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(tamanho, -tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(tamanho, tamanhoY, -tamanho);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(tamanho, tamanhoY, tamanho);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(tamanho, -tamanhoY, tamanho);
    glEnd();
    
    glBegin(GL_POLYGON);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-tamanho, -tamanhoY, tamanho);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-tamanho, tamanhoY, tamanho);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-tamanho, tamanhoY, -tamanho + 7.0);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-tamanho, -tamanhoY, -tamanho + 7.0);
    glEnd();
    
    glBegin(GL_POLYGON);
    glTexCoord2f(0.5f, 0.0f);glVertex3f(-tamanho, -tamanhoY, 0);
    glTexCoord2f(0.5f, 0.5f);glVertex3f(-tamanho, 0, 0);
    glTexCoord2f(0.0f, 0.5f);glVertex3f(-tamanho, 0, -tamanho);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-tamanho, -tamanhoY, -tamanho);
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3f(tamanho, tamanhoY, tamanho);
    glVertex3f(tamanho, tamanhoY, -tamanho);
    glVertex3f(-tamanho + 7.0, tamanhoY, -tamanho);
    glVertex3f(-tamanho + 7.0, tamanhoY, tamanho);
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3f(0, tamanhoY, tamanho);
    glVertex3f(0, tamanhoY, -tamanho + 7.0);
    glVertex3f(-tamanho, tamanhoY, -tamanho + 7.0);
    glVertex3f(-tamanho, tamanhoY, tamanho);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(tamanho, -tamanhoY, -tamanho);
    glVertex3f(tamanho, -tamanhoY, tamanho);
    glVertex3f(-tamanho, -tamanhoY, tamanho);
    glVertex3f(-tamanho, -tamanhoY, -tamanho);
    glEnd();
    
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

//------------------------------------------------------------ Pool
void drawPool(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    initMaterials(0);
    
    glPushMatrix();
    glTranslatef (-4.0, -3.0, 4);
    
    glBegin(GL_POLYGON);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-tamanho, -tamanhoY + tamanhoY, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-tamanho, tamanhoY, 0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0, tamanhoY, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0, -tamanhoY + tamanhoY, 0);
    glEnd();
    
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(0, tamanhoY, 0);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(0, tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, -tamanho);
    glEnd();
   
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 1.0f); glVertex3i(-tamanho, tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 0.0f); glVertex3i(-tamanho, tamanhoY, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3i(0, tamanhoY, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex3i(0, tamanhoY, -tamanho);
    glEnd();
    
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(0, 0, tamanho - 7.0);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(0, 0, -tamanho);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-tamanho, 0, -tamanho);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-tamanho, 0, tamanho - 7.0);
    glEnd();
   
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0,0.81,0.82,0.1);
    
    glBegin(GL_POLYGON);
    glVertex3i(-tamanho, 0, -tamanho);
    glVertex3i(0, 0, -tamanho);
    glVertex3i(0, tamanhoY, -tamanho);
    glVertex3i(-tamanho, tamanhoY, -tamanho);
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3i(-tamanho, 0, 0);
    glVertex3i(-tamanho, tamanhoY, 0);
    glVertex3i(-tamanho, tamanhoY, -tamanho);
    glVertex3i(-tamanho, 0, -tamanho);
    glEnd();
    
    glPopMatrix();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

}
//------------------------------------------------------------ CHAO
void drawChao(){
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[1]);
    initMaterials(0);
    
    glPushMatrix();
    glTranslatef(0,-5,0);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f,0.0f); glVertex3i( -xC, 0, -xC );
    glTexCoord2f(1.0, 0.0);  glVertex3i( -xC, 0,  xC );
    glTexCoord2f(1.0, 1.0);  glVertex3i(  xC, 0,  xC);
    glTexCoord2f(0.0, 1.0);  glVertex3i(  xC, 0,  -xC);
    glEnd();
    
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

//------------------------------------------------------------ Escadas
void drawSteps(){
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
    
    // Lado vermelho - BASE
    glBegin(GL_POLYGON);
    
    glVertex3f(heightstairs, -heightstairs, -heightstairs);
    glVertex3f(heightstairs, -heightstairs, heightstairs);
    glVertex3f(-heightstairs, -heightstairs, heightstairs);
    glVertex3f(-heightstairs, -heightstairs, -heightstairs);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_POLYGON);
    
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-heightstairs, -heightstairs, heightstairs);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(heightstairs, heightstairs, heightstairs);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-heightstairs, heightstairs, heightstairs);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-heightstairs, -heightstairs, -heightstairs);
    glEnd();
}

//------------------------------------------------------------ Escadas

void drawStairs(){
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
}
    
//------------------------------------------------------------ INICIALIZACOES
void init(void) {
    glClearColor(1,1,1,1);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    //…………………………………………………………………………………………………………………………… ILUMINACAO
    glEnable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);    // dos dois lados
    
    initTexturas();
    initLights();
    initMaterials(0);
}


//------------------------------------------------------------ Side text
void desenhaTexto(char *string, GLfloat x, GLfloat y, GLfloat z){
    glRasterPos3f(x,y,z);
    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *string++);
}

//------------------------------------------------------------ Resize
GLvoid resize(GLsizei width, GLsizei height){
    wScreen=width;
    hScreen=height;
    glViewport( 0, 0, wScreen,hScreen );
    glutPostRedisplay();
}

//------------------------------------------------------------ Eixos
void drawOrientacao(){
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    
    //----------------------- OBSERVADOR
    glPushMatrix();
    glColor4f(0,1,0,1);
    glTranslatef (obsPini[0],obsPini[1],obsPini[2]);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glColor4f(0,0,1,1);
    glTranslatef (obsPfin[0],obsPfin[1],obsPfin[2]);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
    glColor4f(0,0,0,1);
    glTranslatef (focoPfin[0],focoPfin[1],focoPfin[2]);
    glutSolidCube(0.75);
    glPopMatrix();
    glEnable(GL_LIGHTING);
    
    //----------------------- EIXOS
    glColor4f(1,1,0,1);
    glBegin(GL_LINES);
    glVertex3i(0,0,-xC);
    glVertex3i(0,0, xC);
    glEnd();
    glColor4f(1,1,0,1);
    glBegin(GL_LINES);
    glVertex3i(0,-xC,0);
    glVertex3i(0, xC,0);
    glEnd();
    glColor4f(1,1,0,1);
    glBegin(GL_LINES);
    glVertex3i(-xC,0,0);
    glVertex3i( xC,0,0);
    glEnd();
    glEnable(GL_LIGHTING);
}

//------------------------------------------------------------ Informação dos comandos
void showInformacao(){
    glDisable(GL_LIGHTING);
    glColor4f(1,0,0.4,1);
    sprintf(texto, "%d - Foco        'F'  ", ligaFoco);
    desenhaTexto(texto,-12,1,-10);
    sprintf(texto, "%f - Ambiente        'L'  ", intensidade);
    desenhaTexto(texto,-12,1,-5);
    glEnable(GL_LIGHTING);
}

//------------------------------------------------------------ Display
void display(void){
    glClearColor(0.8,0.8,0.8,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    //================================================================= Viewport1
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glViewport (0, hScreen/4, wScreen/6, hScreen/5);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (-xC,xC, -xC,xC, -zC,zC);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt( 0, 5,0, 0,0,0, 0, 0, -1);
    
    //--------------------- desenha objectos
    //drawTerrace();
    drawOrientacao();
    showInformacao();
    //drawPool();
    
    //================================================================= Viewport2
    glEnable(GL_LIGHTING);
    glViewport (wScreen/5, 0, 0.75*wScreen, hScreen);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angPersp, (float)wScreen/hScreen, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(obsPini[0], obsPini[1], obsPini[2], obsPfin[0], obsPfin[1], obsPfin[2], 0, 1, 0);
    
    initLights();
    drawTerrace();
    drawChao();
    drawSkySphere();
    //drawStairs();
    drawPool();

    glutSwapBuffers();
}

//------------------------------------------------------------ EVENTOS
void updateVisao(){
    
    obsPfin[0] =obsPini[0]+rVisao*cos(aVisao);
    obsPfin[2] =obsPini[2]-rVisao*sin(aVisao);
    
    focoPini[0]=obsPini[0];
    focoPini[2]=obsPini[2];
    focoPfin[0]=focoPini[0]-rFoco*cos(aFoco+incH);
    focoPfin[2]=focoPini[2]-rFoco*sin(aFoco+incH);
    focoPini[1]=obsPini[1];
    focoPini[2]=obsPini[2];
    focoDir[0] =focoPfin[0]-focoPini[0];
    focoDir[1] =incV;
    focoDir[2] =focoPfin[2]-focoPini[2];
    
    glutPostRedisplay();
}

//------------------------------------------------------------ EVENTOS
void keyboard(unsigned char key, int x, int y){
    switch (key) {
            //--------------------------- Direccao da Lanterna
            //--------------------------- Projeccao
        case 's':
        case 'S':
            incH=incH-0.05;
            if (incH<-incMaxH ) incH=-incMaxH ;
            updateVisao();
            break;
        case 'd':
        case 'D':
            incH=incH+0.05;
            if (incH>incMaxH ) incH=incMaxH ;
            updateVisao();
            break;
        case 'e':
        case 'E':
            incV=incV+0.05;
            if (incV>incMaxV ) incV=incMaxV ;
            updateVisao();
            break;
        case 'c':
        case 'C':
            incV=incV-0.05;
            if (incV<-incMaxV ) incV=-incMaxV ;
            updateVisao();
            break;
            
            //--------------------------- Observador
        case 'f':
        case 'F':
            ligaFoco=!ligaFoco;
            glutPostRedisplay();
            break;
            
        case 'l':
        case 'L':
            if(intensidade==0){
                intensidade=10.0;

            }
            else{
                intensidade=0;
            }
            glutPostRedisplay();
            break;
            
            //=============================================
            //--------------------------- Angulo
        case 'z': case 'Z':
            angPersp=angPersp+3;
            if (angPersp>170) angPersp=170;
            glutPostRedisplay();
            break;
        case 'x': case 'X':
            angPersp=angPersp-3;
            if (angPersp<1) angPersp=1;
            glutPostRedisplay();
            break;
            
            //--------------------------- Altura observador
        case 'q': case 'Q':
            obsPini[1]=obsPini[1]+1;
            glutPostRedisplay();
            break;
        case 'a': case 'A':
            obsPini[1]=obsPini[1]-1;
            glutPostRedisplay();
            break;
            
            //--------------------------- Escape
        case 27:
            exit(0);
            break;
            
    }
}

void teclasNotAscii(int key, int x, int y){
    if(key == GLUT_KEY_UP) {
        obsPini[0]=obsPini[0]+incVisao*cos(aVisao);
        obsPini[2]=obsPini[2]-incVisao*sin(aVisao);
    }
    if(key == GLUT_KEY_DOWN) {
        obsPini[0]=obsPini[0]-incVisao*cos(aVisao);
        obsPini[2]=obsPini[2]+incVisao*sin(aVisao);
    }
    if(key == GLUT_KEY_LEFT) {
        aVisao = (aVisao + 0.1) ;
        aFoco=aFoco-0.1;
    }
    if(key == GLUT_KEY_RIGHT) {
        aVisao = (aVisao - 0.1) ;
        aFoco=aFoco+0.1;
        
    }
    updateVisao();
}


void Timer(int value){
    glutPostRedisplay();
    glutTimerFunc(msec,Timer, 1);
}


//------------------------------------------------------------ MAIN
int main(int argc, char** argv){
    
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize (wScreen, hScreen);
    glutInitWindowPosition (400, 10);
    glutCreateWindow ("Projecto CG 2019");
    
    init();
    glutSpecialFunc(teclasNotAscii);
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(msec, Timer, 1);
    
    glutMainLoop();
    
    return 0;
}


