#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <math.h>

#include "structs.h"
  
#define PI 3.14159265359
 
using namespace std;

Ponto calculaNormal(Ponto v1, Ponto v2)
{
    // v1 . v2
    Ponto produto = { 
                        (v1.y * v2.z) - (v1.z * v2.y), 
                        (v1.z * v2.x) - (v1.x * v2.z),
                        (v1.x * v2.y) - (v1.y * v2.x)
                    };
    
    // || v1 . v2 || 
    float norma = sqrt( pow(produto.x, 2) + pow(produto.y, 2) + pow(produto.z, 2) );

    return  {  
                (produto.x) / norma, 
                (produto.y) / norma, 
                (produto.z) / norma 
            };
}
  
void plano(float larg, float comp, char* nomeF)
{
    ofstream file (nomeF);

    if (file.is_open())
    {
        vector <Ponto> pontos;
        vector <Ponto> normais;

        // Triangulo 1
        //                   X     Y    Z
        pontos.push_back( { 0.0,  0.0, 0.0 } ); // p1
        pontos.push_back( { larg, 0.0, 0.0 } ); // p2
        pontos.push_back( { larg, 0.0, comp} ); // p3

        // Triangulo 2
        //                   X     Y    Z
        pontos.push_back( { 0.0,  0.0, 0.0  } ); // p1
        pontos.push_back( { larg, 0.0, comp } ); // p2
        pontos.push_back( { 0.0,  0.0, comp } ); // p3


        // calcular todas as normais...
        for (int i = 0; i < pontos.size(); i += 3)
        {
            Ponto p1 = pontos.at( i + 0 );
            Ponto p2 = pontos.at( i + 1 );
            Ponto p3 = pontos.at( i + 2 );

            normais.push_back(
                                calculaNormal(  
                                                {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z}, 
                                                {p3.x - p1.x, p3.y - p1.y, p3.z - p1.z} 
                                             )
                             );
        }

        // EXPORT
        // Nº de COORDENADAS DOS PONTOS
        file << ( pontos.size() * 3);

        // PONTOS
        for (int i = 0; i < pontos.size(); i++) 
            file << endl << pontos.at(i).x  << "\t" << pontos.at(i).y  << "\t" << pontos.at(i).z;

        // Nº de COORDENADAS DAS NORMAIS
        file << endl << ( normais.size() * 3);
        
        // NORMAIS
        for (int i = 0; i < normais.size(); i++)
            file << endl << normais.at(i).x  << "\t" << normais.at(i).y  << "\t"<< normais.at(i).z;

        file.close();
    }
    else cout << "Unable to open file";
}
  
