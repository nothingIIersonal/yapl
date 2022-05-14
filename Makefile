all:
	g++ -Wall -O2 -m64 main.cpp -o yapl -std=c++17

debug: 
	g++ -Wall -O2 -O -m64 main.cpp -o yapl -Werror -g -std=c++17
