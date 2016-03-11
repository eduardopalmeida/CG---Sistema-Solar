#ifndef _parser_
#define _parser_

using namespace std;

void parseEscala(string com, Rotacao* rot);

void parseRotacao(string com, Rotacao* rot, int tipo);

void parsePonto(string com, Ponto* auxP);

int parseTempo(string com);

void parseCor(string com, Rotacao* rot);

void parseLuz(string com, Luz* luz, int tipoLuz);

void listaNCoords();


#endif