void paralelo(float larg, float comp, float altura, char* nomeF)
{
    ofstream file (nomeF);
      
    if (file.is_open())
    {
        vector <Ponto> pontos;
        vector <Ponto> normais;

        // PARTE DE CIMA
        // Triangulo 1
        //         X                Y                  Z
        pontos.push_back( { larg/2,     altura/2,  -comp/2  } );//H
        pontos.push_back( { -larg/2,    altura/2,  -comp/2  } );//E
        pontos.push_back( { larg/2,     altura/2,   comp/2  } );//G
        // Triangulo 2
        //         X                Y                  Z
        pontos.push_back( { larg/2,     altura/2,   comp/2  } );//G
        pontos.push_back( { -larg/2,    altura/2,  -comp/2  } );//E
        pontos.push_back( { -larg/2,    altura/2,   comp/2  } );//F
  
        // PARTE DE BAIXO
        // Triangulo 1
        //         X                Y                  Z
        pontos.push_back( { -larg/2,    -altura/2,  -comp/2 } );//A
        pontos.push_back( { larg/2,     -altura/2,  -comp/2 } );//D
        pontos.push_back( { larg/2,     -altura/2,   comp/2 } );//C
        // Triangulo 2
        //         X                Y                  Z
        pontos.push_back( { -larg/2,    -altura/2,  -comp/2 } );//A
        pontos.push_back( { larg/2,     -altura/2,   comp/2 } );//C
        pontos.push_back( { -larg/2,    -altura/2,   comp/2 } );//B
  
        // FRENTE
        // Triangulo 1
        //         X                Y                  Z
        pontos.push_back( { larg/2,     altura/2,   comp/2  } );//G
        pontos.push_back( { -larg/2,    altura/2,   comp/2  } );//F
        pontos.push_back( { larg/2,     -altura/2,  comp/2  } );//C
        // Triangulo 2
        //         X                Y                  Z
        pontos.push_back( { larg/2,     -altura/2,  comp/2  } );//C
        pontos.push_back( { -larg/2,    altura/2,   comp/2  } );//F
        pontos.push_back( { -larg/2,    -altura/2,  comp/2  } );//B
 
        // Tras
        // Triangulo 1 
        //         X                Y                  Z
        pontos.push_back( { larg/2,     -altura/2,  -comp/2 } );//D
        pontos.push_back( { -larg/2,    -altura/2,  -comp/2 } );//A
        pontos.push_back( { -larg/2,    altura/2,   -comp/2 } );//E
        // Triangulo 2 
        //         X                Y                  Z
        pontos.push_back( { larg/2,     -altura/2,  -comp/2 } );//D
        pontos.push_back( { -larg/2,    altura/2,   -comp/2 } );//E
        pontos.push_back( { larg/2,     altura/2,   -comp/2 } );//H
 
        // lateral direita
        // Triangulo 1 
        //         X                Y                  Z
        pontos.push_back( { larg/2,      altura/2,  -comp/2 } );//H
        pontos.push_back( { larg/2,      altura/2,   comp/2 } );//G
        pontos.push_back( { larg/2,     -altura/2,   comp/2 } );//C
        // Triangulo 2
        //         X                Y                  Z
        pontos.push_back( { larg/2,      altura/2,   -comp/2 } );//H
        pontos.push_back( { larg/2,     -altura/2,    comp/2 } );//C
        pontos.push_back( { larg/2,     -altura/2,   -comp/2 } );//D
  
  
        // lateral esquerda
        // Triangulo 1
        //         X                Y                  Z
        pontos.push_back( { -larg/2,     altura/2,   comp/2  } );//F
        pontos.push_back( { -larg/2,     altura/2,  -comp/2  } );//E
        pontos.push_back( { -larg/2,    -altura/2,  -comp/2  } );//A
        // Triangulo 2
        //         X                Y                  Z
        pontos.push_back( { -larg/2,    -altura/2,   comp/2 } );//B
        pontos.push_back( { -larg/2,     altura/2,   comp/2 } );//F
        pontos.push_back( { -larg/2,    -altura/2,  -comp/2 } );//A

        for (int i = 0; i < pontos.size(); i += 3)
        {
            Ponto p1 = pontos.at( i + 0 );
            Ponto p2 = pontos.at( i + 1 );
            Ponto p3 = pontos.at( i + 2 );

            normais.push_back(
                                calculaNormal(  
                                                { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z }, 
                                                { p3.x - p1.x, p3.y - p1.y, p3.z - p1.z } 
                                             )
                             );
        }
  
        // EXPORT
        // Nº de COORDENADAS DOS PONTOS
        file << ( pontos.size() * 3);

        // PONTOS
        for (int i = 0; i < pontos.size(); i++) 
            file << endl << pontos.at(i).x  << "\t" << pontos.at(i).y  << "\t" << pontos.at(i).z;

        // Nº de COORDENADAS DAS NORMAIS
        file << endl << ( normais.size() * 3);
        
        // NORMAIS
        for (int i = 0; i < normais.size(); i++)
            file << endl << normais.at(i).x  << "\t" << normais.at(i).y  << "\t"<< normais.at(i).z;

        file.close();
    }
    else cout << "Unable to open file";
}
  
