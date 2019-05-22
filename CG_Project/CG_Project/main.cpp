// g++ -o main  main.cpp RgbImage.cpp  -Wno-deprecated -framework OpenGL -framework GLUT 
//Librairies
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "materiais.h"
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>

//Image loader
#include "RgbImage.h"

//Global variables
#define PI 3.14159
#define frand()			((float)rand()/RAND_MAX)
#define MAX_PARTICULAS  2500
#define RandomFactor 2.0	//cannot be variable, because it is only use in InitFountain(),
using namespace std;

GLfloat tamanhoY = 32.5;


struct Svertex { // posição 
    GLfloat x,y,z;
};

class CDrop
{
private:
	GLfloat time;  //How many steps the drop was "outside", when it falls into the water, time is set back to 0
	Svertex ConstantSpeed;  //See the doc for explanation of the physics
	GLfloat AccFactor;
public:
	void SetConstantSpeed (Svertex NewSpeed);
	void SetAccFactor(GLfloat NewAccFactor);
	void SetTime(GLfloat NewTime);
	void GetNewPosition(Svertex * PositionVertex);  //increments time, gets the new position
};

void CDrop::SetConstantSpeed(Svertex NewSpeed)
{
	ConstantSpeed = NewSpeed;
}

void CDrop::SetAccFactor (GLfloat NewAccFactor)
{
	AccFactor = NewAccFactor;
}

void CDrop::SetTime(GLfloat NewTime)
{
	time = NewTime;
}

void CDrop::GetNewPosition(Svertex * PositionVertex)
{   
    glPushMatrix();
		glTranslatef(PositionVertex->x,PositionVertex->y, PositionVertex->z);
		glutSolidSphere(0.5,20, 20);
	glPopMatrix();
	Svertex Position; time += 1.0;
    cout<<PositionVertex->x<<endl;
    cout<<PositionVertex->y<<endl;
    cout<<PositionVertex->z<<endl;
   
    //------------------------------------------------------------------------- Posição
    Position.x = ConstantSpeed.x * time ;
    cout<<"Constant Speed: "<<ConstantSpeed.x<<endl;
    Position.y = ConstantSpeed.y * time - 0.5*AccFactor * time * time; 
    Position.z = ConstantSpeed.z * time;
    PositionVertex->x = Position.x; 
    PositionVertex->y = Position.y ; 
    PositionVertex->z = Position.z;
    //------------------------------------------------------------------------- Bate no Chão
    if (Position.y < -tamanhoY) {
        time = time - int(time);
        if (time > 0.0) time -= 1.0; 
    }
		
}

CDrop * FountainDrops;
Svertex * FountainVertices;
GLint Steps = 3;   //a fountain has several steps, each with its own height
GLint RaysPerStep = 8;  
GLint DropsPerRay = 50;
GLfloat DropsComplete = Steps * RaysPerStep * DropsPerRay;
GLfloat AngleOfDeepestStep = 80;
GLfloat AccFactor = 0.011;

Particle  particula1[MAX_PARTICULAS];
GLint    milisec = 1000; 

GLfloat tamanho = 70.0;

GLfloat heightstairs = 10;
GLfloat widthstairs = 10;
GLfloat fountain_walls = 40;

//------------------------------------------------------------ Sistema Coordenadas + objectos
GLint wScreen = 800, hScreen = 600; //.. janela (pixeis)
GLuint height = 600, width = 600;
GLfloat xC = 15.0, yC = 15.0, zC = 70.0; //.. Mundo  (unidades mundo)

GLUquadricObj *sky = gluNewQuadric();
GLfloat sizeSky = 250;

