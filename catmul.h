#ifndef _catmul_
#define _catmul_

#include <GL/glut.h>
#include <math.h>

Ponto Calculate(float u, float v, vector<int> indices);

Ponto CalculateU(float t,int row);

Ponto CalculateV(float t, Ponto* pnts);

void getCatmullRomPoint(float t, int *indices, float *res);

void getCatmullRomPointd(float t, int *indices, float *res);

void getGlobalCatmullRomPoint(int x, float gt, float *res, int pointNum);

void renderCatmullRomCurve(float *res, int pointNum);

#endif