# mass

> Simple C library to handle 2D and 3D meshes and models.
> This library is meant to be extremely simple and lightweight, it is not a full 
> extensible 3D model loader by any means. Usable in simple OpenGL projects or 3D games.

* 3D Meshes (Vertices, UV coordinates, Normals)
* 2D Polygons and Meshes
* Procedural Creation of 2D and 3D Shapes
* Extremely Simple .OBJ Wavefront Saver and Loader

> Meshes can be arbitrarily large. The complete API can of the library can be found 
> in the single header of the library, mass.h. The main object interface 
> is the mesh structure:

```C

typedef struct Mesh3D {
    struct vector vertices;
    struct vector normals;
    struct vector uvs;
} Mesh3D;

```

## Example

```C

#include <mass.h>

/* Create a cube and save it as cube.obj model */

int main(void)
{
    Mesh3D mesh = mesh3D_shape_cube(1);
    mesh3D_normalize_faces(&mesh);
    mesh3D_save(&mesh, "cube.obj");
    mesh3D_free(&mesh);
    return 0;
}

```

## Dependencies

> The two projects on which mass depends are small and depend only
> on the C standard library, so they can be built easily.

* [fract](https://github.com/LogicEu/fract.git): Small 2D and 3D math library for games and graphics

* [utopia](https://github.com/LogicEu/utopia.git): Collection of generic containers and data structures in C

## Clone

> Be sure to clone the two submodules of the project, otherwise you won't be able to
> link successfully due to missing dependencies.

```shell
git clone --recursive https://github.com/LogicEu/mass.git
```

## Build

> You can compile the library with either of the two build scripts in this repo
> using make or bash. The compiled library will be placed at /bin

```shell
make all -j # or ./build.sh all
```
> There is a simple installation function that builds the
> library and puts it in the /usr/local directory:

```shell
sudo make install # or sudo ./build.sh install
```

> Clean object files and binaries created during compilation in /tmp 
> and /bin with:

```shell
make clean # or ./build.sh clean
```

