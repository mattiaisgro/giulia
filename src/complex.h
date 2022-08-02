#pragma once

#include "common.h"
#include "image.h"
#define _hypot(x, y) (sqrt(x*x + y*y))
#include <cmath>


namespace giulia {

	struct complex_t {
		
		real_t x;
		real_t y;

		complex_t() : x(0), y(0) {}
		complex_t(real_t x, real_t y) : x(x), y(y) {}

	};


	inline real_t square(real_t x) {
		return x * x;
	}


	// Complex functions

	inline real_t complex_arg(complex_t z) {
		return atan2(z.y, z.x);
	}

	inline real_t complex_abs(complex_t z) {
		return sqrt(square(z.x) + square(z.y));
	}

	inline complex_t complex_conjugate(complex_t z) {
		return complex_t(z.x, -z.y);
	}

	inline complex_t complex_sum(complex_t z, complex_t w) {
		return complex_t(z.x + w.x, z.y + w.y);
	}

	inline complex_t complex_diff(complex_t z, complex_t w) {
		return complex_t(z.x - w.x, z.y - w.y);
	}

	inline complex_t complex_mul(complex_t z, complex_t w) {
		return complex_t(
			z.x * w.x - z.y * w.y,
			z.x * w.y + z.y * w.x);
	}

	inline complex_t complex_div(complex_t z, complex_t w) {
		return complex_mul(z, complex_conjugate(w));
	}

	inline complex_t complex_pow(complex_t z, real_t p) {

		real_t rho = complex_abs(z);
		real_t theta = complex_arg(z);

		return complex_t(
			pow(rho, p) * cos(theta * p),
			pow(rho, p) * sin(theta * p));
	}


	inline pixel arg_color_map(complex_t z) {

		real_t argument = complex_arg(z);
		real_t modulus = complex_abs(z);

		real_t r = (1 - (argument / PI)) * modulus;
		real_t g = 0;
		real_t b = ((argument / PI) - 1) * modulus;

		return pixel(r * 255, g * 255, b * 255);
	}

}
