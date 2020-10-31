# Ray Tracer

This is an implementation of a ray tracer in C++. It takes a configuration file as input that describes a scene with objects, materials, lights, and textures, and uses a ray-tracing algorithm to produce a high definition image in .ppm format which can be viewed in many different image software.

This began as a project at the University of Minnesota's 5611 graphics course and has been expanded to what it is now.

### Features

- High quality, realistic recursive ray tracing algorithm
- Reflection
- Refraction
- Transparency
- Point lights and directional lights
- Spheres and polygons (meshes)
- Smooth shading
- Textures
- Phong lighting model and material properties

### Compilation and usage

Can be compiled using Visual Studio on Windows.

On Linux, run `make` in the RayTracer/ directory.

After compilation, can be run on the command line with a scene input file.

    $ make
    $ ./render scene.txt

### Still to implement

- Fix linting issues
- Expand documentation
- Implement spatial data structure, build k-d tree?
- Graphical interface
- Parallelization
- Config file builder

### Documentation

Documentation can be viewed [here](https://pages.github.umn.edu/walte735/RayTracer/).

### Sources

[Doxygen](https://www.doxygen.nl/index.html) used to generate HTML documentation.