void esfera(float radio, int fatias, int camadas, char* nomeF)
{
    ofstream file (nomeF);

    float texFactor_fatias=1.0f/fatias;
    float texFactor_camadas=1.0f/camadas;
		


    if (file.is_open())
    {
        float x, y, z;
        int nCoordEsf = 0;
        float i, j, poo = PI / camadas, pee = (2 * PI) / fatias;

        vector <Ponto> pontos;
        vector <Ponto> normais;
        vector <Ponto> texturas;

        // CALCULA VÉRTICES
        for(i = PI/2; i > -(PI/2+poo); i-= poo) 
        {
            for(j = 0.0; j < 2*PI; j+= pee) 
                nCoordEsf = nCoordEsf + 18;
        }
         
        for(i = PI/2; i > -(PI/2+poo); i-= poo) 
        {
            for(j = 0.0; j < 2*PI; j+= pee) 
            {
                pontos.push_back( { (float) (radio*cos(i+poo)*sin(j+pee)),  (float)  (radio*sin(i+poo)), (float)  (radio*cos(i+poo)*cos(j+pee)) } );
                pontos.push_back( { (float) (radio*cos(i+poo)*sin(j)),      (float)  (radio*sin(i+poo)), (float)  (radio*cos(i+poo)*cos(j))     } );
                pontos.push_back( { (float) (radio*cos(i)*sin(j)),          (float)  (radio*sin(i)),     (float)  (radio*cos(i)*cos(j))         } );
                pontos.push_back( { (float) (radio*cos(i)*sin(j+pee)),      (float)  (radio*sin(i)),     (float)  (radio*cos(i)*cos(j+pee))     } );
                pontos.push_back( { (float) (radio*cos(i+poo)*sin(j+pee)),  (float)  (radio*sin(i+poo)), (float)  (radio*cos(i+poo)*cos(j+pee)) } );
                pontos.push_back( { (float) (radio*cos(i)*sin(j)),          (float)  (radio*sin(i)),     (float)  (radio*cos(i)*cos(j))         } );
          			
								texturas.push_back({(float) (1*cos(texFactor_camadas)*sin(texFactor_fatias)),          (float)  (1*sin(texFactor_fatias)),  -1} );
								texFactor_fatias+=texFactor_fatias;	texFactor_camadas+=texFactor_camadas;
						}
        }

        // CALCULA VÉRTICES
        for (int i = 0; i < pontos.size(); i += 3)
        {
            Ponto p1 = pontos.at( i + 0 );
            Ponto p2 = pontos.at( i + 1 );
            Ponto p3 = pontos.at( i + 2 );

            normais.push_back(
                                calculaNormal(  
                                                { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z }, 
                                                { p3.x - p1.x, p3.y - p1.y, p3.z - p1.z } 
                                             )
                             );
        }

        // CÁLCULA PONTOS DE TEXTURA
/*
        for (int i = 0; i < fatias; i++)    texturas.push_back({ i*texFactor_fatias, 1, -1} );

        for (int i = 0; i < fatias; i++)    texturas.push_back( { i*texFactor_fatias, 1.0f - texFactor_fatias, -1} );

        for (int j = 1; j < camadas -1; j++)
        {
            for (int i = 0; i < fatias; i++)
                texturas.push_back({ i*texFactor_fatias, (camadas-(j+1))*texFactor_camadas, -1});
        }

        for (int i = 0; i < fatias; i++)    texturas.push_back( { i*texFactor_fatias, 0, -1} );
*/
            
        // EXPORT
        // Nº de COORDENADAS DOS PONTOS
        file << ( pontos.size() * 3);

        // PONTOS
        for (int i = 0; i < pontos.size(); i++) 
            file << endl << pontos.at(i).x  << "\t" << pontos.at(i).y  << "\t" << pontos.at(i).z;

        // Nº de COORDENADAS DAS NORMAIS
        file << endl << ( normais.size() * 3);
        
        // NORMAIS
        for (int i = 0; i < normais.size(); i++)
            file << endl << normais.at(i).x  << "\t" << normais.at(i).y  << "\t"<< normais.at(i).z;

        // Nº de COORDENADAS DAS TEXTURAS
        file << endl << (texturas.size() * 2);

        for (int i = 0; i < texturas.size(); i++)
            file << endl << texturas.at(i).x  << "\t" << texturas.at(i).y;

        file.close();
    }
    else cout << "Unable to open file";
}


