#include "fractals.h"

#define THEORETICA_LONG_DOUBLE_PREC
#include "theoretica/theoretica.h"

using namespace theoretica;
using namespace giulia;

#include <ctime>


pixel giulia::draw_julia(real_t x, real_t y, real_t c_x, real_t c_y, unsigned int max_iter) {

	complex z = complex(x, y);
	complex d = z;
	complex c = complex(c_x, c_y);

	// Escape radius
	real R = 2;

	// Number of iterations
	unsigned int i = 0;

	while(z.square_modulus() < (R * R) && i <= max_iter) {
		// z_i+1 = z_i ^ 2 + c
		z = square(z) + c;
		i++;
	}

	// Normalized iteration factor
	real iter_factor = i / (real) max_iter;

	// Smooth intensity factor
	real intensity_factor = (i - ln(0.5 * ln(z.square_modulus()) / ln(R)) / LN2) / (real) max_iter;

	real brightness = 0.005 * max_iter;

	// Resulting gray scale color
	real gray_scale = clamp(255 * intensity_factor * brightness, 0, 255);

	return pixel(gray_scale, gray_scale, gray_scale);
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
	real intensity_factor = (i - ln(0.5 * ln(z.square_modulus()) / ln(R)) / LN2) / (real) max_iter;
	real brightness = 0.04 * max_iter;

	unsigned char res = clamp(255 * brightness * intensity_factor, 0, 255);

	// Gray scale result
	return pixel(res, res, res);
}


pixel giulia::draw_mandelbar(real_t x, real_t y, unsigned int max_iter) {

	complex z = complex(x, y);
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
	real intensity_factor = (i - ln(0.5 * ln(z.square_modulus()) / ln(R)) / LN2) / (real) max_iter;
	real brightness = 0.04 * max_iter;

	unsigned char res = clamp(255 * brightness * intensity_factor, 0, 255);

	// Gray scale result
	return pixel(res, res, res);

}


pixel giulia::draw_fractal(real_t x, real_t y, fractal_map f, real_t R, unsigned int max_iter) {

	real_t z_a = x;
	real_t z_b = y;

	// Number of iterations
	unsigned int i = 0;

	while((z_a * z_a + z_b * z_b) < (R * R) && i <= max_iter) {
		auto z_i = f(z_a, z_b);
		z_a = z_i[0];
		z_b = z_i[1];
		i++;
	}

	// Normalized iteration factor
	real iter_factor = i / (real) max_iter;

	// Smooth intensity factor
	real intensity_factor = (i - ln(0.5 * ln(z_a * z_a + z_b * z_b) / ln(R)) / LN2) / (real) max_iter;
	real brightness = 0.04 * max_iter;

	unsigned char res = clamp(255 * brightness * intensity_factor, 0, 255);

	// Gray scale result
	return pixel(res, res, res);

}


void giulia::draw_sierpinski_triangle(
	image& img, real_t x, real_t y, real_t width, unsigned int iter, pixel c) {

	PRNG g = PRNG::wyrand(time(nullptr));

	vec2 A[3];
	A[0] = {x, y};
	A[1] = {x + width, y};
	A[2] = {x + width / 2.0, y + (width * SQRT2 / 2.0)};

	vec2 P = A[g() % 3];

	overwrite(img, A[0][0], A[0][1], c);
	overwrite(img, A[1][0], A[1][1], c);
	overwrite(img, A[2][0], A[2][1], c);

	for (size_t i = 0; i < iter; ++i) {
		P = (P + A[g() % 3]) / 2.0;
		overwrite(img, P[0], P[1], c);
	}
}


pixel giulia::draw_newton_fractal(real_t x, real_t y,
	const std::vector<std::array<real_t, 2>>& roots,
	const std::vector<pixel>& colors, unsigned int max_iter, real_t epsilon) {

	if(roots.size() != colors.size())
		return pixel(0, 0, 0);

	std::vector<complex> complex_roots;
	complex_roots.reserve(roots.size());

	for (size_t i = 0; i < roots.size(); ++i)
		complex_roots.emplace_back(roots[i][0], roots[i][1]);


	polynomial<complex> P = polynomial<complex>::from_roots(complex_roots);
	polynomial<complex> dP = deriv_polynomial(P);

	complex z = complex(x, y);
	pixel c = pixel(0, 0, 0);
	unsigned int iter = 0;
	real dist = inf();

	// Newton's method in the complex plane
	while(dist > epsilon && iter < max_iter) {
		z = z - (P(z) / dP(z));
		dist = P(z).modulus();
		iter++;
	}

	real pick_dist = inf();

	// Find the nearest root
	for (size_t i = 0; i < complex_roots.size(); ++i) {

		const real curr_dist = (z - complex_roots[i]).modulus();

		if(curr_dist < pick_dist) {
			pick_dist = curr_dist;
			c = colors[i];
		}
	}

	real intensity_factor = 1 - (iter / (real) max_iter);
	return c * intensity_factor;
}
