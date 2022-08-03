#include "image.h"

#define THEORETICA_LONG_DOUBLE_PREC
#include "theoretica/core/real_analysis.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace theoretica;
using namespace giulia;


int giulia::save_image(std::string filename, uint32_t width, uint32_t height, pixel* data) {
	
	int res = stbi_write_bmp(filename.c_str(), width, height, 3, (void*) data);
	return res ? 0 : -1;
}


int giulia::save_image(std::string filename, image img) {
	
	return giulia::save_image(filename, img.get_width(), img.get_height(), img.get_data());
}


real_t giulia::intensity(pixel p) {
	return theoretica::sqrt(square(p.r) + square(p.g) + square(p.b));
}


pixel giulia::contrast(pixel p, real_t value) {
	return p * theoretica::powf(intensity(p), value);
}


pixel giulia::supersampling(
		real_t x, real_t y, global_state& state, draw_function draw, unsigned int order, real_t stepsize) {

	if(order == 1)
		return draw(x, y, state);

	if(order % 2 != 0)
		return pixel(0, 0, 0);

	if(stepsize == 0)
		stepsize = 1.0 / state["width"];

	// Grid pattern
	const real_t x1 = x + (stepsize);
	const real_t x2 = x + (stepsize * 3);
	const real_t y1 = y + (stepsize);
	const real_t y2 = y + (stepsize * 3);

	pixel p1 = supersampling(x1, y1, state, draw, order / 2, stepsize / 2.0);
	pixel p2 = supersampling(x1, y2, state, draw, order / 2, stepsize / 2.0);
	pixel p3 = supersampling(x2, y1, state, draw, order / 2, stepsize / 2.0);
	pixel p4 = supersampling(x2, y2, state, draw, order / 2, stepsize / 2.0);

	unsigned int r = (((unsigned int) p1.r) + p2.r + p3.r + p4.r) / 4;
	unsigned int g = (((unsigned int) p1.g) + p2.g + p3.g + p4.g) / 4;
	unsigned int b = (((unsigned int) p1.b) + p2.b + p3.b + p4.b) / 4;

	return pixel(r, g, b);
}
