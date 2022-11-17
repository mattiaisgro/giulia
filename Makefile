.default_target: all

CXXFLAGS = -std=c++11 -O3 -lm -I./include/ -fopenmp -DGIULIA_USE_OPENMP

all:
	@echo Compiling Giulia ...
	@g++ src/*.cpp ${CXXFLAGS} -o giulia
	./giulia giulia.bmp 2048 2048 1