//------------------------------------------------------------ Observador
GLint ligaFoco = 0;
GLint ligaPoll = 0;
GLint ligaEscadas = 0;
GLfloat rVisao = 20, aVisao = PI + PI / 2, incVisao = 0.05;
GLfloat obsPini[] = {-120, 0, -150}; //estava a 320
GLfloat obsPfin[] = {obsPini[0] - rVisao * cos(aVisao), obsPini[1], obsPini[2] - rVisao *sin(aVisao)};
GLfloat angPersp = 109.0;
GLfloat rFoco = 1.1, aFoco = aVisao;
GLfloat focoPini[] = {obsPini[0], obsPini[1], obsPini[2], 1.0};
GLfloat focoPfin[] = {obsPini[0] - rFoco * cos(aFoco), obsPini[1], obsPini[2] - rFoco *sin(aFoco), 1.0};
GLfloat focoDir[] = {focoPfin[0] - focoPini[0], 0, focoPfin[2] - focoPini[2]};
GLfloat focoExp = 10.0;
GLfloat focoCut = 10.0;
GLfloat angZoom = 80;
GLfloat incZoom = 3;
GLuint movement = -1; //-1 para fora do edificio | 0  o res de chão | 1 para o primeiro movement
GLfloat incH = 0.0, incV = 0.0;
GLfloat incMaxH = 0.5, incMaxV = 0.35;

GLuint texture[14];
GLuint tex;
RgbImage imag;

//------------------------------------------------------------ Iluminacao Ambiente
GLint night = 0;
GLfloat intensidade = 10.0;
GLfloat lightColorGlobal[4] = {0.1, 0.1, 0.1, 1.0};

GLfloat focoCorDif[4] = {0.9, 0.9, 0.9, 1.0};
GLfloat focoCorEsp[4] = {1.0, 1.0, 1.0, 1.0};

GLfloat light2_position[] = {-101.823, -1, -123.3, 1.0};
GLfloat light2_dir[] = {0.980331, -0.03, 0.5};
GLfloat light2_focoCorEsp[4] = {5.0, 5.0, 5.0, 1.0};

GLfloat light3_position[] = {-22.3496, -4, -265.98, 1.0};
GLfloat light3_dir[] = {0.665709, -0.05, 0.875702};



//------------------------------------------------------------ ILUMINACAO
void initLights(void)
{
    //…………………………………………………………………………………………………………………………………………… Ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightColorGlobal);

    //…………………………………………………………………………………………………………………………………………… Foco
    glLightfv(GL_LIGHT1, GL_POSITION, focoPini);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, focoDir);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, focoExp);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, focoCut);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, focoCorDif);
    glLightfv(GL_LIGHT1, GL_SPECULAR, focoCorEsp);
    glLightfv(GL_LIGHT1, GL_AMBIENT, focoCorEsp);

    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_dir);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, focoExp);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, focoCut);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, focoCorDif);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light2_focoCorEsp);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light2_focoCorEsp);

    glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_dir);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, focoExp);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, focoCut);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, focoCorDif);
    glLightfv(GL_LIGHT3, GL_SPECULAR, light2_focoCorEsp);
    glLightfv(GL_LIGHT3, GL_AMBIENT, light2_focoCorEsp);

    if (ligaFoco)
        glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);

    if (ligaPoll)
    {
        glEnable(GL_LIGHT2);
    }
    else
    {
        glDisable(GL_LIGHT2);
    }

    if (ligaEscadas)
    {
        glEnable(GL_LIGHT3);
    }
    else
    {
        glDisable(GL_LIGHT3);
    }
}

void initMaterials(int material)
{

    switch (material)
    {
    case 0: //……………………………………………………………………………………………whitePlastic
        glMaterialfv(GL_FRONT, GL_AMBIENT, whitePlasticAmb);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, whitePlasticDif);
        glMaterialfv(GL_FRONT, GL_SPECULAR, whitePlasticSpec);
        glMaterialf(GL_FRONT, GL_SHININESS, whitePlasticCoef);
        break;
    case 1: //……………………………………………………………………………………………cyanRubber
        glMaterialfv(GL_FRONT, GL_AMBIENT, cyanRubberAmb);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, cyanRubberDif);
        glMaterialfv(GL_FRONT, GL_SPECULAR, cyanRubberSpec);
        glMaterialf(GL_FRONT, GL_SHININESS, cyanRubberCoef);
        break;
    }
}

