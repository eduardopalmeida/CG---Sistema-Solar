#include "structs.h"
#include "motor.h"
#include <IL/il.h>

extern vector<Instrucao>instrucoes;
extern vector<Patch>	patches;
extern vector<Luz>		luzes;

extern int nPatchs;
extern int nModelos;
extern int nNormais;
extern int nPtsTexturas;

extern int nTexturas;

extern vector<int> nCoordModelos;
extern vector<int> nCoordNormais;
extern vector<int> nCoordPtsTexturas;

extern vector<Modelo> modelosVBO;

unsigned int tw, th, texID;
unsigned char* texData;

vector < vector<Ponto> > modelos;
vector < vector<Ponto> > normais;
vector < vector<Ponto> > ptsTexturas;

void carregaModelo(char* file)
{
	float x, y, z;

	int nCoordVert =  0;
	int nCoordNorm =  0;
	int nCoordText = -1; // Caso não tenha pontos de textura: PLANO, PARALELO, CONE

	vector<Ponto> auxV;
	vector<Ponto> auxN;
	vector<Ponto> auxT;

	Ponto auxPonto;

	ifstream source;

	source.open(file,ios_base::in);

	// CARREGA VÉRTICES
	source >> nCoordVert;

	//cout << endl << "Nº DE COORDENADAS DOS VÈRTICES:: " << nCoordVert << endl;

	for (int i = 0; i < nCoordVert/3; i++)
	{
		source >> auxPonto.x;
		source >> auxPonto.y;
		source >> auxPonto.z;

		//cout << "PONTO:: (" << auxPonto.x << ", " << auxPonto.y << ", " << auxPonto.z << ")" << endl;

		auxV.push_back(auxPonto);
	}

	modelos.push_back(auxV);

	nCoordModelos.push_back(nCoordVert);

	nModelos++;

	// CARREGA NORMAIS
	source >> nCoordNorm;

	//cout << endl << "Nº DE COORDENADAS DAS NORMAIS:: " << nCoordNorm << endl;

	for (int i = 0; i < nCoordNorm/3; i++)
	{
		source >> auxPonto.x;
		source >> auxPonto.y;
		source >> auxPonto.z;

		//cout << "PONTO:: (" << auxPonto.x << ", " << auxPonto.y << ", " << auxPonto.z << ")" << endl;

		auxN.push_back(auxPonto);
	}

	normais.push_back(auxN);

	nCoordNormais.push_back(nCoordNorm);

	nNormais++;

	// CARREGA TEXTURAS ------ > ESFERA
	if(	source >> nCoordText )
	{
		//cout << endl << "Nº DE COORDENADAS DAS TEXTURAS:: " << nCoordText << endl;

		for (int i = 0; i < nCoordText/2; i++)
		{
			source >> auxPonto.x;
			source >> auxPonto.y;
			auxPonto.z = -1;

			//cout << "PONTO:: (" << auxPonto.x << ", " << auxPonto.y << ")" << endl;

			auxT.push_back(auxPonto);
		}

		ptsTexturas.push_back(auxT);
	}
	else
	{
		auxPonto.x = auxPonto.y = auxPonto.z = -1;
		auxT.push_back(auxPonto);
		ptsTexturas.push_back(auxT);
	}

	nPtsTexturas++;

	nCoordPtsTexturas.push_back(nCoordText);
}


void carregaTextura(char* file)
{	
	unsigned int lalala;
	
	ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
 
	ilGenImages(1, &lalala);
    ilBindImage(lalala);

    ilLoadImage( (ILstring) file);

    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    
    glGenTextures(1, &texID);

    // !!!!!!! // texID -------> rot -> eixoY
    glBindTexture(GL_TEXTURE_2D, nTexturas);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	
	nTexturas++;
}

void carregaPatch(char* file)
{
	char x[30];

	Patch auxPatch;

	ifstream source;
	source.open(file, ios_base::in);

	// Num de patchs
	source >> ( auxPatch.numPatchs );

	//cout << "NUM PATCHS:" << (auxPatch.numPatchs) << "##" << endl;
	/*
    glEnableClientState(GL_VERTEX_ARRAY);
	buffers = (GLuint*) malloc(sizeof(GLuint)*numPatchs);
    glGenBuffers(numPatchs, buffers);
	*/

	// INDICES
	for (int j = 0; j < (auxPatch.numPatchs); j++)
	{
		vector<int> auxList;

		for (int i = 0; i < 15; i++)
		{
			source >> x;
			x[strlen(x) - 1] = '\0';
			//cout << "#" << x << "# ";

			auxList.push_back( stoi(x) );
		}

		source >> x;
		//cout << "#" << x << "# ";

		auxList.push_back(stoi(x));

		(auxPatch.indices).push_back(auxList);

		//cout << endl;
	}

	// Num de pontos de controlo
	source >> (auxPatch.numCPoints);

	//cout << "NUM PONTOS DE CONTROLO:" << (auxPatch.numCPoints) << "##" << endl;

	// Pontos
	vector<Ponto> auxPontos;

	for (int i = 0; i < (auxPatch.numCPoints); i++)
	{
		Ponto auxP;

		source >> x; x[strlen(x) - 1] = '\0'; auxP.x = stof(x);
		source >> x; x[strlen(x) - 1] = '\0'; auxP.y = stof(x);
		source >> x; x[strlen(x)	] = '\0'; auxP.z = stof(x);

		auxPontos.push_back(auxP);
	}

	listaPontos(auxPontos);

	auxPatch.CPoints = auxPontos;

	patches.push_back(auxPatch);

	nPatchs++;
}


