#pragma once

#include "common.h"
#include "image.h"
#include <array>
#include <functional>


namespace giulia {

	using fractal_map = std::function<std::array<real_t, 2>(real_t, real_t)>;


	// Draw a Julia fractal with parameter (c_x, c_y)
	pixel draw_julia(real_t x, real_t y, real_t c_x = -0.76, real_t c_y = 0.1482, unsigned int max_iter = 1000);


	// Draw the Mandelbrot fractal
	pixel draw_mandelbrot(real_t x, real_t y, unsigned int max_iter = 1000);


	// Draw the Mandelbar fractal
	pixel draw_mandelbar(real_t x, real_t y, unsigned int max_iter = 1000);


	// Draw a fractal map
	pixel draw_fractal(real_t x, real_t y, fractal_map f, real_t R = 2, unsigned int max_iter = 1000);


	// Draw a Sierpinski triangle (in post-processing)
	void draw_sierpinski_triangle(
		image& img, real_t x = 0, real_t y = 0,
		real_t width = 0, unsigned int iter = 1000000,
		pixel c = pixel(255, 255, 255));


	// Draw Newton's fractal
	pixel draw_newton_fractal(
		real_t x, real_t y,
		const std::vector<std::array<real_t, 2>>& roots,
		const std::vector<pixel>& colors,
		unsigned int max_iter = 30,
		real_t epsilon = 0.00000001);

}