void initTexturas()
{
    //=======================================Floor
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("/Users/pedrochicoria/Desktop/UC/LEI/CG/Project/ProjectCG/CG_Project/CG_Project/texturas/grass.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    // SKY
    //----------------------------------------- SkyBox
    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("/Users/pedrochicoria/Desktop/UC/LEI/CG/Project/ProjectCG/CG_Project/CG_Project/texturas/skyBox.bmp");
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
    imag.LoadBmpFile("/Users/pedrochicoria/Desktop/UC/LEI/CG/Project/ProjectCG/CG_Project/CG_Project/texturas/pool.bmp");
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
    imag.LoadBmpFile("/Users/pedrochicoria/Desktop/UC/LEI/CG/Project/ProjectCG/CG_Project/CG_Project/texturas/water.bmp");
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
    imag.LoadBmpFile("/Users/pedrochicoria/Desktop/UC/LEI/CG/Project/ProjectCG/CG_Project/CG_Project/texturas/wall.bmp");
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
    imag.LoadBmpFile("/Users/pedrochicoria/Desktop/UC/LEI/CG/Project/ProjectCG/CG_Project/CG_Project/texturas/marble.bmp");
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
    imag.LoadBmpFile("/Users/pedrochicoria/Desktop/UC/LEI/CG/Project/ProjectCG/CG_Project/CG_Project/texturas/madeira.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    //=======================================Rocha
    glGenTextures(1, &texture[8]);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    imag.LoadBmpFile("/Users/pedrochicoria/Desktop/UC/LEI/CG/Project/ProjectCG/CG_Project/CG_Project/texturas/rock.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    //=======================================Water
    glGenTextures(1, &texture[9]);
    glBindTexture(GL_TEXTURE_2D, texture[9]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    imag.LoadBmpFile("/Users/pedrochicoria/Desktop/UC/LEI/CG/Project/ProjectCG/CG_Project/CG_Project/texturas/water.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

}
GLfloat GetRandomFloat(GLfloat range)
{
	return (GLfloat)rand() / (GLfloat)RAND_MAX * range * RandomFactor;
}

void iniParticulas(Particle *particula)
{   

    FountainDrops = new CDrop [ int(DropsComplete) ];
	FountainVertices = new Svertex [ int(DropsComplete) ];
	Svertex NewSpeed;
	GLfloat DropAccFactor; //different from AccFactor because of the random change
	GLfloat TimeNeeded;
	GLfloat StepAngle; //Angle, which the ray gets out of the fountain with
	GLfloat RayAngle;	//Angle you see when you look down on the fountain
	GLint i,j,k;
   
	for (k = 0; k <Steps; k++)
	{
		for (j = 0; j < RaysPerStep; j++)
		{
			for (i = 0; i < DropsPerRay; i++)
			{   
				DropAccFactor = AccFactor + GetRandomFloat(0.0005);
				StepAngle = AngleOfDeepestStep + (90.0-AngleOfDeepestStep) 
						* GLfloat(k) / (Steps-1) + GetRandomFloat(0.2+0.8*(Steps-k-1)/(Steps-1));
              
				//This is the speed caused by the step:
				NewSpeed.x = cos ( StepAngle * PI / 180.0) * (0.2+0.04*k);
				NewSpeed.y = sin ( StepAngle * PI / 180.0) * (0.2+0.04*k);
				//This is the speed caused by the ray:
				RayAngle = (GLfloat)j / (GLfloat)RaysPerStep * 360.0;
				//for the next computations "NewSpeed.x" is the radius. Care! Dont swap the two
				//lines, because the second one changes NewSpeed.x!
				NewSpeed.z = NewSpeed.x * sin ( RayAngle * PI /180.0);
				NewSpeed.x = NewSpeed.x * cos ( RayAngle * PI /180.0);	
				//Calculate how many steps are required, that a drop comes out and falls down again
				TimeNeeded = NewSpeed.y/ DropAccFactor;
                cout<<i+j*DropsPerRay+k*DropsPerRay*RaysPerStep<<endl;
				FountainDrops[i+j*DropsPerRay+k*DropsPerRay*RaysPerStep].SetConstantSpeed ( NewSpeed );
				FountainDrops[i+j*DropsPerRay+k*DropsPerRay*RaysPerStep].SetAccFactor (DropAccFactor);
				FountainDrops[i+j*DropsPerRay+k*DropsPerRay*RaysPerStep].SetTime(TimeNeeded * i / DropsPerRay);
                
			}
		}
	}


	//Tell OGL that we'll use the vertex array function
	glEnableClientState(GL_VERTEX_ARRAY);
	//Pass the date position
	glVertexPointer(	3,			//x,y,z-components
						GL_FLOAT,	//data type of SVertex
						0,			//the vertices are tightly packed
						FountainVertices);
}





void drawSkySphere()
{

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    initMaterials(0);

    glPushMatrix();
    glRotatef(90, -1, 0, 0);
    gluQuadricDrawStyle(sky, GLU_FILL);
    gluQuadricNormals(sky, GLU_SMOOTH);
    gluQuadricTexture(sky, GL_TRUE);
    gluSphere(sky, sizeSky * 5, 250, 250);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawTerrace()
{
    glTranslated(50, -10, -45);
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    //Lado multicolorido - FRENTE
    glBegin(GL_POLYGON);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(tamanho, -tamanhoY, -tamanho);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(tamanho, tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-tamanho + 70, tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-tamanho + 70, -tamanhoY, -tamanho);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(0, -tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, -tamanho);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(-tamanho, 0, -tamanho);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(-tamanho, -tamanhoY, -tamanho);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[5]);
    // Lado branco - TRASEIRA
    glBegin(GL_POLYGON);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(tamanho, -tamanhoY, tamanho);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(tamanho, tamanhoY, tamanho);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-tamanho, tamanhoY, tamanho);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-tamanho, -tamanhoY, tamanho);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[5]);
    // Lado roxo - DIREITA
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(tamanho, -tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(tamanho, tamanhoY, -tamanho);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(tamanho, tamanhoY, tamanho);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(tamanho, -tamanhoY, tamanho);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[5]);
    // Lado verde - ESQUERDA
    glBegin(GL_POLYGON);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-tamanho, -tamanhoY, tamanho);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-tamanho, tamanhoY, tamanho);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-tamanho, tamanhoY, -tamanho + 70);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-tamanho, -tamanhoY, -tamanho + 70);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(-tamanho, -tamanhoY, 0);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(-tamanho, 0, 0);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(-tamanho, 0, -tamanho);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-tamanho, -tamanhoY, -tamanho);
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

    initMaterials(1);

    //TRASEIRA
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBegin(GL_POLYGON);

    // Back Face
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-tamanho, -tamanhoY + tamanhoY, 0); // Bottom Right
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-tamanho, tamanhoY, 0); // Top Right
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0, tamanhoY, 0); // Top Left
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, -tamanhoY + tamanhoY, 0); // Bottom Left
    glEnd();
    glDisable(GL_TEXTURE_2D);

    initMaterials(1);

    //RIGHT SIDE
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);

    glBegin(GL_POLYGON);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0, tamanhoY, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0, tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0, 0, -tamanho);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_2D);
    //TOP SIDE
    glFrontFace(GL_CCW);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[4]);

    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3i(-tamanho, tamanhoY, -tamanho);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3i(-tamanho, tamanhoY, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3i(0, tamanhoY, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3i(0, tamanhoY, -tamanho);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    initMaterials(1);
    glFrontFace(GL_CW);
    //DOWN SIDE
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, tamanho - 70);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0, 0, -tamanho);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-tamanho, 0, -tamanho);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-tamanho, 0, tamanho - 70);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    initMaterials(1);

    glFrontFace(GL_CW);
    //INFINITY POOL FRONT
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0, 0.81, 0.82, 0.1);
    glBegin(GL_POLYGON);
    glVertex3i(-tamanho, 0, -tamanho);
    glVertex3i(0, 0, -tamanho);
    glVertex3i(0, tamanhoY, -tamanho);
    glVertex3i(-tamanho, tamanhoY, -tamanho);
    glEnd();
    glDisable(GL_BLEND);
    initMaterials(1);
    //LEFT SIDEl
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0, 0.81, 0.82, 0.1);
    glBegin(GL_POLYGON);
    glVertex3i(-tamanho, 0, -tamanho);
    glVertex3i(-tamanho, tamanhoY, -tamanho);
    glVertex3i(-tamanho, tamanhoY, 0);
    glVertex3i(-tamanho, 0, 0);

    glEnd();

    glDisable(GL_BLEND);
}

