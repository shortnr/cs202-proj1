all : game

game : main.o statics.o card.o datastructure.o game.o 
	g++ -o game -pipe -std=c++17 main.o statics.o game.o card.o datastructure.o -lncursesw -ltinfo

statics.o : statics.cpp statics.h
	g++ -c -g -std=c++17 statics.cpp

card.o : card.cpp card.h player.h
	g++ -c -g -std=c++17 card.cpp

datastructure.o : datastructure.cpp datastructure.h effect.h
	g++ -c -g -std=c++17 datastructure.cpp

game.o : game.cpp game.h player.h
	g++ -c -g -std=c++17 game.cpp

main.o : main.cpp
	g++ -c -g -std=c++17 main.cpp

clean :
	rm *.o
