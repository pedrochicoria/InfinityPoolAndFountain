
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>

GLfloat  esmeraldAmb []={ 0.0215 ,0.1745 ,0.0215 };
GLfloat  esmeraldDif []={   0.07568 ,0.61424 ,0.07568 };
GLfloat  esmeraldSpec []={ 0.633 ,0.727811 ,0.633 };
GLint    esmeraldCoef = 0.6 *128;

GLfloat  jadeAmb []={ 0.135 ,0.2225 ,0.1575 };
GLfloat  jadeDif []={   0.54 ,0.89 ,0.63 };
GLfloat  jadeSpec []={ 0.316228 ,0.316228 ,0.316228 };
GLint   jadeCoef = 0.1 *128;

GLfloat  obsidianAmb []={ 0.05375 ,0.05 ,0.06625 };
GLfloat  obsidianDif []={   0.18275 ,0.17 ,0.22525 };
GLfloat  obsidianSpec []={ 0.332741 ,0.328634 ,0.346435 };
GLint  obsidianCoef = 0.3 *128;

GLfloat  pearlAmb []={ 0.25 ,0.20725 ,0.20725 };
GLfloat  pearlDif []={   1.0 ,0.829 ,0.829 };
GLfloat  pearlSpec []={ 0.296648 ,0.296648 ,0.296648 };
GLint  pearlCoef = 0.088 *128;

GLfloat  rubyAmb []={ 0.1745 ,0.01175 ,0.01175 };
GLfloat  rubyDif []={   0.61424 ,0.04136 ,0.04136 };
GLfloat  rubySpec []={ 0.727811 ,0.626959 ,0.626959 };
GLint  rubyCoef = 0.6 *128;

GLfloat  turquoiseAmb []={ 0.1 ,0.18725 ,0.1745 };
GLfloat  turquoiseDif []={   0.396 ,0.74151 ,0.69102 };
GLfloat  turquoiseSpec []={ 0.297254 ,0.30829 ,0.306678 };
GLint  turquoiseCoef = 0.1 *128;

GLfloat  brassAmb []={ 0.329412 ,0.223529 ,0.027451 };
GLfloat  brassDif []={   0.780392 ,0.568627 ,0.113725 };
GLfloat  brassSpec []={ 0.992157 ,0.941176 ,0.807843 };
GLint  brassCoef = 0.21794872 *128;

GLfloat  bronzeAmb []={ 0.2125 ,0.1275 ,0.054 };
GLfloat  bronzeDif []={   0.714 ,0.4284 ,0.18144 };
GLfloat  bronzeSpec []={ 0.393548 ,0.271906 ,0.166721 };
GLint  bronzeCoef = 0.2 *128;

GLfloat  chromeAmb []={ 0.25 ,0.25 ,0.25 };
GLfloat  chromeDif []={   0.4 ,0.4 ,0.4 };
GLfloat  chromeSpec []={ 0.774597 ,0.774597 ,0.774597 };
GLint  chromeCoef = 0.6 *128;

GLfloat  copperAmb []={ 0.19125 ,0.0735 ,0.0225 };
GLfloat  copperDif []={   0.7038 ,0.27048 ,0.0828 };
GLfloat  copperSpec []={ 0.256777 ,0.137622 ,0.086014 };
GLint  copperCoef = 0.1 *128;

GLfloat  goldAmb []={ 0.24725 ,0.1995 ,0.0745 };
GLfloat  goldDif []={   0.75164 ,0.60648 ,0.22648 };
GLfloat  goldSpec []={ 0.628281 ,0.555802 ,0.366065 };
GLint  goldCoef = 0.4 *128;

GLfloat  silverAmb []={ 0.19225 ,0.19225 ,0.19225 };
GLfloat  silverDif []={   0.50754 ,0.50754 ,0.50754 };
GLfloat  silverSpec []={ 0.508273 ,0.508273 ,0.508273 };
GLint  silverCoef = 0.4 *128;

