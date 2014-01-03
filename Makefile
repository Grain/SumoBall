all: game
	

game: main.o player.o game.o skill.o
	g++ main.o player.o game.o skill.o -o game -lsfml-graphics -lsfml-window -lsfml-system

main.o:
	g++ -c main.cpp 

player.o:
	g++ -c player.cpp

game.o:
	g++ -c game.cpp

skill.o:
	g++ -c skill.cpp