#ifndef MASS_MESH_H
#define MASS_MESH_H

#ifdef __cplusplus
extern "C" {
#endif

/*====================================================

>>>>>>>>>>>>>>  MASS, MESH & MODELS   >>>>>>>>>>>>>>>>

Mesh and vertex C library for handling 3D models. 

=============================== @Eugenio Arteaga A. */

#include <fract.h>
#include <utopia.h>

typedef struct mesh_t {
    array_t vertices;
    array_t normals;
    array_t uvs;
} mesh_t;

typedef struct imesh_t {
    table_t vertices;
    table_t normals;
    table_t uvs;
} imesh_t;

/* --- Wavefront OBJ File Format I/O --- */
imesh_t imesh_load_wavefront(const char* path);
void imesh_save_wavefront(const imesh_t* mesh, const char* path);
void mesh_save_wavefront(const mesh_t* mesh, const char* path);;

/* --- iMesh 3D --- */
imesh_t imesh_create(void);
imesh_t imesh_load(const char* path);
imesh_t imesh_shape_quad(const float x, const float y);
imesh_t imesh_shape_plane(const size_t width, const size_t height);
imesh_t imesh_shape_cube(const size_t size);
imesh_t imesh_shape_hex(const vec3 size);
imesh_t imesh_shape_sphere(const size_t detail);
void imesh_save(const imesh_t* mesh, const char* path);
void imesh_free(imesh_t* mesh);

/* --- Mesh of 3D Vectors --- */
mesh_t mesh_create(void);
mesh_t mesh_load(const char* path);
mesh_t mesh_shape_quad(const float x, const float y);
mesh_t mesh_shape_plane(const size_t width, const size_t height);
mesh_t mesh_shape_cube(const size_t size);
mesh_t mesh_shape_hex(const vec3 size);
mesh_t mesh_shape_sphere(const size_t detail);
void mesh_save(const mesh_t* mesh, const char* path);
void mesh_save_quick(const mesh_t* mesh, const char* path);
void mesh_free(mesh_t* mesh);

/* --- Procedural Operations --- */
void mesh_push_quad_z(mesh_t* mesh, const float x, const float y);
void imesh_push_quad_z(imesh_t* mesh, const float x, const float y);
void mesh_push_quad_xyz(mesh_t* mesh, const vec3 off, const vec3 xvec, const vec3 yvec);
void imesh_push_quad_xyz(imesh_t* mesh, const vec3 off, const vec3 xvec, const vec3 yvec);
void mesh_push_plane_z(mesh_t* mesh, const size_t xsize, const size_t ysize);
void imesh_push_plane_z(imesh_t* mesh, const size_t xsize, const size_t ysize);
void mesh_push_plane_xyz(mesh_t* mesh, const size_t xsize, const size_t ysize, const vec3 off, const vec3 xvec, const vec3 yvec);
void imesh_push_plane_xyz(imesh_t* mesh, const size_t xsize, const size_t ysize, const vec3 off, const vec3 xvec, const vec3 yvec);
void mesh_normals_get_face(mesh_t* mesh);
void imesh_normals_get_face(imesh_t* mesh);
void mesh_scale(mesh_t* mesh, const float f);
void imesh_scale(imesh_t* mesh, const float f);
void mesh_move(mesh_t* mesh, const vec3 add);
void imesh_move(imesh_t* mesh, const vec3 add);
void mesh_normalize(mesh_t* mesh);
void imesh_normalize(imesh_t* mesh);
void mesh_merge(mesh_t* m1, mesh_t* m2);
void imesh_merge(imesh_t* m1, imesh_t* m2);

/* --- Mesh Type Convertions --- */
mesh_t imesh_to_mesh(const imesh_t* mesh);
imesh_t mesh_to_imesh(const mesh_t* mesh);

#ifdef __cplusplus
}
#endif
#endif
