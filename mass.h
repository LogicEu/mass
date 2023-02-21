
/*  Copyright (c) 2022 Eugenio Arteaga A.

Permission is hereby granted, free of charge, to any 
person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the 
Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to 
permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice 
shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  */

#ifndef MASS_MESHES_H
#define MASS_MESHES_H

#ifdef __cplusplus
extern "C" {
#endif

/*====================================================
>>>>>>>>>>>>>>>  MASS, MESH & MODELS   >>>>>>>>>>>>>>>
C library for handling 2D and 3D meshes and models.
=============================== @Eugenio Arteaga A. */

#include <fract.h>
#include <utopia/vector.h>
#include <utopia/table.h>

/* <===> MASS COMMON <===> */

const vec2* mass_shape_quad(void);
const vec3* mass_shape_cube(void);
const vec2* mass_shape_cube_uvs(void);
const vec3* mass_shape_skybox(void);
size_t mass_shape_quad_size(void);
size_t mass_shape_cube_size(void);
size_t mass_shape_cube_uvs_size(void);
size_t mass_shape_skybox_size(void);


/* <===> MASS 3D <===> */

typedef struct Mesh3D {
    struct vector vertices;
    struct vector normals;
    struct vector uvs;
} Mesh3D;

typedef struct iMesh3D {
    struct table vertices;
    struct table normals;
    struct table uvs;
} iMesh3D;

/* --- Wavefront OBJ File Format I/O --- */
iMesh3D imesh3D_load_wavefront(const char* path);
void imesh3D_save_wavefront(const iMesh3D* mesh, const char* path);
void mesh3D_save_wavefront(const Mesh3D* mesh, const char* path);

/* --- Indexed 3D Mesh --- */
iMesh3D imesh3D_create(void);
iMesh3D imesh3D_load(const char* path);
iMesh3D imesh3D_shape_quad(const vec3 position);
iMesh3D imesh3D_shape_plane(const size_t width, const size_t height);
iMesh3D imesh3D_shape_cube(const size_t size);
iMesh3D imesh3D_shape_hex(const vec3 size);
iMesh3D imesh3D_shape_sphere(const size_t detail);
void imesh3D_save(const iMesh3D* mesh, const char* path);
void imesh3D_free(iMesh3D* mesh);

void imesh3D_push_quad_z(iMesh3D* mesh, const vec3 position);
void imesh3D_push_quad_xyz(iMesh3D* mesh, const vec3 position, const vec3 xvec, const vec3 yvec);
void imesh3D_push_plane_z(iMesh3D* mesh, const size_t xsize, const size_t ysize);
void imesh3D_push_plane_xyz(iMesh3D* mesh, const size_t xsize, const size_t ysize, const vec3 position, const vec3 xvec, const vec3 yvec);
void imesh3D_normalize_faces(iMesh3D* mesh);
void imesh3D_scale(iMesh3D* mesh, const float f);
void imesh3D_move(iMesh3D* mesh, const vec3 add);
void imesh3D_normalize(iMesh3D* mesh);
void imesh3D_merge(iMesh3D* m1, iMesh3D* m2);

/* --- 3D Mesh --- */
Mesh3D mesh3D_create(void);
Mesh3D mesh3D_load(const char* path);
Mesh3D mesh3D_shape_quad(const vec3 position);
Mesh3D mesh3D_shape_plane(const size_t width, const size_t height);
Mesh3D mesh3D_shape_cube(const size_t size);
Mesh3D mesh3D_shape_hex(const vec3 size);
Mesh3D mesh3D_shape_sphere(const size_t detail);
void mesh3D_save(const Mesh3D* mesh, const char* path);
void mesh3D_save_quick(const Mesh3D* mesh, const char* path);
void mesh3D_free(Mesh3D* mesh);

void mesh3D_push_quad_z(Mesh3D* mesh, const vec3 position);
void mesh3D_push_quad_xyz(Mesh3D* mesh, const vec3 position, const vec3 xvec, const vec3 yvec);
void mesh3D_push_plane_z(Mesh3D* mesh, const size_t xsize, const size_t ysize);
void mesh3D_push_plane_xyz(Mesh3D* mesh, const size_t xsize, const size_t ysize, const vec3 position, const vec3 xvec, const vec3 yvec);
void mesh3D_normalize_faces(Mesh3D* mesh);
void mesh3D_scale(Mesh3D* mesh, const float f);
void mesh3D_move(Mesh3D* mesh, const vec3 add);
void mesh3D_normalize(Mesh3D* mesh);
void mesh3D_merge(Mesh3D* m1, Mesh3D* m2);

/* --- 3D Mesh Type Convertions --- */
Mesh3D imesh3D_to_mesh3D(const iMesh3D* mesh);
iMesh3D mesh3D_to_imesh3D(const Mesh3D* mesh);


/* <===> MASS 2D <===> */

typedef struct Mesh2D {
    struct vector vertices;
    struct vector uvs;
} Mesh2D;

typedef struct iMesh2D {
    struct table vertices;
    struct table uvs;
} iMesh2D;

/* --- Indexed 2D Mesh --- */
iMesh2D imesh2D_create(void);
iMesh2D imesh2D_shape_quad(const vec2 position);
iMesh2D imesh2D_shape_plane(const size_t width, const size_t height);
iMesh2D imesh2D_shape_polygon(const size_t sides);
void imesh2D_free(iMesh2D* mesh);

void imesh2D_push_quad(iMesh2D* mesh, const vec2 position);
void imesh2D_push_plane(iMesh2D* mesh, const size_t xsize, const size_t ysize);
void imesh2D_scale(iMesh2D* mesh, const float f);
void imesh2D_move(iMesh2D* mesh, const vec2 add);
void imesh2D_merge(iMesh2D* m1, iMesh2D* m2);

/* --- 2D Mesh --- */
Mesh2D mesh2D_create(void);
Mesh2D mesh2D_shape_quad(const vec2 position);
Mesh2D mesh2D_shape_plane(const size_t width, const size_t height);
Mesh2D mesh2D_shape_polygon(const size_t sides);
void mesh2D_free(Mesh2D* mesh);

void mesh2D_push_quad(Mesh2D* mesh, const vec2 position);
void mesh2D_push_plane(Mesh2D* mesh, const size_t xsize, const size_t ysize);
void mesh2D_scale(Mesh2D* mesh, const float f);
void mesh2D_move(Mesh2D* mesh, const vec2 add);
void mesh2D_merge(Mesh2D* m1, Mesh2D* m2);

/* --- 2D Mesh Type Convertions --- */
Mesh2D imesh2D_to_mesh2D(const iMesh2D* mesh);
iMesh2D mesh2D_to_imesh2D(const Mesh2D* mesh);

#ifdef __cplusplus
}
#endif
#endif