void drawSkybox()
{

    //chão
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    initMaterials(0);

    glPushMatrix();
    glTranslated(0, 0, -45);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3i(-250, -tamanhoY - 10, -250);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3i(-250, -tamanhoY - 10, 250);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3i(250, -tamanhoY - 10, 250);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3i(250, -tamanhoY - 10, -250);
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

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(heightstairs, -heightstairs, heightstairs);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(heightstairs, heightstairs, heightstairs);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-heightstairs, heightstairs, heightstairs);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-heightstairs, -heightstairs, heightstairs);
    glEnd();

    // Lado roxo - DIREITA
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_POLYGON);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(heightstairs, -heightstairs, -heightstairs);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(heightstairs, heightstairs, -heightstairs);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(heightstairs, heightstairs, heightstairs);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(heightstairs, -heightstairs, heightstairs);
    glEnd();

    // Lado verde - ESQUERDA
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_POLYGON);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-heightstairs, -heightstairs, heightstairs);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-heightstairs, heightstairs, heightstairs);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-heightstairs, heightstairs, -heightstairs);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-heightstairs, -heightstairs, -heightstairs);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[7]);
    // Lado azul - TOPO
    glBegin(GL_POLYGON);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(heightstairs, heightstairs, heightstairs);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(heightstairs, heightstairs, -heightstairs);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-heightstairs, heightstairs, -heightstairs);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-heightstairs, heightstairs, heightstairs);
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

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(heightstairs, -heightstairs, -heightstairs);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(heightstairs, heightstairs, -heightstairs);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-heightstairs, heightstairs, -heightstairs);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-heightstairs, -heightstairs, -heightstairs);
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

