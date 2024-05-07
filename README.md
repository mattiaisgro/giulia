# Giulia
A lightweight rendering and image processing framework for procedural art.
![Julia fractal](https://github.com/chaotic-society/giulia/raw/main/giulia.png)
_Giulia's fractal_

## Introduction
Giulia works by calling three custom functions provided by the user:
- A `setup` function, called once at the start of the rendering process.
- A `draw` function, called for every pixel of the output image.
- A `postprocess` function, called after rendering the image to modify it as a whole.

## Features
- Fractal rendering
- 3D rendering using raymarching
- Common image postprocessing filters

## Example

The `setup` function takes in by reference the `global_state` of the rendering pipeline, without returning anything:
```cpp
void setup(global_state& state) {
  
  // Set a global variable named "example"
  state["example"] = 0;
}
```

The `draw` function takes in the **normalized Cartesian coordinates** (between -1 and 1) of the current pixel and the global state, and it returns the computed pixel color:
```cpp
pixel draw(real_t x, real_t y, global_state& state) {

  // Draw a very special Julia fractal
  return draw_giulia_present(x, y);
}
```

The `postprocess` takes as arguments the rendered image and the global state, returning nothing:
```cpp
void postprocess(image& img, global_state& state) {

  // Draw a Sierpinski triangle on top of the output image
  draw_sierpinski_triangle(img);
  
  // Apply contrast to the output image
  contrast(img, 0.9, 0);
}
```

## Gallery
Some images rendered using Giulia can be seen at [chaotic-society.github.io/gallery](https://chaotic-society.github.io/gallery/)
