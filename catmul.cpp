#include "structs.h"
#include "catmul.h"

extern vector< vector<Ponto> > translacoes;
extern int translActual;
extern vector <Patch>	patches;


Ponto Points[4][4];

void getCatmullRomPoint(float t, int *indices, float *res)
{
	// catmull-rom matrix
	float m[4][4] = { 
						{ -0.5f, 1.5f, -1.5f, 0.5f },
						{ 1.0f, -2.5f, 2.0f, -0.5f },
						{ -0.5f, 0.0f, 0.5f, 0.0f },
						{ 0.0f, 1.0f, 0.0f, 0.0f } 
					};
	res[0] = 0.0; 
	res[1] = 0.0; 
	res[2] = 0.0;

	// Compute point res = T * M * P
	// where Pi = p[indices[i]]
	double T[4] = { pow(t, 3), pow(t, 2), t, 1 };

	float sol1[4] = { 0, 0, 0, 0 };
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			sol1[i] += T[j] * m[j][i];
	}

	for (int j = 0; j < 4; j++)
	{
	/*
		cout << "X= " << (translacoes.at(translActual)).at(indices[j]).x << endl;
		cout << "Y= " << (translacoes.at(translActual)).at(indices[j]).y << endl;
		cout << "Z= " << (translacoes.at(translActual)).at(indices[j]).z << endl;
		cout << endl;
	*/

		res[0] += sol1[j] * (translacoes.at(translActual)).at(indices[j]).x;
		res[1] += sol1[j] * (translacoes.at(translActual)).at(indices[j]).y;
		res[2] += sol1[j] * (translacoes.at(translActual)).at(indices[j]).z;
	}
}

void getCatmullRomPointd(float t, int *indices, float *res) 
{
	// catmull-rom matrix
	float m[4][4] = { 
					{ -0.5f, 1.5f, -1.5f, 0.5f },
					{ 1.0f, -2.5f, 2.0f, -0.5f },
					{ -0.5f, 0.0f, 0.5f, 0.0f  },
					{ 0.0f,	 1.0f, 0.0f, 0.0f  } 
					};

	res[0] = 0.0; 
	res[1] = 0.0; 
	res[2] = 0.0;
	
	// Compute point res = T * M * P
	// where Pi = p[indices[i]]
	
	double T[4] = { 3*pow(t, 2), 2*t, 1, 0 };
	float sol1[4] = { 0, 0, 0, 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			sol1[i] += T[j] * m[j][i];
	}

	for (int j = 0; j < 4; j++)
	{
	/*
		cout << (translacoes.at(translActual)).at(indices[j]).x;
		cout << (translacoes.at(translActual)).at(indices[j]).y;
		cout << (translacoes.at(translActual)).at(indices[j]).z;
	*/
		res[0] += sol1[j] * (translacoes.at(translActual)).at(indices[j]).x;
		res[1] += sol1[j] * (translacoes.at(translActual)).at(indices[j]).y;
		res[2] += sol1[j] * (translacoes.at(translActual)).at(indices[j]).z;
	}
}

void getGlobalCatmullRomPoint(int x, float gt, float *res, int pointNum)
{

	float t = gt * pointNum;	// this is the real global t
	int index = floor(t);  		// which segment
	t = t - index; 				// where within  the segment

	// indices store the points
	int indices[4];

	indices[0] = (index + pointNum - 1)		% pointNum;	
	indices[1] = (indices[0] + 1) 			% pointNum;
	indices[2] = (indices[1] + 1) 			% pointNum; 
	indices[3] = (indices[2] + 1) 			% pointNum;
	
	switch (x)
	{
		case 1 	:	getCatmullRomPoint(t, indices, res);	break;
		case 0 	:	getCatmullRomPointd(t, indices, res);
	}
	
}

void renderCatmullRomCurve(float *res, int pointNum) 
{
	// desenhar a curva usando segmentos de reta - GL_LINE_LOOP

	glColor3f(1.0,1.0,1.0);

	glBegin(GL_LINE_LOOP);
	float i = 0;
	while (i <= 1)
	{
		getGlobalCatmullRomPoint(1, i, res, pointNum);
		i += 0.001;
		glVertex3f(res[0], res[1], res[2]);
	}
	glEnd();
}


Ponto CalculateU(float t,int row) 
{
	// the final point
	Ponto p;

	// the t value inverted
	float it = 1.0f-t;

	// calculate blending functions
	float b0 = t*t*t;
	float b1 = 3*t*t*it;
	float b2 = 3*t*it*it;
	float b3 =  it*it*it;
	
	// sum the effects of the Points and their respective blending functions
	p.x =	b0 * Points[row][0].x +
			b1 * Points[row][1].x +
			b2 * Points[row][2].x +
			b3 * Points[row][3].x;

	p.y =	b0 * Points[row][0].y +
			b1 * Points[row][1].y +
			b2 * Points[row][2].y +
			b3 * Points[row][3].y;

	p.z =	b0 * Points[row][0].z +
			b1 * Points[row][1].z +
			b2 * Points[row][2].z +
			b3 * Points[row][3].z;

	return p;
}

Ponto CalculateV(float t, Ponto* pnts)
{
	Ponto p;

	// the t value inverted
	float it = 1.0f-t;

	// calculate blending functions
	float b0 = t*t*t;
	float b1 = 3*t*t*it;
	float b2 = 3*t*it*it;
	float b3 =  it*it*it;

	// sum the effects of the Points and their respective blending functions
	p.x =	b0*pnts[0].x + 
			b1*pnts[1].x + 
			b2*pnts[2].x + 
			b3*pnts[3].x ;

	p.y = 	b0*pnts[0].y + 
			b1*pnts[1].y + 
			b2*pnts[2].y + 
			b3*pnts[3].y ;

	p.z = 	b0*pnts[0].z + 
			b1*pnts[1].z + 
			b2*pnts[2].z + 
			b3*pnts[3].z ;

	return p;
}

Ponto Calculate(float u, float v, vector<int> indices) 
{
	Ponto temp[4];

	//cout << "YOOOOO1" << "PATCHACTUAL"  << patchActual << endl;
	/*
	for (int i = 0; i < 16; ++i)
	{
		cout << indices.at(i) << " ";
	}
	cout << endl;
	*/

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			Points[i][j] = ( patches.at(0) ).CPoints.at(indices.at( (4*i) + j ));
			//cout << (4*i)+j << endl;
	}

	//cout << "YOOOOO2" << endl;

	temp[0] = CalculateU(u,0);
	temp[1] = CalculateU(u,1);
	temp[2] = CalculateU(u,2);
	temp[3] = CalculateU(u,3);

	return CalculateV(v,temp);
}


