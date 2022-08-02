#pragma once

#include "common.h"
#include "image.h"
#include <array>


namespace giulia {

	using fractal_map = std::array<real_t, 2>(*)(real_t, real_t);

	// Draw a Julia fractal with parameter (c_x, c_y)
	pixel draw_julia(real_t x, real_t y, real_t c_x, real_t c_y, unsigned int max_iter = 1000);

	// Draw the Mandelbrot fractal
	pixel draw_mandelbrot(real_t x, real_t y, unsigned int max_iter = 1000);

	// Draw the Mandelbar fractal
	pixel draw_mandelbar(real_t x, real_t y, unsigned int max_iter = 1000);

	// Draw a fractal map
	// pixel draw_fractal(real_t x, real_t y, real_t c_x, real_t c_y, fractal_map f, unsigned int max_iter = 1000);

}
