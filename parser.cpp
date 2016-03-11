#include <iostream> 
#include <string> 
#include "structs.h"
#include "parser.h"

void parseEscala(string com, Rotacao* rot)
{
	string sX, sY, sZ;
	int i;

	com.erase(com.begin(), com.begin()+10);
	i = 0;
	while( com.at(i) != ' ') i++;
	sX.append(com.begin(), com.begin() + i);

	com.erase(com.begin(), com.begin()+ i + 3);
	i = 0;
	while( com.at(i) != ' ') i++;
	sY.append(com.begin(), com.begin() + i);

	com.erase(com.begin(), com.begin()+ i + 3);
	i = 0;
	while( com.at(i) != ' ') i++;
	sZ.append(com.begin(), com.begin() + i);

	rot -> eixoX 	= stof(sX);
	rot -> eixoY 	= stof(sY);
	rot -> eixoZ 	= stof(sZ);

	rot -> angulo = -1;
}

void parseRotacao(string com, Rotacao* rot, int tipo)
{
	string angulo, eixoX, eixoY, eixoZ;
	int i;

	if(tipo == INSTROTANG)
	{
		com.erase(com.begin(), com.begin()+18);
		i = 0;
		while( com.at(i) != ' ') i++;
		angulo.append(com.begin(), com.begin() + i);		
	}

	if (tipo == INSTROTEMP)
	{
		com.erase(com.begin(), com.begin()+17);
		i = 0;
		while( com.at(i) != ' ') i++;
		angulo.append(com.begin(), com.begin() + i);		
	}

	com.erase(com.begin(), com.begin()+ i + 7);
	i = 0;
	while( com.at(i) != ' ') i++;
	eixoX.append(com.begin(), com.begin() + i);

	com.erase(com.begin(), com.begin()+ i + 7);
	i = 0;
	while( com.at(i) != ' ') i++;
	eixoY.append(com.begin(), com.begin() + i);

	com.erase(com.begin(), com.begin()+ i + 7);
	i = 0;
	while( com.at(i) != ' ') i++;
	eixoZ.append(com.begin(), com.begin() + i);


	if(tipo == INSTROTANG)	rot -> angulo	= stof(angulo);

	if(tipo == INSTROTEMP)	rot -> angulo	= stof(angulo) * 1000;

	rot -> eixoX 	= stof(eixoX);
	rot -> eixoY 	= stof(eixoY);
	rot -> eixoZ 	= stof(eixoZ);
}

void parsePonto(string com, Ponto* auxP)
{
	string sX, sY, sZ;
	int i;

	//cout << "P";

	com.erase(com.begin(), com.begin()+9);
	i = 0;

	while( com.at(i) != ' ') i++;
	sX.append(com.begin(), com.begin() + i);

	com.erase(com.begin(), com.begin()+ i + 3);
	i = 0;
	while( com.at(i) != ' ') i++;
	sY.append(com.begin(), com.begin() + i);

	com.erase(com.begin(), com.begin()+ i + 3);
	i = 0;
	while( com.at(i) != ' ') i++;
	sZ.append(com.begin(), com.begin() + i);

	/*
	cout << "PONTOOOOOOOOOOOOO" << endl;
	cout << "X= " << stof(sX) << endl;
	cout << "Y= " << stof(sY) << endl;
	cout << "Z= " << stof(sZ) << endl;
	*/

	auxP -> x 	= stof(sX);
	auxP -> y 	= stof(sY);
	auxP -> z 	= stof(sZ);
}

int parseTempo(string com)
{
	string str;
	int i = 0;

	com.erase(com.begin(), com.begin()+20);
	i = 0;

	while( com.at(i) != ' ') i++;
	str.append(com.begin(), com.begin() + i);


	//cout << "TEMPOOOOO " << stof(str) << endl;
	//cout << "TEMPOOOOO " << stof(str) << endl;


	return stof(str);
}


void parseCor(string com, Rotacao* rot)
{
	string sX, sY, sZ;
	int i = 0;
	while(com.at(i) != ' ') i++;
	com.erase(com.begin(), com.begin()+i);

	while(com.at(i) != ' ') i++;
	com.erase(com.begin(), com.begin()+i+7);

	i = 0;
	while( com.at(i) != ' ') i++;
	sX.append(com.begin(), com.begin() + i);

	com.erase(com.begin(), com.begin()+ i + 7);
	i = 0;
	while( com.at(i) != ' ') i++;
	sY.append(com.begin(), com.begin() + i);

	com.erase(com.begin(), com.begin()+ i + 7);
	i = 0;
	while( com.at(i) != ' ') i++;
	sZ.append(com.begin(), com.begin() + i);

	rot -> eixoY 	= stof(sX);
	rot -> eixoZ 	= stof(sY);
	rot -> angulo  = stof(sZ);

}