void drawFountainWalls()
{

    glEnable(GL_TEXTURE_2D);

    // Lado branco - TRASEIRA
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(fountain_walls, -fountain_walls, fountain_walls);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(fountain_walls, fountain_walls, fountain_walls);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-fountain_walls, fountain_walls, fountain_walls);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-fountain_walls, -fountain_walls, fountain_walls);
    glEnd();

    // Lado roxo - DIREITA

    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(fountain_walls, -fountain_walls, -fountain_walls);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(fountain_walls, fountain_walls, -fountain_walls);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(fountain_walls, fountain_walls, fountain_walls);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(fountain_walls, -fountain_walls, fountain_walls);
    glEnd();

    // Lado verde - ESQUERDA
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-fountain_walls, -fountain_walls, fountain_walls);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-fountain_walls, fountain_walls, fountain_walls);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-fountain_walls, fountain_walls, -fountain_walls);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-fountain_walls, -fountain_walls, -fountain_walls);
    glEnd();

    // Lado azul - TOPO
    glBegin(GL_POLYGON);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(fountain_walls, fountain_walls, fountain_walls);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(fountain_walls, fountain_walls, -fountain_walls);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-fountain_walls, fountain_walls, -fountain_walls);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-fountain_walls, fountain_walls, fountain_walls);
    glEnd();

    // Lado vermelho - BASE
    glBegin(GL_POLYGON);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(fountain_walls, -fountain_walls, -fountain_walls);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(fountain_walls, -fountain_walls, fountain_walls);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-fountain_walls, -fountain_walls, fountain_walls);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-fountain_walls, -fountain_walls, -fountain_walls);
    glEnd();

    glBegin(GL_POLYGON);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(fountain_walls, -fountain_walls, -fountain_walls);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(fountain_walls, fountain_walls, -fountain_walls);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-fountain_walls, fountain_walls, -fountain_walls);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-fountain_walls, -fountain_walls, -fountain_walls);
    glEnd();
}

