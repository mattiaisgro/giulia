.default_target: all

CXXFLAGS = -std=c++11 -O3 -lm -I./include/

all:
	@echo Compiling Giulia ...
	@g++ src/*.cpp ${CXXFLAGS} -o giulia
	./giulia giulia.bmp 1024 1024
