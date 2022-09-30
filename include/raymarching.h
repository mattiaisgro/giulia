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

		// Object material (defaults to 0 = none)
		unsigned int material {0};

		de_object() {}

		de_object(real d) : distance(d) {}

		de_object(real d, pixel c) : distance(d), color(c) {}

	};


	// A signed distance function
	using SDF = std::function<de_object(vec3)>;


	// Raymarching routine
	pixel raymarch(
		SDF f,
		vec3 camera,
		vec3 direction = vec3({0, 0, -1}),
		pixel background = pixel(0, 0, 0),
		real min_distance = 0.001,
		unsigned int max_steps = 100,
		bool lighting = false);


	// Signed distance functions
	namespace sdf {


		// Union of objects
		de_object obj_union(de_object a, de_object b);


		// Union of objects
		de_object obj_union(std::vector<de_object> v);


		// Blend objects
		de_object obj_blend(de_object a, de_object b, real k);


		// Intersection of objects
		de_object obj_intersection(de_object a, de_object b);


		// Object difference
		de_object obj_difference(de_object a, de_object b);


		// SDF for the sphere
		de_object sphere(vec3 pos, vec3 center, real radius, pixel color);


		// SDF for the Mandelbulb fractal
		de_object mandelbulb(vec3 pos);

	}
	
}
