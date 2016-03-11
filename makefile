MFLAGS = -lm -lGL -lGLU -lglut -lGLEW -lIL
GFLAGS = -lm
 
buildall: parser gerador main

clean:	
		rm *.o
		rm main gerador

parser: parser.l
						flex parser.l

gerador: gerador.cpp
						g++ -std=c++11 gerador.cpp -o gerador $(GFLAGS)
 
main: main.o lex.yy.o parser.o motor.o catmul.o 
						g++ main.o motor.o parser.o catmul.o lex.yy.o -o main $(MFLAGS)
 
lex.yy.o: lex.yy.c
						g++ -c lex.yy.c

parser.o: parser.cpp
						g++ -c -std=c++11 parser.cpp

motor.o: motor.cpp
						g++ -c -std=c++11 motor.cpp

catmul.o: catmul.cpp
						g++ -c -std=c++11 catmul.cpp

main.o: main.cpp
						g++ -c -std=c++11 main.cpp
