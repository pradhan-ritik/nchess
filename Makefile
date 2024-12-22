default:
	g++ -o nchess src/*.cpp -Wall -Wextra

performance:
	g++ -o nchess src/*.cpp -O3