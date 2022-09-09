#include "raymarching.h"
#include "theoretica/interpolation/spline_interp.h"

using namespace giulia;
using namespace theoretica;
namespace th = theoretica;


pixel giulia::raymarch(
	SDF f, vec3 camera, vec3 direction, pixel background,
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
		obj = f(pos);
		tot_distance += obj.distance;
		obj.position = pos;

		if(obj.distance < minimum)
			minimum = obj.distance;

		// Stop when close enough
		if (obj.distance < min_distance)
			break;
	}

	// Set the distance of the object to the total computed distance
	obj.distance = tot_distance;

	if(i == max_steps) {

		// Soft border
		// real border_gradient = (1 - minimum * 10);
		// obj.color = pixel(
		// 	clamp(obj.color.r * border_gradient, 0, 255),
		// 	clamp(obj.color.g * border_gradient, 0, 255),
		// 	clamp(obj.color.b * border_gradient, 0, 255)
		// );

		obj.color = background;

	} else {

		// Simple ambient occlusion
		real coeff = square(1 - i / (real) max_steps);
		
		obj.color = pixel(
			clamp(obj.color.r * coeff, 0, 255),
			clamp(obj.color.g * coeff, 0, 255),
			clamp(obj.color.b * coeff, 0, 255)
		);

		// Finite differences method for normal approximation
		if(lighting) {

			obj.normal = vec3({
				f(pos + vec3({1, 0, 0})).distance - f(pos - vec3({1, 0, 0})).distance,
				f(pos + vec3({0, 1, 0})).distance - f(pos - vec3({0, 1, 0})).distance,
				f(pos + vec3({0, 0, 1})).distance - f(pos - vec3({0, 0, 1})).distance
			}).normalized();

		} else {
			obj.normal = {0, 0, 0};
		}
	}

	return obj.color;
}


de_object giulia::sdf::obj_union(de_object a, de_object b) {

	return (a.distance < b.distance) ? a : b;
}


de_object giulia::sdf::obj_union(std::vector<de_object> v) {

	if(!v.size())
		return de_object(0, pixel(0));

	real m = v[0].distance;
	unsigned int index = 0;

	for (unsigned int i = 0; i < v.size(); ++i) {
		if(v[i].distance < m) {
			m = v[i].distance;
			index = i;
		}
	}

	return v[index];
}


de_object giulia::sdf::obj_blend(de_object a, de_object b, real k) {

	real h = clamp(0.5 + 0.5 * (a.distance - b.distance) / k, 0.0, 1.0);
	
    return de_object(
    	th::lerp(a.distance, b.distance, h) - k * h * (1.0 - h),
		pixel(
			a.color.r + ((int) b.color.r - a.color.r) * h,
			a.color.g + ((int) b.color.g - a.color.g) * h,
			a.color.b + ((int) b.color.b - a.color.b) * h
		));
}


de_object giulia::sdf::obj_intersection(de_object a, de_object b) {

	return (a.distance > b.distance) ? a : b;
}


de_object giulia::sdf::obj_difference(de_object a, de_object b) {

	return (-a.distance > b.distance) ? a : b;
}



de_object giulia::sdf::sphere(vec3 pos, vec3 center, real radius, pixel color) {

	return de_object((pos - center).length() - radius, color);
}


de_object giulia::sdf::mandelbulb(vec3 pos) {

	vec3 z = pos;
	real dr = 1;
	real r = 0;
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

		const real sin_theta = th::sin(theta);
		
		// Cartesian coordinates
		z = zr * vec3({
			sin_theta * th::cos(phi),
			sin_theta * th::sin(phi),
			th::cos(theta)});

		z += pos;
	}

	de_object obj;
	obj.distance = 0.5 * th::ln(r) * r / dr;
	obj.color = pixel(255, 255, 255);

	return obj;
}
