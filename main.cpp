#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <IL/il.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>

#include "structs.h"

#include "motor.h"
#include "parser.h"
#include "catmul.h"

using namespace std;

extern int yylex();
extern int yylineno;
extern char* yytext;

int nModelos;
int nNormais;
int nPtsTexturas;

int nTexturas;
int nPatchs;
int nTranslacoes;
int nLuzes;

vector< vector<Ponto> > translacoes;

vector<int> nCoordModelos;
vector<int> nCoordNormais;
vector<int> nCoordPtsTexturas;

vector <Instrucao> 	instrucoes;
vector <Modelo> 	modelosVBO;
vector <Patch>		patches;
vector <Luz> 		luzes;

int translActual;

float inclinacaoA = 0.0;
float inclinacaoD = 0.0;
int face, modo;

float angX = 0.0;
float angY = 0.0;
float alt = 1.0;
float movX = 0.0;
float movZ = 0.0;

float anguloH = M_PI;
float anguloV = 0.0;
float camX=0.0f, camY= 0.0f, camZ=0.0f;

float x_tela, y_tela;
float posX,posY;
static int estado_botao=0;

float eyeX = sin(anguloH)*cos(anguloV);
float eyeZ = cos(anguloH)*cos(anguloV);
float eyeY = sin(anguloV);

long currentTime = 0;

vector<float> temposTransl;

int patchActual = 0;
int indiceActual = 0;

int ptsTransV[3];
int ptsTrans = 0;

Rotacao* auxR;
Ponto*	 auxP;
Luz*	 auxL;


int recolhePts = 0;

// Vector onde vão ser guardados 
// os pontos da translação com tempo
vector<Ponto> translTempPts;

unsigned int LOD=20;

////////////////


