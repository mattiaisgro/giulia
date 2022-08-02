.default_target: all

CXXFLAGS = -std=c++11 -O3 -lm -I./include/

all:
	@echo Compiling Giulia ...
	@g++ src/*.cpp ${CXXFLAGS} -o giulia
	./giulia giulia.bmp 3072 2048