void botaVBOs()
{
	int vertex = 0;
	int temTexturas = 0;

	cout << "Carregando VBO's" << endl;

    // 3 = 1 + 1 + 1
    // BUFFER DOS VÉRTICES + BUFFER DAS NORMAIS + BUFFER DOS PTS DE TEXTURA


	for (int i = 0; i < nModelos; i++)
	{

		// O primeiro ponto de cada modelo é o 
		// número de coordenadas a ser carregado

		int nCoordV = nCoordModelos.at(i);
		int nCoordN = nCoordNormais.at(i);
		int nCoordT = nCoordPtsTexturas.at(i);

		float *bVertices 	= (float*) malloc(sizeof(float) * nCoordV);
		float *bNormais 	= (float*) malloc(sizeof(float) * nCoordN);
		float *bPtsTextura 	= (float*) malloc(sizeof(float) * nCoordT);

		int k ;

		Modelo auxM;

		if(nCoordT == -1)
			glGenBuffers(2, auxM . buffers);
		else
			glGenBuffers(3, auxM . buffers);

		temTexturas = 0;


		k = 0;

		for (int j = 0; j < (nCoordV/3); j++)
		{	
			// Carrega vértices para VBO
			bVertices[k + 0] = (modelos.at(i)).at(j).x;
			bVertices[k + 1] = (modelos.at(i)).at(j).y;
			bVertices[k + 2] = (modelos.at(i)).at(j).z;
			
			k += 3;
		}

        glBindBuffer(GL_ARRAY_BUFFER, auxM.buffers[0] );
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nCoordV, bVertices, 	GL_STATIC_DRAW);

				
		//cout << "Nº DE COORDENADAS NORMAIS:::" << nCoordN << endl
		//	 << "Nª DE PONTOS:::" << nCoordN/3 << endl;

		k = 0;

		for (int j = 0; j < (nCoordN/3); j++)
		{
			// Carrega Nórmais para VBO
			bNormais[k + 0] = (normais.at(i)).at(j).x;
			bNormais[k + 1] = (normais.at(i)).at(j).y;
			bNormais[k + 2] = (normais.at(i)).at(j).z;
			
			k += 3;
		}

        glBindBuffer(GL_ARRAY_BUFFER, auxM.buffers[1] );
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nCoordN, bNormais, 	GL_STATIC_DRAW);


		k = 0;

		if(nCoordT > 0)
		{
			//cout << "Nº DE COORDENADAS DE PONTOS DE TEXTURA:::" << nCoordT << endl
			//	 << "Nª DE PONTOS:::" << nCoordT/2 << endl;

			for (int j = 0; j < (nCoordT/2); j++)
			{	
				// Carrega Pontos de Textura para VBO
				bPtsTextura[k + 0] = (ptsTexturas.at(i)).at(j).x;
				bPtsTextura[k + 1] = (ptsTexturas.at(i)).at(j).y;

				k += 2;
			}

	        glBindBuffer(GL_ARRAY_BUFFER, auxM.buffers[2] );
	        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nCoordT, bPtsTextura, GL_STATIC_DRAW);
		}


		modelosVBO.push_back(auxM);

		free(bVertices);
		free(bNormais);
		free(bPtsTextura);
	}
}



void addInst(Rotacao* auxR, int tipo)
{
	Instrucao auxInstr;

	auxInstr.rot = auxR;
	auxInstr.tipo = tipo;

	instrucoes.push_back(auxInstr);
}