void changeSize(int w, int h) 
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0) h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void renderScene(void)
{
	static float t = 0;
	float res[3];
	float res2[3];

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(	
				camX, 		camY, 		camZ,
				camX+eyeX, 	camY+eyeY, 	camZ+eyeZ,
				0.0f,		1.0f, 		0.0f
			);

	currentTime = glutGet(GLUT_ELAPSED_TIME);

	// pôr instruções de desenho aqui

	glPolygonMode(face, modo);
	glTranslatef(movX, 0.0, movZ);
	glRotatef(angX,0.0f,1.0f,0.0f);
	glRotatef(angY,1.0f,0.0f,0.0f);

	vector<Ponto> auxV;

	vector<Instrucao>::iterator it;
	


	for (it = instrucoes.begin(); it < instrucoes.end(); it++)
	{
		switch(it -> tipo)
		{
			case	INSTMODELF :
								{
									cout << "MODELO Nº "<< it->rot->eixoX << " :: " << endl;
									cout << "Número de Coordenadas: " << nCoordModelos.at( (int) it -> rot -> eixoX) << endl;

									//glColor3f(0.3,0.2,1);

									glBindBuffer(GL_ARRAY_BUFFER, modelosVBO.at( (int) it -> rot -> eixoX ).buffers[0] );
									glVertexPointer(3,GL_FLOAT, 0, 0);

									glBindBuffer(GL_ARRAY_BUFFER, modelosVBO.at( (int) it -> rot -> eixoX ).buffers[1] );
								    glNormalPointer(GL_FLOAT,0,0);


									glDrawArrays(GL_TRIANGLES, 0, nCoordModelos.at( (int) it -> rot -> eixoX) );
									glDrawArrays(GL_TRIANGLES, 0, nCoordNormais.at( (int) it -> rot -> eixoX) );

									float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};

									glMaterialfv(GL_FRONT, GL_POINT, white);

									break;
								}
			case 	INSTMODELP	:
								{
									cout << "MODELO PATCH::" << endl;

									glColor3f(0.3,0.2,1);

									glBegin(GL_TRIANGLES);

									// use the parametric time value 0 to 1
									for(int i=0;i!=LOD;++i) 
									{
										// calculate the parametric u value
										float u = (float)i/(LOD-1);

										if(patchActual > 31)	patchActual = 0;

										for(int j=0;j!=LOD;++j) 
										{
											// calculate the parametric v value
											float v = (float)j/(LOD-1);

											// calculate the point on the surface

											for (int i = 0; i < 32; ++i)
											{
												Ponto p = Calculate(u, v, patches.at(0).indices.at(i));

												// draw point
												glVertex3f(p.x,p.y,p.z);
											}
											//patchActual++;
										}
									}
									glEnd();
								  	
								  	break;
								}

			case 	INSTMODELC	:
								{
									cout << "MODELO COM COR:: " << endl;
									cout << "Número de Coordenadas: " << nCoordModelos.at( (int) it -> rot -> eixoX ) << endl;
									cout << "Cor vermelha: " << it -> rot -> eixoY << endl;
									cout << "Cor verde: " 	 << it -> rot -> eixoZ << endl;
									cout << "Cor azul: " 	 << it -> rot -> angulo << endl;


									//glColor3f(it -> rot -> eixoY, it -> rot -> eixoZ, it -> rot -> angulo);

									float matt[4] = 
													{
														it -> rot -> eixoY, 
														it -> rot -> eixoZ, 
														it -> rot -> angulo,
														1.0
													};

									glMaterialfv(GL_FRONT, GL_DIFFUSE, matt);

									glBindBuffer(GL_ARRAY_BUFFER, modelosVBO.at( (int) it -> rot -> eixoX  ).buffers[0] );
									glVertexPointer(3,GL_FLOAT, 0, 0);
									glDrawArrays(GL_TRIANGLES, 0, nCoordModelos.at( it -> rot -> eixoX)  );

									break;
								}

			case 	INSTMODELT	:
								{
									cout << "MODELO COM TEXTURA:: " << endl;
									cout << "Número de Coordenadas: " << nCoordModelos.at( (int) it -> rot -> eixoX) << endl;

									//glColor3f(0.3,0.2,1);

									glBindBuffer(GL_ARRAY_BUFFER, modelosVBO.at( 		(int) it -> rot -> eixoX  ).buffers[0] 	);
									glVertexPointer(3,GL_FLOAT, 0, 0);

									glBindBuffer(GL_ARRAY_BUFFER, modelosVBO.at( 		(int) it -> rot -> eixoX  ).buffers[1] 	);
									glNormalPointer(GL_FLOAT, 0, 0);

									glBindBuffer(GL_ARRAY_BUFFER, modelosVBO.at( 		(int) it -> rot -> eixoX ).buffers[2] 	);
									glTexCoordPointer(2, GL_FLOAT, 0, 0);

									glDrawArrays(GL_TRIANGLES, 0, nCoordModelos.at( 	(int) it -> rot -> eixoX) 				);
									glDrawArrays(GL_TRIANGLES, 0, nCoordNormais.at( 	(int) it -> rot -> eixoX) 				);
									glDrawArrays(GL_TRIANGLES, 0, nCoordPtsTexturas.at( (int) it -> rot -> eixoX) 				);

									glBindTexture(GL_TEXTURE_2D, it -> rot -> eixoY);
									
									float white[4] = {1,1,1,1};
									glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
								    
									//glDrawArrays(GL_TRIANGLES, 0, 50);
									glDrawArrays(GL_TRIANGLES, 0, nCoordModelos.at( (int) it -> rot -> eixoX) );
									glBindTexture(GL_TEXTURE_2D,0);


								    //Para nehuma textura estar seleccionada
								    glBindTexture(GL_TEXTURE_2D,0);

									break;
								}

			case 	INSTRPUSH	: 	cout << "PUSH:: " << "\n"; glPushMatrix(); break;

			case 	INSTRPOP 	: 	cout << "POP:: " << "\n"; glPopMatrix();  break;

			case 	INSTTRANSTEMP	:
									{
										translActual = it -> rot -> eixoX;

										int pointNum = (translacoes.at(translActual)).size();

										renderCatmullRomCurve(res, pointNum);
										//getGlobalCatmullRomPoint(1, t * temposTransl.at(translActual)), res2, pointNum);
										getGlobalCatmullRomPoint(1, t , res2, pointNum);
										glTranslatef(res2[0], res2[1], res2[2]);
										//glutWireTeapot(0.1);


										break;
									}

			case 	INSTRLUZPOINT	:
									{
										cout << "LUZ TIPO PONTO::" 									<< endl;
										cout << "Posicao no array = "	<< it -> rot -> eixoX 		<< endl;
										cout << "posX = "	<< (luzes.at(it -> rot -> eixoX)).posX 	<< endl;
										cout << "posY = "	<< (luzes.at(it -> rot -> eixoX)).posY 	<< endl;
										cout << "posZ = "	<< (luzes.at(it -> rot -> eixoX)).posZ 	<< endl;

										GLfloat pos[4] = 
														{ 	
															( (luzes.at(it -> rot -> eixoX)).posX ), 
												 			( (luzes.at(it -> rot -> eixoX)).posY ),
												 			( (luzes.at(it -> rot -> eixoX)).posZ ),
											 				1
												 	 	};

										//GLfloat amb[3] 	= { 0.2, 0.2, 02 	};												
										//GLfloat diff[3] = { 1.0, 1.0, 1.0 	};												


										glLightfv( GL_LIGHT0, GL_POSITION, 	pos  ); 
										//glLightfv( GL_LIGHT0, GL_AMBIENT, 	amb  ); 
										//glLightfv( GL_LIGHT0, GL_DIFFUSE, 	diff ); 

										break;
									}

			case 	INSTRLUZDIREC	: 
									{
										cout << "LUZ TIPO DIRECIONAL::" 							<< endl;
										cout << "Posicao no array = "	<< it -> rot -> eixoX 		<< endl;
										cout << "posX = "	<< (luzes.at(it -> rot -> eixoX)).posX 	<< endl;
										cout << "posY = "	<< (luzes.at(it -> rot -> eixoX)).posY 	<< endl;
										cout << "posZ = "	<< (luzes.at(it -> rot -> eixoX)).posZ 	<< endl;

										GLfloat pos[4] = 
														{ 	
															( (luzes.at(it -> rot -> eixoX)).posX ), 
												 			( (luzes.at(it -> rot -> eixoX)).posY ),
												 			( (luzes.at(it -> rot -> eixoX)).posZ ),
											 				0
												 	 	};

										GLfloat amb[3] 	= { 0.2, 0.2, 02 	};												
										GLfloat diff[3] = { 1.0, 1.0, 1.0 	};												


										glLightfv( GL_LIGHT0, GL_POSITION, 	pos  ); 
										glLightfv( GL_LIGHT0, GL_AMBIENT, 	amb  ); 
										glLightfv( GL_LIGHT0, GL_DIFFUSE, 	diff ); 

										break;
									}

			case INSTRLUZSPOT		:
									{
										cout << "LUZ TIPO SPOT::" 				<< endl;
										cout << "Posicao no array = "	<<   it -> rot -> eixoX				<< endl;
										cout << "posX = "		<< (luzes.at(it -> rot -> eixoX)).posX 		<< endl;
										cout << "posY = "		<< (luzes.at(it -> rot -> eixoX)).posY 		<< endl;
										cout << "posZ = "		<< (luzes.at(it -> rot -> eixoX)).posZ 		<< endl;
										cout << "angulo = "		<< (luzes.at(it -> rot -> eixoX)).angulo 	<< endl;
										cout << "atenuacao = "	<< (luzes.at(it -> rot -> eixoX)).atenuacao << endl;
										cout << "dirX = "		<< (luzes.at(it -> rot -> eixoX)).dirX 		<< endl;
										cout << "dirY = "		<< (luzes.at(it -> rot -> eixoX)).dirY 		<< endl;
										cout << "dirZ = "		<< (luzes.at(it -> rot -> eixoX)).dirZ 		<< endl;
										

										//GLfloat diff[3] = { 1.0, 1.0, 1.0 	};												

										GLfloat pos[4] = 
														{ 	
															( (luzes.at(it -> rot -> eixoX)).posX ), 
												 			( (luzes.at(it -> rot -> eixoX)).posY ),
												 			( (luzes.at(it -> rot -> eixoX)).posZ ),
											 				1
												 	 	};

										GLfloat spotDir[3] 	= 
															{ 
																( (luzes.at(it -> rot -> eixoX)).dirX ), 
													 			( (luzes.at(it -> rot -> eixoX)).dirY ),
													 			( (luzes.at(it -> rot -> eixoX)).dirZ ),
															};	

										glLightfv( GL_LIGHT0, GL_POSITION, 			pos  	); 
										//glLightfv( GL_LIGHT0, GL_DIFFUSE, 			diff 	); 
										glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, 	spotDir );
										glLightf ( GL_LIGHT0, GL_SPOT_CUTOFF, (luzes.at(it -> rot -> eixoX)).atenuacao );
										glLightf (GL_LIGHT0,GL_SPOT_EXPONENT, 		0.0);


										break;
									}

			case	INSTROTANG :
								{
									cout << "ROTAÇÃO ANGULO::" << "\n";
									cout << "angulo = "	<< it -> rot -> angulo << "\n";
									cout << "eixoX = "	<< it -> rot -> eixoX << "\n";
									cout << "eixoY = "	<< it -> rot -> eixoY << "\n";
									cout << "eixoZ = "	<< it -> rot -> eixoZ << "\n";
									
									glRotatef (	
												it -> rot -> angulo,
												it -> rot -> eixoX,
												it -> rot -> eixoY,
												it -> rot -> eixoZ
											  );

									break;
								}
			case	INSTROTEMP :
								{ 
									cout << "ROTAÇÃO TEMPO::" << "\n";
									cout << "tempo = "	<< it -> rot -> angulo << "\n";
									cout << "eixoX = "	<< it -> rot -> eixoX << "\n";
									cout << "eixoY = "	<< it -> rot -> eixoY << "\n";
									cout << "eixoZ = "	<< it -> rot -> eixoZ << "\n";
									
									glRotatef (	
												360*(currentTime % ((int) it -> rot -> angulo))/(it -> rot -> angulo),
												it -> rot -> eixoX,
												it -> rot -> eixoY,
												it -> rot -> eixoZ
											  );
									break;
								}
			case	INSTTRANS : 
								{
									cout << "TRANSLAÇÃO::" << "\n";
									cout << "X = "	<< it -> rot -> eixoX << "\n";
									cout << "Y = "	<< it -> rot -> eixoY << "\n";
									cout << "Z = "	<< it -> rot -> eixoZ << "\n";
	
									glTranslatef (	
												it -> rot -> eixoX,
												it -> rot -> eixoY,
												it -> rot -> eixoZ
											  );
	
									break;
								}
			case	INSTESCAL : 
								{
									cout << "ESCALA::" << "\n";
									cout << "X = "	<< it -> rot -> eixoX << "\n";
									cout << "Y = "	<< it -> rot -> eixoY << "\n";
									cout << "Z = "	<< it -> rot -> eixoZ << "\n";
									glScalef (	
												it -> rot -> eixoX,
												it -> rot -> eixoY,
												it -> rot -> eixoZ
											  );
									break;
								}
			default: printf("INSTRUÇÃO INVÁLIDA\n");
		}
	}

	//glEnd();

	// End of frame
	glutSwapBuffers();

	t+=0.001;
}