void drawFountain()
{
    glPushMatrix();
    glPushMatrix();
    glTranslated((-175), -tamanhoY, tamanho - tamanho);
    glScaled(1, 0.5, 0.15);
    drawFountainWalls();
    glPopMatrix();
    glPushMatrix();
    glTranslated((-175) + fountain_walls + 6, -tamanhoY, tamanho - tamanho + fountain_walls - 6);
    glScaled(0.15, 0.5, 1);
    drawFountainWalls();
    glPopMatrix();
    glPushMatrix();
    glTranslated((-175), -tamanhoY, tamanho - tamanho + fountain_walls + 28);
    glScaled(1, 0.5, 0.15);
    drawFountainWalls();
    glPopMatrix();
    glPushMatrix();
    glTranslated((-175) - fountain_walls - 6, -tamanhoY, tamanho - tamanho + fountain_walls - 6);
    glScaled(0.15, 0.5, 1);
    drawFountainWalls();
    glPopMatrix();
    glTranslated((-175), -tamanhoY, tamanho - tamanho + fountain_walls);
    glScaled(1, 0.25, 0.8);
    drawFountainWalls();
    glPopMatrix();

    // glScaled(0.5, 1, 0.5);
    glPopMatrix();
}

void drawScene()
{

    drawSkybox();
    drawSkySphere();
    drawTerrace();
    drawStairs();
    drawFountain();
    drawPool();
}

void showParticulas(Particle *particula, int sistema) {
 int i;
 int numero;

 numero=(int) (frand()*10.0);
 
 for (i=0; i<MAX_PARTICULAS; i++)
	{

	glColor4f(1,1,1, particula[i].life);
    
 	/*glBegin(GL_QUADS);				        
		glTexCoord2d(0,0); glVertex3f(particula[i].x -particula[i].size, particula[i].y -particula[i].size, particula[i].z);      
		glTexCoord2d(1,0); glVertex3f(particula[i].x +particula[i].size, particula[i].y -particula[i].size, particula[i].z);        
		glTexCoord2d(1,1); glVertex3f(particula[i].x +particula[i].size, particula[i].y +particula[i].size, particula[i].z);            
		glTexCoord2d(0,1); glVertex3f(particula[i].x -particula[i].size, particula[i].y +particula[i].size, particula[i].z);       
	glEnd();	
    */
    glPushMatrix();
		glTranslatef(particula[i].x,particula[i].y, particula[i].z);
		glutSolidSphere(0.5,20, 20);
	glPopMatrix();
    
    particula[i].x += particula[i].vx;
    particula[i].y += particula[i].vy;
    particula[i].z += particula[i].vz;
    particula[i].vx += particula[i].ax;
    particula[i].vy += particula[i].ay;
    particula[i].vz += particula[i].az;
	particula[i].life -= particula[i].fade;	
	}

}
void DrawFountain(void)
{
	glColor4f(0.8,0.8,0.8,0.8);
	
	for (int i = 0; i < DropsComplete; i++)
	{
		FountainDrops[i].GetNewPosition(&FountainVertices[i]);
	}
	glDrawArrays(	GL_POINTS,
					0,
					DropsComplete);
}

void display(void)
{
    glClearColor(0.8, 0.8, 0.8, 1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_LIGHTING);
    glViewport(0, 0, wScreen, hScreen);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angPersp, (float)wScreen / hScreen, 0.1, 100 * zC);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(obsPini[0], obsPini[1], obsPini[2], obsPfin[0], obsPfin[1], obsPfin[2], 0, 1, 0);

    initLights();

    drawScene();
     //showParticulas(particula1, 1);
     DrawFountain();
    glutSwapBuffers();
}

