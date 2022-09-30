#pragma once

#define THEORETICA_LONG_DOUBLE_PREC
#include "theoretica/algebra/vec.h"
using theoretica::vec2;

#include "image.h"

#include <vector>
#include <functional>


namespace giulia {

	// Draw a Voronoi diagram given a set of points <p>, a set of colors
	// for each point <c> and a distance function <d>
	pixel voronoi_diagram(
		real_t x, real_t y,
		std::vector<vec2> p, std::vector<pixel> c,
		std::function<real_t(vec2, vec2)> d);

}