void teclado(int tecla, int x, int y)
{
	switch(tecla)
	{
		case GLUT_KEY_LEFT :
			anguloH +=0.05;
			eyeX = sin(anguloH)*cos(anguloV);
			eyeZ = cos(anguloH)*cos(anguloV);
			break;
		
		case GLUT_KEY_RIGHT :
			anguloH -=0.05;
			eyeX = sin(anguloH)*cos(anguloV);
			eyeZ = cos(anguloH)*cos(anguloV);
			break;

		case GLUT_KEY_UP :
			//if(anguloV+0.05<M_PI_2)	
			anguloV+=0.05;
			eyeX = sin(anguloH)*cos(anguloV);
			eyeZ = cos(anguloH)*cos(anguloV);
			eyeY = sin(anguloV);
			break;

		case GLUT_KEY_DOWN :
			//if(anguloV-0.05<M_PI_2)	
			anguloV-=0.05;
			eyeX = sin(anguloH)*cos(anguloV);
			eyeZ = cos(anguloH)*cos(anguloV);
			eyeY = sin(anguloV);
			break;
	}
	glutPostRedisplay();
}

void roda(unsigned char tecla, int x, int y)
{
	float fraction = 0.7f;
	
	switch (tecla)
	{
		case 'w':
			camX += fraction * sin(anguloH);
			camZ += fraction * cos(anguloH);
			camY += fraction * sin(anguloV);
			break;

		case 's':
			camX -= fraction * sin(anguloH);
			camZ -= fraction * cos(anguloH);
			camY -= fraction * sin(anguloV);
			break;

		case 'a':
			camX += fraction * sin(anguloH+M_PI_2);
			camZ += fraction * cos(anguloH+M_PI_2);
			break;

		case 'd':
			camX += fraction * sin(anguloH-M_PI_2);
			camZ += fraction * cos(anguloH-M_PI_2);
		break;
	}
	glutPostRedisplay();
}

