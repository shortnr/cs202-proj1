all : game

game : main.o statics.o game.o card.o player.o datastructure.o
	g++ -o game -std=c++17 main.o statics.o game.o card.o player.o datastructure.o -lncursesw -ltinfo

ds : main.o card.o datastructure.o
	g++ -o game -std=c++17 main.o card.o datastructure.o -lncursesw -ltinfo

main.o : main.cpp
	g++ -c -g -std=c++17 main.cpp

statics.o : statics.cpp statics.h
	g++ -c -g -std=c++17 statics.cpp

game.o : game.cpp game.h
	g++ -c -g -std=c++17 game.cpp

player.o : player.cpp player.h
	g++ -c -g -std=c++17 player.cpp

card.o : card.cpp card.h
	g++ -c -g -std=c++17 card.cpp

datastructure.o : datastructure.cpp datastructure.h
	g++ -c -g -std=c++17 datastructure.cpp

clean :
	rm *.o
