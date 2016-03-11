#include <vector>
#include <GL/glew.h>


#define CENA1			1
#define CENA2			2
#define MODELOS1 		3
#define MODELOS2 		4
#define MODELOF			5
#define MODELOP			6
#define MODELOC			7
#define MODELOT			8
#define GRUPO1			9
#define GRUPO2			10
#define LUZES1			11
#define LUZES2			12

#define LUZPOINT		13
#define LUZDIREC		14
#define LUZSPOT			15

#define FICHEIRO		21
#define PONTO			22
#define TRANSLACAO		23
#define ESCALA			24
#define ROTACANG		25
#define ROTACTEMP		26

#define PONTINHOS		27
#define INTEIRO			28
#define STRING			29

#define	EIXOX			30
#define	EIXOY			31
#define	EIXOZ			32

#define	X				33
#define	Y				34
#define	Z				35

#define ANGXYZ			40

#define FIMTRANS		50
#define TRANSLTEMPI		51
#define TRANSLTEMPF		52

#define INSTTRANS		61
#define INSTTRANSTEMP	62
#define INSTESCAL		63
#define INSTMODELF		64
#define INSTMODELP		65
#define INSTMODELC		66
#define INSTMODELT		67
#define INSTRPUSH		68
#define INSTRPOP		69
#define INSTROTANG		70
#define INSTROTEMP		71
#define INSTRLUZPOINT	72
#define INSTRLUZDIREC	73
#define INSTRLUZSPOT	74

#define STAR_LIST		200


using namespace std;

typedef struct sPonto
{
	float x;
	float y;
	float z;

	} Ponto;

typedef struct sRotacao
{
	float angulo;
	float eixoX; 
	float eixoY;
	float eixoZ;

	} Rotacao;


typedef struct sLuz
{
	int tipo;
	float posX, posY, posZ;
	float angulo;
	float atenuacao;
	float dirX, dirY, dirZ;
	float cor;

	} Luz;

typedef struct sInstrucao
{
	int tipo;
	Rotacao* rot;

	} Instrucao;

typedef struct sPatch
{
	int numPatchs;
	vector< vector<int> > indices;
	int numCPoints;
	vector<Ponto> CPoints;

	} Patch;

typedef struct sModelo
{
	GLuint buffers[3];
	} Modelo;