void rato(int botao, int estado, int x, int y){
    if (botao==GLUT_LEFT_BUTTON){
        if (estado==GLUT_DOWN){
            estado_botao=1;
            x_tela=anguloH;
						posX=x;
            y_tela=anguloV;
						posY=y;
        }
        else{
            estado_botao=0;
						}

		}
}

void mov_rato(int x, int y){
    float teste;
    if(estado_botao){
        if(posX!=x)
						
            anguloH=x_tela+((posX-x)*0.005);
        
        if(posY!=y){
						
            anguloV=y_tela+((posY-y)*0.005);
        }
        

			eyeX = sin(anguloH)*cos(anguloV);
			eyeZ = cos(anguloH)*cos(anguloV);
			eyeY = sin(anguloV);

			//glutPostRedisplay();
    }
}

void menu(int id_op)
{
	if(id_op == 1){	face = GL_FRONT; modo = GL_LINE;  }
	if(id_op == 2){ face = GL_FRONT; modo = GL_FILL;  }
	if(id_op == 3){	face = GL_FRONT; modo = GL_POINT; }
	if(id_op == 4){ face = GL_BACK;  modo = GL_FILL;  }
}

int main(int argc, char **argv)
{
	int ntoken, vtoken;

	char* ficActual = NULL;

	nModelos 	 = 0;
	nNormais 	 = 0;
	nTexturas	 = 0;
	nPatchs  	 = 0;
	nTranslacoes = 0;
	nLuzes	 	 = 0;


	ptsTransV[0] = 0;
	ptsTransV[1] = 0;
	ptsTransV[2] = 0;


	/////////// GLUTING ///////////

	// inicialização
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1024,800);
	glutCreateWindow("GRUPO 3 @ CG - LCC");

	// registo de funções 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	// pôr aqui registo da funções do teclado e rato
	glutSpecialFunc(teclado);
	glutKeyboardFunc(roda);
	glutMouseFunc(rato);
	glutMotionFunc(mov_rato);


	// pôr aqui a criação do menu
	glutCreateMenu(menu);
	glutAddMenuEntry("FRONT_LINE", 1);
	glutAddMenuEntry("FRONT_FILL", 2);
	glutAddMenuEntry("FRONT_POINT", 3);
	glutAddMenuEntry("BACK_FILL", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	// init GLEW
	glewInit();


    ilInit();

	// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Texturas
	glEnable(GL_TEXTURE_2D);

	
	/////////// PARSING ///////////

	// PARSING DA PRIMEIRA LINHA
	ntoken = yylex();

	while(ntoken)
	{
		switch(ntoken)
		{
			case  CENA1			:	break;
			case  CENA2			:
								{
									//listaInst();

									// passar MODELOS para VBO's
									botaVBOs();

									// listaNCoords();

									glutMainLoop();
									break;
								}

			case  MODELOS1		:	break;
			case  MODELOS2		:	break;

			case  MODELOC		: 
								{
									ficActual = strdup(yytext+18);

									int i = 0;
									while(ficActual[i] != '"') i++;
									
									ficActual[i] = '\0';

									carregaModelo(ficActual);

									//Parsing das cores R G B

									auxR = (Rotacao*) malloc(sizeof(Rotacao));
									auxR -> eixoX 	= nModelos - 1;

									parseCor(yytext, auxR);

									addInst(auxR, INSTMODELC);
									free(ficActual);
									break;
								}
			case  MODELOF		:	
								{
									ficActual = strdup(yytext+18);
									ficActual[strlen(ficActual) - 4] = '\0';
								
									carregaModelo(ficActual);

									auxR = (Rotacao*) malloc(sizeof(Rotacao));
									auxR -> eixoX 	= nModelos - 1;
									auxR -> eixoY 	= -1;
									auxR -> eixoZ 	= -1;
									auxR -> angulo 	= -1;

									addInst(auxR, INSTMODELF);
									free(ficActual);
									break;
								}

			case MODELOP		:
								{
									ficActual = strdup(yytext+18);
									ficActual[strlen(ficActual) - 4] = '\0';
									carregaPatch(ficActual);

									auxR = (Rotacao*) malloc(sizeof(Rotacao));
									auxR -> eixoX 	= nPatchs - 1;
									auxR -> eixoY 	= -1;
									auxR -> eixoZ 	= -1;
									auxR -> angulo 	= -1;
									
									free(ficActual);

									addInst(auxR, INSTMODELP);
									break;
								}

			case MODELOT		:
								{
									string file = yytext;
									string textura = yytext;
									char* fileFile;
									char* texturaTextura;

									int i = 0;

									file.erase(file.begin(), file.begin()+18);									
									i = 0;
									while( file.at(i) != '"') i++;
									file.erase(file.begin()+i, file.end());		
									
									i = 0;
									while( textura.at(i) != '"') i++;
									i++;
									while( textura.at(i) != '"') i++;
									i++;
									while( textura.at(i) != '"') i++;
									i++;

									textura.erase(textura.begin(), textura.begin() + i);
									textura.erase(textura.end()-4, textura.end()      );

									fileFile 		= (char *) file.c_str();
									texturaTextura  = (char *) textura.c_str();
									
									carregaModelo(fileFile);
									carregaTextura(texturaTextura);

									auxR = (Rotacao*) malloc(sizeof(Rotacao));
									auxR -> eixoX 	= nModelos  - 1;
									auxR -> eixoY 	= nTexturas - 1;
									auxR -> eixoZ 	= -1;
									auxR -> angulo 	= -1;

									addInst(auxR, INSTMODELT);

									break;
								}

			case  GRUPO1		:	
								{
									auxR = (Rotacao*) malloc(sizeof(Rotacao));
									auxR -> eixoX 	= nModelos - 1;
									auxR -> eixoY 	= -1;
									auxR -> eixoZ 	= -1;
									auxR -> angulo 	= -1;

									addInst(auxR, INSTRPUSH);
									break;
								}

			case  GRUPO2		:	
								{
									auxR = (Rotacao*) malloc(sizeof(Rotacao));
									auxR -> eixoX 	= nModelos - 1;
									auxR -> eixoY 	= -1;
									auxR -> eixoZ 	= -1;
									auxR -> angulo 	= -1;

									addInst(auxR, INSTRPOP);
									break;
								}

			case  LUZES1		:	
								{
									cout << "LUZES ACTIVAS!!!!" << endl;

									glEnable(GL_LIGHTING);
								    glEnable(GL_LIGHT0);
									break;
								}

			case  LUZES2		:	break;


			case  LUZPOINT		:
								{
									auxR = (Rotacao*) malloc(sizeof(Rotacao));
									auxL = (Luz*)	  malloc(sizeof(Luz));

									parseLuz(yytext, auxL, LUZPOINT);

									auxR -> eixoX 	= nLuzes;
									auxR -> eixoY 	= -1;
									auxR -> eixoZ 	= -1;
									auxR -> angulo 	= -1;

									// Adicionar a luz ao vector luzes
									luzes.push_back(*auxL);

									nLuzes++;

									addInst(auxR, INSTRLUZPOINT);

									break;
								}

			case  LUZDIREC		:
								{
									auxR = (Rotacao*) malloc(sizeof(Rotacao));
									auxL = (Luz*)	  malloc(sizeof(Luz));

									parseLuz(yytext, auxL, LUZDIREC);

									auxR -> eixoX 	= nLuzes;
									auxR -> eixoY 	= -1;
									auxR -> eixoZ 	= -1;
									auxR -> angulo 	= -1;

									// Adicionar a luz ao vector luzes
									luzes.push_back(*auxL);

									nLuzes++;

									addInst(auxR, INSTRLUZDIREC);

									break;
								}

			case LUZSPOT		:
								{
									auxR = (Rotacao*) malloc(sizeof(Rotacao));
									auxL = (Luz*)	  malloc(sizeof(Luz));

									parseLuz(yytext, auxL, LUZSPOT);

									auxR -> eixoX 	= nLuzes;
									auxR -> eixoY 	= -1;
									auxR -> eixoZ 	= -1;
									auxR -> angulo 	= -1;

									// Adicionar a luz ao vector luzes
									luzes.push_back(*auxL);

									nLuzes++;

									addInst(auxR, INSTRLUZSPOT);

									break;
								}

			case  ROTACANG		:	
								{
									auxR = (Rotacao*) malloc(sizeof(Rotacao));
									parseRotacao(yytext, auxR, INSTROTANG);
									addInst(auxR, INSTROTANG);
									//free(auxR);
									break;
								}

			case  ROTACTEMP		:	
								{
									auxR = (Rotacao*) malloc(sizeof(Rotacao));
									parseRotacao(yytext, auxR, INSTROTEMP);
									
									addInst(auxR, INSTROTEMP);
									//free(auxR);
									break;
								}

			case  TRANSLACAO	:	cout << "TRANSLAÇÃO NORMAL" << endl;

									break;

			case  TRANSLTEMPI	:	
								{
									cout << "TRANSLAÇÃO DE TEMPO::::::" << endl;

									temposTransl.push_back( parseTempo(yytext) );
									recolhePts = 1;
									break;
								}

			case  TRANSLTEMPF	:	
								{	
									listaPontos(translTempPts);
									recolhePts = 0;

									nTranslacoes++;

									auxR = (Rotacao*) malloc(sizeof(Rotacao));
									auxR -> eixoX 	= nTranslacoes - 1;
									auxR -> eixoY 	= -1;
									auxR -> eixoZ 	= -1;
									auxR -> angulo 	= -1;

									translacoes.push_back(translTempPts);

									translTempPts.clear();

									addInst(auxR, INSTTRANSTEMP);
									break;
								}

			case PONTO 			:	
								{
									if(recolhePts == 1)
									{
										auxP = (Ponto*) malloc(sizeof(Ponto));

										parsePonto(yytext, auxP);

										translTempPts.push_back(*auxP);
									}

									break;
								}

			case  FIMTRANS		:	
								{
									if(ptsTrans > 3)	printf("Translação mal definida!\n");

									else
									{									
										auxR = (Rotacao*) malloc(sizeof(Rotacao));

										auxR -> eixoX 	= ptsTransV[0];
										auxR -> eixoY 	= ptsTransV[1];
										auxR -> eixoZ 	= ptsTransV[2];

										auxR -> angulo 	= -1;

										addInst(auxR, INSTTRANS);

										//free(auxR);

										ptsTrans 		= 0;
										ptsTransV[0] 	= 0;
										ptsTransV[1] 	= 0;
										ptsTransV[2] 	= 0;
									}
									break;
								}

			case  ESCALA		:	
								{
									auxR = (Rotacao*) malloc(sizeof(Rotacao));
									parseEscala(yytext, auxR);
									addInst(auxR, INSTESCAL);
									//free(auxR);
									break;
								}

			case  X				:	ptsTransV[0] = stof(yytext+2); ptsTrans++; break;
			case  Y				:	ptsTransV[1] = stof(yytext+2); ptsTrans++; break;
			case  Z				:	ptsTransV[2] = stof(yytext+2); ptsTrans++; break;

			case  PONTINHOS		:	break;
			case  INTEIRO		:	break;
			case  STRING		:	break;
			default : cout << "Erro, comando desconhecido!" << endl;
		}
		// PARSING DA LINHA SEGUINTE
		ntoken = yylex();
	}
	return 0;
}