void parseLuz(string com, Luz* luz, int tipoLuz)
{
	string sTipo;
	string pX, pY, pZ, dX, dY, dZ;
	string angulo, atenuacao, cor;

	int auxTipo;
	int i = 0;

	// PONTO

	if(tipoLuz == LUZPOINT)
	{
		// LIXO
		while(com.at(i) != '"') i++;
		com.erase(com.begin(), com.begin()+i);

		// TIPO DA LUZ
		i = 1;
		while(com.at(i) != '"') i++;
		sTipo.append(com.begin() +1, com.begin() + i);

		// COORDENADAS DA LUZ
		while(com.at(i) != '=') i++;
		com.erase(com.begin(), com.begin()+i+1);

		i = 0;
		while( com.at(i) != ' ') i++;
		pX.append(com.begin(), com.begin() + i);

		com.erase(com.begin(), com.begin()+ i + 6);
		i = 0;
		while( com.at(i) != ' ') i++;
		pY.append(com.begin(), com.begin() + i);

		com.erase(com.begin(), com.begin()+ i + 6);
		i = 0;
		while( com.at(i) != ' ') i++;
		pZ.append(com.begin(), com.begin() + i);
		

		luz -> tipo = LUZPOINT;
		luz -> posX = stof(pX);
		luz -> posY = stof(pY);
		luz -> posZ = stof(pZ);
		luz -> angulo 	 = -1;
		luz -> atenuacao = -1;
		luz -> cor 		 = -1;
	}

	if(tipoLuz == LUZDIREC)
	{
		// LIXO
		while(com.at(i) != '"') i++;
		com.erase(com.begin(), com.begin()+i);

		// TIPO DA LUZ
		i = 1;
		while(com.at(i) != '"') i++;
		sTipo.append(com.begin() +1, com.begin() + i);

		// COORDENADAS DA LUZ
		while(com.at(i) != '=') i++;
		com.erase(com.begin(), com.begin()+i+1);

		i = 0;
		while( com.at(i) != ' ') i++;
		pX.append(com.begin(), com.begin() + i);

		com.erase(com.begin(), com.begin()+ i + 6);
		i = 0;
		while( com.at(i) != ' ') i++;
		pY.append(com.begin(), com.begin() + i);

		com.erase(com.begin(), com.begin()+ i + 6);
		i = 0;
		while( com.at(i) != ' ') i++;
		pZ.append(com.begin(), com.begin() + i);
		

		luz -> tipo = LUZDIREC;
		luz -> posX = stof(pX);
		luz -> posY = stof(pY);
		luz -> posZ = stof(pZ);
		luz -> angulo =		-1;
		luz -> atenuacao =	-1;
		luz -> dirX =		-1;
		luz -> dirY =		-1;
		luz -> dirZ =		-1;
		luz -> cor =		-1;
	}

	if(tipoLuz == LUZSPOT)
	{
		// LIXO
		while(com.at(i) != '"') i++;
		com.erase(com.begin(), com.begin()+i);

		// TIPO DA LUZ
		i = 1;
		while(com.at(i) != '"') i++;
		sTipo.append(com.begin() +1, com.begin() + i);

		// POSIÇÃO DA LUZ
		while(com.at(i) != '=') i++;
		com.erase(com.begin(), com.begin() + i + 1);

		// pX
		i = 0;
		while( com.at(i) != ' ') i++;
		pX.append(com.begin(), com.begin() + i);
		// pY
		com.erase(com.begin(), com.begin() + i + 6);
		i = 0;
		while( com.at(i) != ' ') i++;
		pY.append(com.begin(), com.begin() + i);
		// pZ
		com.erase(com.begin(), com.begin() + i + 6);
		i = 0;
		while( com.at(i) != ' ') i++;
		pZ.append(com.begin(), com.begin() + i + 6);
		
		// ANGULO
		com.erase(com.begin(), com.begin() + i + 8);
		i = 0;
		while( com.at(i) != ' ') i++;
		angulo.append(com.begin(), com.begin() + i + 6);
		
		// ATENUACAO
		com.erase(com.begin(), com.begin() + i + 11);
		i = 0;
		while( com.at(i) != ' ') i++;
		atenuacao.append(com.begin(), com.begin() + i + 6);
		
		// dX
		com.erase(com.begin(), com.begin() + i + 6);
		i = 0;
		while( com.at(i) != ' ') i++;
		dX.append(com.begin(), com.begin() + i + 6);
		// dY
		com.erase(com.begin(), com.begin() + i + 6);
		i = 0;
		while( com.at(i) != ' ') i++;
		dY.append(com.begin(), com.begin() + i + 6);
		// dZ
		com.erase(com.begin(), com.begin() + i + 6);
		i = 0;
		while( com.at(i) != ' ') i++;
		dZ.append(com.begin(), com.begin() + i + 6);


		luz -> tipo = 		LUZDIREC;
		luz -> posX = 		stof(pX);
		luz -> posY = 		stof(pY);
		luz -> posZ = 		stof(pZ);
		luz -> angulo =		stof(angulo);
		luz -> atenuacao =	stof(atenuacao);
		luz -> dirX =		stof(dX);
		luz -> dirY =		stof(dY);
		luz -> dirZ =		stof(dZ);
		luz -> cor  =		-1;

		/*
		cout << "tipo" 		<< "\t#" << luz -> tipo  		<< "#"	<< endl;
		cout << "posX" 		<< "\t#" << luz -> posX  		<< "#"	<< endl;
		cout << "posY" 		<< "\t#" << luz -> posY  		<< "#"	<< endl;
		cout << "posZ" 		<< "\t#" << luz -> posZ  		<< "#"	<< endl;
		cout << "angulo" 	<< "\t#" << luz -> angulo 		<< "#"	<< endl;
		cout << "atenuacao" << "\t#" << luz -> atenuacao 	<< "#"	<< endl;
		cout << "dirX" 		<< "\t#" << luz -> dirX 		<< "#"	<< endl;
		cout << "dirY" 		<< "\t#" << luz -> dirY 		<< "#"	<< endl;
		cout << "dirZ" 		<< "\t#" << luz -> dirZ 		<< "#"	<< endl;
		cout << "cor" 		<< "\t#" << luz -> cor 			<< "#"	<< endl;
		*/
	}
}


void listaNCoords(int nModelos, int* nCoordModelos)
{
	for (int i = 0; i < nModelos; i++)
		cout << "Número de coordenadas do Modelo " << i  <<": " << nCoordModelos[i] << endl;
}