void cone(float raio, int fatias, float altura, char* nomeF)
{
    int nCoord;
    float cx, cy, cz;

    cx = cy = cz = 0.0;

    vector<Ponto> pontinhos, pontos, normais;
 
    float angA = (2 * PI) / fatias;
 
    ofstream file (nomeF);

    if (file.is_open())
    {
        for(int i = 0; i < fatias;  i++)
            pontinhos.push_back(   
                                {   (float) ( raio * cos(i * angA)), 
                                    (float) 0.0, 
                                    (float) ( raio * sin(i * angA))
                                }
                            );
 
        // BASE        
        pontos.push_back( { cx,                    cy,                    cz                 } );
        pontos.push_back( { pontinhos.at(0).x,     pontinhos.at(0).y,     pontinhos.at(0).z  } );
        pontos.push_back( { pontinhos.back().x,    pontinhos.back().y,    pontinhos.back().z } );

        for (int i = 0; i < fatias - 1; i++)
        {
            pontos.push_back( { cx,                     cy,                    cz                  } );
            pontos.push_back( { pontinhos.at(i+1).x,    pontinhos.at(i+1).y,   pontinhos.at(i+1).z } );
            pontos.push_back( { pontinhos.at(i).x,      pontinhos.at(i).y,     pontinhos.at(i).z   } );
        }
 
        // CORPO
        pontos.push_back( { cx,                     altura,                cz                 } );
        pontos.push_back( { pontinhos.back().x,     pontinhos.back().y,    pontinhos.back().z } );
        pontos.push_back( { pontinhos.at(0).x,      pontinhos.at(0).y,     pontinhos.at(0).z  } );
     
 
        for (int i = 0; i < fatias - 1; i++)
        {
            pontos.push_back( { cx,                    altura,                cz                    } );
            pontos.push_back( { pontinhos.at(i).x,     pontinhos.at(i).y,     pontinhos.at(i).z     } );
            pontos.push_back( { pontinhos.at(i+1).x,   pontinhos.at(i+1).y,   pontinhos.at(i+1).z   } );
        }

        // NORMAIS
        for (int i = 0; i < pontos.size(); i += 3)
        {
            Ponto p1 = pontos.at( i + 0 );
            Ponto p2 = pontos.at( i + 1 );
            Ponto p3 = pontos.at( i + 2 );

            normais.push_back(
                                calculaNormal(  
                                                { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z }, 
                                                { p3.x - p1.x, p3.y - p1.y, p3.z - p1.z } 
                                             )
                             );
        }

        // EXPORT
        // Nº de COORDENADAS DOS PONTOS
        file << ( pontos.size() * 3);

        // PONTOS
        for (int i = 0; i < pontos.size(); i++) 
            file << endl << pontos.at(i).x  << "\t" << pontos.at(i).y  << "\t" << pontos.at(i).z;

        // Nº de COORDENADAS DAS NORMAIS
        file << endl << ( normais.size() * 3);
        
        // NORMAIS
        
        for (int i = 0; i < normais.size(); i++)
            file << endl << normais.at(i).x  << "\t" << normais.at(i).y  << "\t"<< normais.at(i).z;
        
        file.close();
    }
    else cout << "Unable to open file";
}


void erro() {   printf("ERRO!"); }
  
int main(int argc, char **argv)
{
    int opcao = -1;
  
    if(argc > 1)
    {
        // PARSING DOS COMANDOS
        if(strcmp(argv[1],  "plano"     ) == 0) opcao = 1; else
        {
        if(strcmp(argv[1],  "esfera"    ) == 0) opcao = 2; else
        {
        if(strcmp(argv[1],  "paralelo"  ) == 0) opcao = 3; else
        {
        if(strcmp(argv[1],  "cone"      ) == 0) opcao = 4;
        }}}
  
        switch(opcao)
        {
            case -1 : erro(); break;
  
            case  1 : plano(   
                                strtof(argv[2], NULL),
                                strtof(argv[3], NULL),
                                argv[4]
                            ); break;
  
            case  2 : esfera(  
                                strtof(argv[2], NULL),
                                atoi(argv[3]),
                                atoi(argv[4]),
                                argv[5]
                            ) ; break;
  
            case  3 : paralelo(
                                strtof(argv[2], NULL),
                                strtof(argv[3], NULL),
                                strtof(argv[4], NULL),
                                argv[5]
                              ); break;
  
            case  4 : cone(    
                                strtof(argv[2], NULL),
                                atoi(argv[3]),
                                strtof(argv[4], NULL),
                                argv[5]
                            ) ; break;
  
            default : erro();break;
        }
    }
    return 1;
 
}
