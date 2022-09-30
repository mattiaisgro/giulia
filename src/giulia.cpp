
#define THEORETICA_LONG_DOUBLE_PREC
#include "theoretica/theoretica.h"

#include "common.h"
#include "image.h"
#include "fractals.h"
#include "raymarching.h"
#include "geometry.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace giulia;


// Setup rendering variables
void setup(global_state& state) {

	state["scale.x"] = 10;
	state["scale.y"] = 10;

	state["translation.x"] = 0;
	state["translation.y"] = 0;
}


// Draw a pixel at the specified x and y
pixel draw(real_t x, real_t y, global_state& state) {

	// Register normalized coordinates before transformation
	real_t norm_x = x;
	real_t norm_y = y;

	// Coordinate transformation
	x = (x * state["scale.x"]) - state["translation.x"];
	y = (y * state["scale.y"]) - state["translation.y"];

	// Perspective
	vec3 camera = {0, 0, 0};
	vec3 direction = {norm_x, norm_y, -1};
	direction.normalize();

	pixel output = raymarch(
		[](vec3 pos) {

			return sdf::obj_blend(
				sdf::obj_blend(
					sdf::sphere(pos, {-1, 0, -6}, 1, pixel(0xFA0A0A)),
					sdf::sphere(pos, {1, 1, -7}, 1.5, pixel(0x0AFA0A)),
					0.7
				),
				sdf::sphere(pos, {-1, -1.5, -5.8}, 0.75, pixel(0x0A0AFA)),
				0.5);

		}, camera, direction);

	return output;
}


void postprocess(image& img, global_state& state) {
	// draw_sierpinski_triangle(img);
	// negative(img);
	// contrast(img, 0.9, 0);
	// grayscale(img);
}


int main(int argc, char const *argv[]) {

	// Changing seed
	int seed = time(nullptr);
	srand(seed);

	// Image width and height
	unsigned int width = 1024;
	unsigned int height = 1024;

	// Set width and height from terminal
	if(argc >= 4) {
		width = std::atoi(argv[2]);
		height = std::atoi(argv[3]);
	}

	// Aspect ratio of the image
	real_t aspect_ratio = width / (real_t) height;

	// Total size of the image
	unsigned int size = width * height;

	// Output file name
	std::string filename = "giulia.bmp";

	// Rendering progress counter
	unsigned int progress = 10;

	// Global state variables
	global_state state;
	state["seed"] = seed;
	state["width"] = width;
	state["height"] = height;
	state["aspect_ratio"] = aspect_ratio;
	state["iteration"] = 0;

	// Set supersampling level from terminal
	if(argc >= 5) {
		state["supersampling"] = std::atoi(argv[4]);
	} else {
		state["supersampling"] = 1;
	}

	// Read file name from terminal
	if(argc >= 2)
		filename = std::string(argv[1]);

	// Image data
	image img = image(width, height);

	// Setup global state before rendering
	setup(state);

	std::cout << "Rendering image ..." << std::endl;

	// Render the image
	for (int i = 0; i < size; ++i) {

		if((i == (size * progress / 100))) {
			std::cout << "[" << progress << "%]" << std::endl;
			progress += 10;
		}

		state["iteration"] += 1;
		
		// Convert index to pixel location
		real_t x = ((i % width) / (real_t) (width - 1)) - 0.5;
		real_t y = ((((size - i) / (real_t) width) / (real_t) height) - 0.5) / aspect_ratio;

		// The origin corresponds to the center of the image

		// Draw pixel
		img[i] = supersampling(x, y, state, draw, state["supersampling"], 0.25 / state["width"]);
	}

	std::cout << "[100%]" << std::endl;

	// Post-process the whole image
	std::cout << "Post-processing ..." << std::endl;
	postprocess(img, state);

	// Save the result to file
	std::cout << "Saving image as " << filename << " ..." << std::endl;
	int res = img.save(filename);

	if(res)	std::cout << "Failed writing to file" << res << std::endl;
	else	std::cout << "Successfully saved image" << std::endl;

	return res;
}

