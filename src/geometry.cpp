#include "geometry.h"

using namespace giulia;
using namespace theoretica;


pixel giulia::voronoi_diagram(
	real_t x, real_t y, std::vector<vec2> p, std::vector<pixel> c,
	std::function<real_t(vec2, vec2)> d) {

	if(p.size() != c.size() || !p.size())
		return pixel(0, 0, 0);

	vec2 pos = {x, y};
	int index = 0;
	real_t min = d(pos, p[0]);

	for (int i = 1; i < p.size(); ++i) {

		const real_t dist = d(pos, p[i]);

		if(dist < min) {
			min = dist;
			index = i;
		}

	}

	return c[index];
}