GLfloat blackPlasticAmb []={ 0.0 ,0.0 ,0.0 };
GLfloat blackPlasticDif []={   0.01 ,0.01 ,0.01 };
GLfloat blackPlasticSpec []={ 0.50 ,0.50 ,0.50 };
GLint blackPlasticCoef = 0.25 *128;


GLfloat  cyanPlasticAmb []={ 0.0 ,0.1 ,0.06 };
GLfloat  cyanPlasticDif []={   0.0 ,0.50980392 ,0.50980392 };
GLfloat  cyanPlasticSpec []={ 0.50196078 ,0.50196078 ,0.50196078 };
GLint  cyanPlasticCoef = 0.25 *128;

GLfloat greenPlasticAmb []={ 0.0 ,0.0 ,0.0 };
GLfloat greenPlasticDif []={   0.1 ,0.35 ,0.1 };
GLfloat greenPlasticSpec []={ 0.45 ,0.55 ,0.45 };
GLint greenPlasticCoef = 0.25 *128;

GLfloat  redPlasticAmb []={ 0.0 ,0.0 ,0.0 };
GLfloat  redPlasticDif []={   0.5 ,0.0 ,0.0 };
GLfloat  redPlasticSpec []={ 0.7 ,0.6 ,0.6 };
GLint  redPlasticCoef = 0.25 *128;

GLfloat whitePlasticAmb []={ 0.8 ,0.8 ,0.8 };
GLfloat whitePlasticDif []={   0.55 ,0.55 ,0.55 };
GLfloat whitePlasticSpec []={ 0.870 ,0.870 ,0.870 };
GLint whitePlasticCoef = 0.25 *128;

GLfloat yellowPlasticAmb []={ 0.0 ,0.0 ,0.0 };
GLfloat yellowPlasticDif []={   0.5 ,0.5 ,0.0 };
GLfloat yellowPlasticSpec []={ 0.60 ,0.60 ,0.50 };
GLint yellowPlasticCoef = 0.25 *128;

GLfloat  blackRubberAmb []={ 0.02 ,0.02 ,0.02 };
GLfloat  blackRubberDif []={   0.01 ,0.01 ,0.01 };
GLfloat  blackRubberSpec []={ 0.4 ,0.4 ,0.4 };
GLint  blackRubberCoef = 0.078125 *128;

GLfloat  cyanRubberAmb []={ 0.0 ,0.05 ,0.05 };
GLfloat  cyanRubberDif []={   0.4 ,0.5 ,0.5 };
GLfloat  cyanRubberSpec []={ 0.04 ,0.7 ,0.7 };
GLint  cyanRubberCoef = 0.078125 *128;

GLfloat  greenRubberAmb []={ 0.0 ,0.05 ,0.0 };
GLfloat  greenRubberDif []={   0.4 ,0.5 ,0.4 };
GLfloat  greenRubberSpec []={ 0.04 ,0.7 ,0.04 };
GLint  greenRubberCoef = 0.078125 *128;

GLfloat  redRubberAmb []={ 0.05 ,0.0 ,0.0 };
GLfloat  redRubberDif []={   0.5 ,0.4 ,0.4 };
GLfloat  redRubberSpec []={ 0.7 ,0.04 ,0.04 };
GLint  redRubberCoef = 0.078125 *128;

GLfloat  whiteRubberAmb []={ 0.05 ,0.05 ,0.05 };
GLfloat  whiteRubberDif []={   0.5 ,0.5 ,0.5 };
GLfloat  whiteRubberSpec []={ 0.7 ,0.7 ,0.7 };
GLint  whiteRubberCoef = 0.078125 *128;

GLfloat yellowRubberAmb []={ 0.05 ,0.05 ,0.0 };
GLfloat yellowRubberDif []={   0.5 ,0.5 ,0.4 };
GLfloat yellowRubberSpec []={ 0.7 ,0.7 ,0.04 };
GLint yellowRubberCoef = 0.078125 *128;



