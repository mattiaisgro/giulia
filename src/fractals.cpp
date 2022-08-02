#include "fractals.h"

#define THEORETICA_LONG_DOUBLE_PREC
#include "theoretica/theoretica.h"

using namespace theoretica;
using namespace giulia;

#include <iostream>


pixel giulia::draw_julia(real_t x, real_t y, real_t c_x, real_t c_y, unsigned int max_iter) {

	complex z = complex(x, y);
	complex d = z;
	complex c = complex(c_x, c_y);

	// Escape radius
	real R = 2;

	// Number of iterations
	unsigned int i = 0;

	// Orbit trap minimum distances
	real dist1 = R;
	real dist2 = R;
	real dist3 = R;
	real dist4 = R;

	// Orbit trap positions
	complex trap1 = complex(0, 0);
	complex trap2 = complex(0.1, 0.1);
	complex trap3 = complex(0.2, 0.2);
	complex trap4 = complex(0.3, 0.3);

	while(z.square_modulus() < (R * R) && i <= max_iter) {

		// z_i+1 = z_i ^ 2 + c
		z = square(z) + c;

		dist1 = min(dist1, (z - trap1).modulus());
		dist2 = min(dist2, (z - trap2).modulus());
		dist3 = min(dist3, (z - trap3).modulus());
		dist4 = min(dist4, (z - trap4).modulus());

		i++;
	}

	// Normalized iteration factor
	real iter_factor = i / (real) max_iter;

	// Smooth intensity factor
	real intensity_factor = (i - ln(ln(z.modulus()) / ln(R)) / LN2) / (real) max_iter;

	// Base and trap colors
	vec3 base_color = {0x9b, 0x5d, 0xe5};
	vec3 trap_color1 = {0xf1, 0x5b, 0xb5};
	vec3 trap_color2 = {0xfe, 0x00, 0x40};
	vec3 trap_color3 = {0x0f, 0xbb, 0xb9};
	vec3 trap_color4 = {0xff, 0x00, 0x6e};

	// Pixel color
	vec3 color = lerp(base_color, trap_color1, dist1);
	color = lerp(color, trap_color2, dist2);
	color = lerp(color, trap_color4, dist4);

	color *= intensity_factor;

	real x_factor = exp(-square(d.Re()) / 4);
	real y_factor = exp(-square(d.Im()) / 1);

	color *= x_factor * y_factor;

	real brightness = 80;

	return pixel(
		clamp(color[0] * brightness, 0, 255),
		clamp(color[1] * brightness, 0, 255),
		clamp(color[2] * brightness, 0, 255));
}


pixel giulia::draw_mandelbrot(real_t x, real_t y, unsigned int max_iter) {

	complex z = complex(x, y);
	complex c = complex(x, y);

	// Escape radius
	real R = 2;

	// Number of iterations
	unsigned int i = 0;

	while(z.square_modulus() < (R * R) && i <= max_iter) {

		z = square(z) + c;
		i++;
	}

	// Normalized iteration factor
	real iter_factor = i / (real) max_iter;

	// Smooth intensity factor
	real intensity_factor = (i - ln(ln(z.modulus()) / ln(R)) / LN2) / (real) max_iter;

	// Pixel color
	vec3 color = {0x9b, 0x5d, 0xe5};
	real brightness = 0.04 * max_iter;

	return pixel(
		clamp(color[0] * brightness * intensity_factor, 0, 255),
		clamp(color[1] * brightness * intensity_factor, 0, 255),
		clamp(color[2] * brightness * intensity_factor, 0, 255));
}


pixel giulia::draw_mandelbar(real_t x, real_t y, unsigned int max_iter) {

	complex z = complex(0, 0);
	complex c = complex(x, y);

	// Escape radius
	real R = 2;

	// Number of iterations
	unsigned int i = 0;

	while(z.square_modulus() < (R * R) && i <= max_iter) {

		z = square(z.conjugate()) + c;
		i++;
	}

	// Normalized iteration factor
	real iter_factor = i / (real) max_iter;

	// Smooth intensity factor
	real intensity_factor = (i - ln(ln(z.modulus()) / ln(R)) / LN2) / (real) max_iter;

	// Pixel color
	vec3 color = {0x9b, 0x5d, 0xe5};
	real brightness = 0.04 * max_iter;

	return pixel(
		clamp(color[0] * brightness * intensity_factor, 0, 255),
		clamp(color[1] * brightness * intensity_factor, 0, 255),
		clamp(color[2] * brightness * intensity_factor, 0, 255));

}