//------------------------------------------------------------ EVENTOS
void updateVisao()
{

    obsPfin[0] = obsPini[0] + rVisao * cos(aVisao);
    obsPfin[2] = obsPini[2] - rVisao * sin(aVisao);

    focoPini[0] = obsPini[0];
    focoPini[2] = obsPini[2];
    focoPfin[0] = focoPini[0] - rFoco * cos(aFoco + incH);
    focoPfin[2] = focoPini[2] - rFoco * sin(aFoco + incH);
    focoPini[1] = obsPini[1];
    focoPini[2] = obsPini[2];
    focoDir[0] = focoPfin[0] - focoPini[0];
    focoDir[1] = incV;
    focoDir[2] = focoPfin[2] - focoPini[2];
    glutPostRedisplay();
}
//------------------------------------------------------------ EVENTOS
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        //--------------------------- Direccao da Lanterna
        //--------------------------- Projeccao
    case 's':
    case 'S':
        incH = incH - 0.05;
        if (incH < -incMaxH)
            incH = -incMaxH;
        updateVisao();
        break;
    case 'd':
    case 'D':
        incH = incH + 0.05;
        if (incH > incMaxH)
            incH = incMaxH;
        updateVisao();
        break;
    case 'e':
    case 'E':
        incV = incV + 0.05;
        if (incV > incMaxV)
            incV = incMaxV;
        updateVisao();
        break;
    case 'c':
    case 'C':
        incV = incV - 0.05;
        if (incV < -incMaxV)
            incV = -incMaxV;
        updateVisao();
        break;

        //--------------------------- Observador
    case 'f':
    case 'F':
        ligaFoco = !ligaFoco;
        cout << "ok1: " << ligaFoco << endl;
        glutPostRedisplay();
        break;

    case 'l':
    case 'L':
        night = !night;

        if (night)
        {
            lightColorGlobal[0] = 0.1;
            lightColorGlobal[1] = 0.1;
            lightColorGlobal[2] = 0.1;
        }

        else
        {
            lightColorGlobal[0] = intensidade;
            lightColorGlobal[1] = intensidade;
            lightColorGlobal[2] = intensidade;
        }

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightColorGlobal);
        glutPostRedisplay();
        break;

        //=============================================
        //--------------------------- Angulo
    case 'z':
    case 'Z':
        angPersp = angPersp + 3;
        if (angPersp > 170)
            angPersp = 170;
        glutPostRedisplay();
        break;
    case 'x':
    case 'X':
        angPersp = angPersp - 3;
        if (angPersp < 1)
            angPersp = 1;
        glutPostRedisplay();
        break;

        //--------------------------- Altura observador
    case 'q':
    case 'Q':
        obsPini[1] = obsPini[1] + 1;
        glutPostRedisplay();
        break;
    case 'a':
    case 'A':
        obsPini[1] = obsPini[1] - 1;
        glutPostRedisplay();
        break;

    case 'p':
    case 'P':
        ligaPoll = !ligaPoll;
        glutPostRedisplay();
        break;
    case 'k':
    case 'K':
        ligaEscadas = !ligaEscadas;
        glutPostRedisplay();
        break;

        //--------------------------- Escape
    case 27:
        exit(0);
        break;
    }
}

void teclasNotAscii(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)
    {
        obsPini[0] = obsPini[0] + 20 * incVisao * cos(aVisao);
        obsPini[2] = obsPini[2] - 20 * incVisao * sin(aVisao);
    }
    if (key == GLUT_KEY_DOWN)
    {
        obsPini[0] = obsPini[0] - 20 * incVisao * cos(aVisao);
        obsPini[2] = obsPini[2] + 20 * incVisao * sin(aVisao);
    }
    if (key == GLUT_KEY_LEFT)
    {
        aVisao = (aVisao + 0.1);
        aFoco = aFoco - 0.1;
    }
    if (key == GLUT_KEY_RIGHT)
    {
        aVisao = (aVisao - 0.1);
        aFoco = aFoco + 0.1;
    }
    updateVisao();
}

void idle(void)
{
    glutPostRedisplay();
}

//------------------------------------------------------------ INICIALIZACOES
void init(void)
{
    glClearColor(1, 1, 1, 1);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    //…………………………………………………………………………………………………………………………… ILUMINACAO
    glEnable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // dos dois lados

    initTexturas();
    initLights();
    initMaterials(0);
    iniParticulas(particula1);
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(wScreen, hScreen);
    glutInitWindowPosition(300, 100);
    glutCreateWindow(" Projeto Computação Gráfica ");
    init();
    glutSpecialFunc(teclasNotAscii);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(display);
    glutMainLoop();
    return 0;
}
