#pragma once

#define THEORETICA_LONG_DOUBLE_PREC
#include "theoretica/algebra/vec.h"
using theoretica::vec3;
using theoretica::real;

#include "image.h"
#include <functional>


namespace giulia {


	// A distance estimated object
	struct de_object {

		// Estimated distance to the object
		real distance;

		// Resulting position after raymarching
		vec3 position;

		// Estimated normal to surface
		vec3 normal;

		// Object color
		pixel color;
	};


	// Raymarching routine
	pixel raymarch(
		std::function<de_object(vec3)> DE,
		vec3 camera,
		vec3 direction = vec3({0, 0, -1}),
		real min_distance = 0.0001,
		unsigned int max_steps = 1000,
		bool lighting = true);


	// Render the Mandelbulb fractal
	de_object mandelbulb(vec3 pos);
	
}
