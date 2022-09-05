#include "raymarching.h"
using namespace giulia;
using namespace theoretica;
namespace th = theoretica;


pixel giulia::raymarch(
	std::function<de_object(vec3)> DE, vec3 camera, vec3 direction,
	real min_distance, unsigned int max_steps, bool lighting) {


	unsigned int i = 0;
	real tot_distance = 0;
	real minimum = 10000000;
	de_object obj;
	vec3 pos;

	for (i = 0; i < max_steps; i++) {

		// Compute the current position
		pos = camera + direction * tot_distance;

		// Compute another step of distance estimation
		obj = DE(pos);
		tot_distance += obj.distance;
		obj.position = pos;

		if(obj.distance < minimum)
			minimum = obj.distance;

		// Stop when close enough
		if (obj.distance < min_distance)
			break;
	}

	obj.distance = tot_distance;

	if(i == max_steps) {

		// TO-DO Apply glow or soft border
		// float border_gradient = (1 - minimum * 5);
		// obj.color = pixel(
		// 	clamp(obj.color.r * border_gradient, 0, 255),
		// 	clamp(obj.color.g * border_gradient, 0, 255),
		// 	clamp(obj.color.b * border_gradient, 0, 255)
		// );

		obj.color = pixel(0, 0, 0);

	} else {

		real coeff = pow(1 - i / (real) max_steps, 10);
		obj.color = pixel(
			clamp(coeff * obj.color.r, 0, 255),
			clamp(coeff * obj.color.g, 0, 255),
			clamp(coeff * obj.color.b, 0, 255)
		);

		// Finite differences method for normal approximation
		if(lighting) {
			obj.normal = vec3({
				DE(pos + vec3({1, 0, 0})).distance - DE(pos - vec3({1, 0, 0})).distance,
				DE(pos + vec3({0, 1, 0})).distance - DE(pos - vec3({0, 1, 0})).distance,
				DE(pos + vec3({0, 0, 1})).distance - DE(pos - vec3({0, 0, 1})).distance
			}).normalized();
		} else {
			obj.normal = {0, 0, 0};
		}
	}

	return obj.color;
}



de_object giulia::mandelbulb(vec3 pos) {

	vec3 z = pos;
	real dr = 1.0;
	real r = 0.0;
	int power = 8;

	for (int i = 0; i < 10; i++) {

		r = z.length();

		// Escape radius is 2
		if (r > 2)
			break;
		
		// Polar coordinates
		real theta = th::acos(z[2] / r);
		real phi = th::atan2(z[1], z[0]);

		const real r_pow_1 = th::pow(r, power - 1);
		dr = r_pow_1 * power * dr + 1;
		
		// Scale and rotate the point
		const real zr = r_pow_1 * r;
		theta = theta * power;
		phi = phi * power;
		
		// Cartesian coordinates
		z = zr * vec3({
			th::sin(theta) * th::cos(phi),
			th::sin(theta) * th::sin(phi),
			th::cos(theta)});

		z += pos;
	}

	de_object obj;
	obj.distance = 0.5 * th::ln(r) * r / dr;
	obj.color = pixel(255, 255, 255);

	return obj;
}
