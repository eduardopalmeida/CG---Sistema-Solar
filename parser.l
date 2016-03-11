%{
	#include "structs.h"
%}


%%

:						return PONTINHOS;
"\<cena\>" 				return CENA1;
"\<\/cena\>" 			return CENA2;
"\<grupo\>" 			return GRUPO1;
"\<\/grupo\>" 			return GRUPO2;
"\<modelos\>"			return MODELOS1;
"\<\/modelos\>"			return MODELOS2;

"\<luzes\>"				return LUZES1;
"\<\/luzes\>"			return LUZES2;

"\<luz tipo="\"SPOT+\"[ ]"posX="[-]*[0-9]+[.]*[0-9]*[ ]"posY="[-]*[0-9]+[.]*[0-9]*[ ]"posZ="[-]*[0-9]+[.]*[0-9]*[ ]"angulo="[-]*[0-9]+[.]*[0-9]*[ ]"atenuacao="[-]*[0-9]+[.]*[0-9]*[ ]"dirX="[-]*[0-9]+[.]*[0-9]*[ ]"dirY="[-]*[0-9]+[.]*[0-9]*[ ]"dirZ="[-]*[0-9]+[.]*[0-9]*[ ]"\/\>" return LUZSPOT;
"\<luz tipo="\"POINT+\"[ ]"posX="[-]*[0-9]+[.]*[0-9]*[ ]"posY="[-]*[0-9]+[.]*[0-9]*[ ]"posZ="[-]*[0-9]+[.]*[0-9]*[ ]"\/\>" return LUZPOINT;
"\<luz tipo="\"DIRECT+\"[ ]"posX="[-]*[0-9]+[.]*[0-9]*[ ]"posY="[-]*[0-9]+[.]*[0-9]*[ ]"posZ="[-]*[0-9]+[.]*[0-9]*[ ]"\/\>" return LUZDIREC;


"\<translação"			return TRANSLACAO;
" \/\>"					return FIMTRANS;


"\<translação tempo="[0-9]+[.]*[0-9]*[ ]\>	return TRANSLTEMPI;
"\<\/translação\>"							return TRANSLTEMPF;


"\<ponto X="[-]*[0-9]+[.]*[0-9]*[ ]"Y="[-]*[0-9]+[.]*[0-9]*[ ]"Z="[-]*[0-9]+[.]*[0-9]*[ ]"\/\>" return PONTO;

"\<modelo ficheiro="\"[_a-zA-Z0-9]+[.3d]+\"[ ]\/\>			return MODELOF;
"\<modelo ficheiro="\"[_a-zA-Z0-9]+[.3d]+\"[ ]"textura="\"[_a-zA-Z0-9]+[.jpg]+\"[ ]\/\>			return MODELOT;
"\<modelo ficheiro="\"[_a-zA-Z0-9]+[.3d]+\"[ ]"diffR="[0-9]+[.]*[0-9]*[ ]"diffG="[0-9]+[.]*[0-9]*[ ]"diffB="[0-9]+[.]*[0-9]*[ ]\/\>	return MODELOC;
"\<modelo ficheiro="\"[_a-zA-Z0-9]+[.patch]+\"[ ]\/\>		return MODELOP;

"\<rotação angulo="[0-9]+[.]*[0-9]*[ ]"eixoX="[-]*[0-9]+[.]*[0-9]*[ ]"eixoY="[-]*[0-9]+[.]*[0-9]*[ ]"eixoZ="[-]*[0-9]+[.]*[0-9]*[ ]\/\> return ROTACANG;
"\<rotação tempo="[0-9]+[.]*[0-9]*[ ]"eixoX="[-]*[0-9]+[.]*[0-9]*[ ]"eixoY="[-]*[0-9]+[.]*[0-9]*[ ]"eixoZ="[-]*[0-9]+[.]*[0-9]*[ ]\/\> return ROTACTEMP;

"\<escala X="[0-9]+[ ]"Y="[0-9]+[ ]"Z="[0-9]+[ ]\/\>	return ESCALA;


"X="[0-9]*				return X;
"Y="[0-9]*				return Y;
"Z="[0-9]*				return Z;

[a-zA-Z][_a-zA-Z0-9]	return STRING;
[0-9]*					return INTEIRO;
[ \t\n]+

.						printf("Caracter não reconhecido: %s na linha %d\n", yytext, yylineno);

%%

int yywrap()
{
	return 1;
}