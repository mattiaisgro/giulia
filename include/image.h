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

			// Construct an image of width <w> and height <h>
			image(unsigned int w, unsigned int h) : width(w), height(h) {

				if(data.size())
					data.clear();
				
				data.resize(w * h);
			}


			// Get raw pointer to image data
			pixel* get_data() const;


			// Get the pixel at index <i>
			pixel get_pixel(unsigned int i) const;


			// Get the pixel at horizontal index <i> and vertical index <j>
			pixel get_pixel(unsigned int i, unsigned int j) const;


			// Get width of the image
			unsigned int get_width() const;


			// Get height of the image
			unsigned int get_height() const;


			// Get total pixel size of the image
			unsigned int get_size() const;


			// Get pixel at index <i>
			// @see get_pixel(unsigned int)
			pixel& operator[](unsigned int i);


			// Save image to file as a Bitmap
			int save(const std::string& filename);


		private:
			unsigned int width {1024};
			unsigned int height {1024};
			std::vector<pixel> data;

	};

	// A pixel drawing function
	using draw_function = std::function<pixel(real_t, real_t, global_state&)>;


	// Apply a pixel modification to an image
	void apply(image& img, std::function<pixel(pixel)> f);


	// Apply a series of pixel modifications to an image
	void apply(image& img, std::vector<std::function<pixel(pixel)>> functions);


	// Overwrite a pixel corresponding to the given normalized Cartesian coordinates
	void overwrite(image& img, real_t x, real_t y, pixel c);


	// Apply a negative mask to an image
	void negative(image& img);


	// Apply a gray scale mask to an image
	void grayscale(image& img);


	// Apply gamma correction to an image
	void gamma_correction(image& img, real_t gamma, real_t c);


	// Apply contrast to an image
	void contrast(image& img, real_t a, unsigned char s);


	// Apply contrast thresholding to an image
	void contrast_threshold(image& img, real_t t);


	// Apply contrast stretching to an image
	void contrast_stretch(image& img,
		unsigned char min_in, unsigned char max_in,
		unsigned char min_out, unsigned char max_out);


	// Apply logarithmic transformation to an image
	void log(image& img, real_t c);


	// Linear interpolation between pixels
	pixel lerp(pixel P1, pixel P2, real_t interp);


	// Pixel intensity
	real_t intensity(pixel p);


	// Apply contrast to pixel
	pixel contrast(pixel p, real_t value);


	// Supersampling Anti-aliasing with grid points
	pixel supersampling(
		real_t x, real_t y, global_state& state,
		draw_function draw, unsigned int order = 2, real_t stepsize = 0);

}
