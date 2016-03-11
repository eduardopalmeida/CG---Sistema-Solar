#ifndef _motor_
#define _motor_

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>


void carregaModelo(char* file);

void carregaTextura(char* file);

void carregaPatch(char* file);

void botaVBOs();

void addInst(Rotacao* auxR, int tipo);

void listaInst();

void listaPontos(vector<Ponto> pts);

#endif