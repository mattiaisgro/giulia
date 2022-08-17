#pragma once

// Image storage and saving

#include "common.h"
#include <string>
#include <vector>
#include <functional>


namespace giulia {


	// An RGB pixel
	struct pixel {
		
		unsigned char r;
		unsigned char g;
		unsigned char b;

		pixel() : r(0), g(0), b(0) {}

		pixel(unsigned char r, unsigned char g, unsigned char b)
			: r(r), g(g), b(b) {}

		pixel(unsigned int hex) {
			r = (hex & 0x00FF0000) >> 16;
			g = (hex & 0x0000FF00) >> 8;
			b = hex & 0x000000FF;
		}

		inline pixel operator+(pixel other) {
			return pixel(r + other.r, g + other.g, b + other.b);
		}

		inline pixel operator-(pixel other) {
			return pixel(r - other.r, g - other.g, b - other.b);
		}

		inline pixel operator*(real_t scalar) {
			return pixel(r * scalar, g * scalar, b * scalar);
		}

		inline pixel operator/(real_t scalar) {
			return pixel(r / scalar, g / scalar, b / scalar);
		}
	};


	// An RGB encoded image
	struct image {

		public:

			image(unsigned int w, unsigned int h) : width(w), height(h) {

				if(data.size())
					data.clear();
				
				data.resize(w * h);
			}

			inline pixel* get_data() const {
				return (pixel*) &(data[0]);
			}

			inline pixel get_pixel(unsigned int i) const {
				return data[i];
			}

			inline pixel get_pixel(unsigned int i, unsigned int j) const {
				return data[width * j + i];
			}

			inline unsigned int get_width() const {
				return width;
			}


			inline unsigned int get_height() const {
				return height;
			}

			inline pixel& operator[](unsigned int i) {
				return data[i];
			}


		private:
			unsigned int width {1024};
			unsigned int height {1024};
			std::vector<pixel> data;

	};

	// A pixel drawing function
	using draw_function = std::function<pixel(real_t, real_t, global_state&)>;


	// Overwrite a pixel corresponding to the given normalized Cartesian coordinates
	void overwrite(image& img, real_t x, real_t y, pixel c);


	// Save an image to file in the BMP format
	// Returns 0 on success
	int save_image(std::string filename, unsigned int width, unsigned int height, pixel* data);


	// Save an image to file in the BMP format
	// Returns 0 on success
	int save_image(std::string filename, image img);


	// Linear interpolation between pixels
	pixel lerp(pixel P1, pixel P2, real_t interp);


	// Pixel intensity
	real_t intensity(pixel p);


	// Apply contrast
	pixel contrast(pixel p, real_t value);


	// Supersampling Anti-aliasing with grid points
	pixel supersampling(
		real_t x, real_t y, global_state& state,
		draw_function draw, unsigned int order = 2, real_t stepsize = 0);

}