void listaInst()
{
	vector<Instrucao>::iterator it;

	cout << "INSTRUÇÕES!!!!" << endl;

	for (it = instrucoes.begin(); it < instrucoes.end(); it++)
	{
		switch(it -> tipo)
		{
			case	INSTROTANG	: cout << "ROTAÇÃO ANGULO::" << "\n";
								  cout << "angulo = "	<< it -> rot -> angulo << "\n";
								  cout << "eixoX = "	<< it -> rot -> eixoX << "\n";
								  cout << "eixoY = "	<< it -> rot -> eixoY << "\n";
								  cout << "eixoZ = "	<< it -> rot -> eixoZ << "\n";
								  break;

			case	INSTROTEMP	: cout << "ROTAÇÃO TEMPO::" << "\n";
								  cout << "tempo = "	<< it -> rot -> angulo << "\n";
								  cout << "eixoX = "	<< it -> rot -> eixoX << "\n";
								  cout << "eixoY = "	<< it -> rot -> eixoY << "\n";
								  cout << "eixoZ = "	<< it -> rot -> eixoZ << "\n";
								  break;


			case	INSTTRANS 	:   cout << "TRANSLAÇÃO::" << "\n";
									cout << "X = "	<< it -> rot -> eixoX << "\n";
								    cout << "Y = "	<< it -> rot -> eixoY << "\n";
									cout << "Z = "	<< it -> rot -> eixoZ << "\n";
									break;

			case 	INSTTRANSTEMP:	cout << "TRANSLAÇÃO TEMPO:: "		 << "\n";
									//cout << "PONTOS DE CONTROL:::::: "	 << "\n";
									//listaPontos(translacoes.at(it->rot->eixoX));
									break;

			case	INSTESCAL 	:	cout << "ESCALA::" << "\n";
									cout << "X = "	<< it -> rot -> eixoX << "\n";
									cout << "Y = "	<< it -> rot -> eixoY << "\n";
									cout << "Z = "	<< it -> rot -> eixoZ << "\n";
									break;
			
			case	INSTMODELF	:	cout << "MODELO 3D::" << "\n";
									cout << "modelo número = " << it -> rot -> eixoX << "\n";
									break;

			case 	INSTMODELP	:	cout << "MODELO PATCH::" << "\n";									
									break;

			case 	INSTMODELC	:	cout << "MODELO COM COR:" << "\n";
									break;

			case	INSTRLUZPOINT	:	cout << "LUZ TIPO PONTO::" 					<< endl;
										cout << "Posicao no array = "	<< it -> rot -> eixoX 	<< endl;
										cout << "posX = "	<< (luzes.at(it -> rot -> eixoX)).posX 	<< endl;
										cout << "posY = "	<< (luzes.at(it -> rot -> eixoX)).posY 	<< endl;
										cout << "posZ = "	<< (luzes.at(it -> rot -> eixoX)).posZ 	<< endl;
										break;

			case	INSTRLUZDIREC	:	cout << "LUZ TIPO DIRECT::" 				<< endl;
										cout << "Posicao no array = "	<< it -> rot -> eixoX 	<< endl;
										cout << "posX = "	<< (luzes.at(it -> rot -> eixoX)).posX 	<< endl;
										cout << "posY = "	<< (luzes.at(it -> rot -> eixoX)).posY 	<< endl;
										cout << "posZ = "	<< (luzes.at(it -> rot -> eixoX)).posZ 	<< endl;
										break;
		
			case	INSTRLUZSPOT	:	cout << "LUZ TIPO SPOT::" 				<< endl;
										cout << "Posicao no array = "	<<   it -> rot -> eixoX				<< endl;
										cout << "posX = "		<< (luzes.at(it -> rot -> eixoX)).posX 		<< endl;
										cout << "posY = "		<< (luzes.at(it -> rot -> eixoX)).posY 		<< endl;
										cout << "posZ = "		<< (luzes.at(it -> rot -> eixoX)).posZ 		<< endl;
										cout << "angulo = "		<< (luzes.at(it -> rot -> eixoX)).angulo 	<< endl;
										cout << "atenuacao = "	<< (luzes.at(it -> rot -> eixoX)).atenuacao << endl;
										cout << "dirX = "		<< (luzes.at(it -> rot -> eixoX)).dirX 		<< endl;
										cout << "dirY = "		<< (luzes.at(it -> rot -> eixoX)).dirY 		<< endl;
										cout << "dirZ = "		<< (luzes.at(it -> rot -> eixoX)).dirZ 		<< endl;
										break;
		


			case 	INSTRPUSH	: 	cout << "PUSH::" << "\n";
									break;

			case 	INSTRPOP 	: 	cout << "POP::" << "\n";
									break;

			default: printf("INSTRUÇÃO INVÁLIDA\n");
		}
		cout << "\n";
	}
}

void listaPontos(vector<Ponto> pts)
{
	int tam = pts.size();

	printf("TAMANHO: %d\n", tam);

	for (int i = 0; i < tam; i++)
		cout << "(" << pts.at(i).x << ",\t "
			 		<< pts.at(i).y << ",\t " 
			 		<< pts.at(i).z << ")\n";
}