#ifndef MASS_MESH_H
#define MASS_MESH_H

#ifdef __cplusplus
extern "C" {
#endif

/*

==================================================

>>>>>>>>>>>>>>  MASS MESH FRAME   >>>>>>>>>>>>>>>>

ANSI C99 header only mesh and vertex framework for 
handling 3D data models and files. 

=========================================  @eulogic

*/

#include <libfract.h>
#include <Utopia.h>

typedef enum {
    OBJ_V,
    OBJ_VN,
    OBJ_VTN
} obj_flag;

typedef struct {
    vec3 position;
    vec2 uv;
    vec3 normal;
} vertex_t;

typedef struct {
    array_t* vertices;
    array_t* normals;
    array_t* uvs;
} mesh_t;

typedef struct {
    array_t* vertices;
    array_t* indices;
    obj_flag type;
} vmesh_t;

typedef struct {
    unsigned int id, size;
    mesh_t* mesh;
} model_t;

/*
------------------------------------
 -> Wavefront OBJ File Format I/O <- 
------------------------------------
*/

void wavefront_file_load(const char* path, obj_flag* type, 
                        array_t* v_index, array_t* u_index, array_t* n_index,
                        array_t* temp_positions, array_t* temp_uvs, array_t* temp_normals);
void wavefront_file_write(const char* path, const unsigned int size,
                        array_t* temp_vertices, array_t* temp_uvs, array_t* temp_normals,
                        array_t* v_index, array_t* u_index, array_t* n_index);

/*
-----------------------------
 -> Useful Array Extension <- 
-----------------------------
*/

array_t* array_map(array_t* buffer);
void array_map_indexed(array_t* buffer, array_t** map, array_t** indices);
void array_push_indexed(array_t* map, array_t* indices, void* data);

/*
--------------------------
 -> 3D Vertex Data Type <- 
--------------------------
*/

vertex_t vertex_new(vec3 position, vec2 uv, vec3 normal);
void vertex_array_normalize_normals(vertex_t* v, const unsigned int size);
void vec3_normal_array_normalize(vec3* v, const unsigned int size);
array_t* vertex_array_by_index_array(array_t* index, array_t* buffer);

/*
-----------------------------
 -> Procedural Opertations <- 
-----------------------------
*/

void vec3_position_array_push_quad_z(array_t* positions, float x, float y);
void vertex_array_push_quad_z(array_t* vertices, array_t* indices, float x, float y);
void vertex_array_push_quad_xyz(array_t* vertices, array_t* indices, vec3 off, vec3 xvec, vec3 yvec);
void vertex_array_push_plane_z(vmesh_t* mesh, const unsigned int x_size, const unsigned int y_size);
void vertex_array_push_plane_xyz(vmesh_t* mesh, const unsigned int x_size, const unsigned int y_size, vec3 off, vec3 xvec, vec3 yvec);
array_t* vec3_face_normal_array(array_t* positions);
void vertex_array_set_face_normal(array_t* vertices, array_t* indices);

/*
--------------------------
 -> 3D Mesh of Vectors <- 
--------------------------
*/

mesh_t* mesh_load(const char* path);
void mesh_free(mesh_t* mesh);
mesh_t* mesh_shape_quad(float x, float y);
mesh_t* mesh_shape_plane(const unsigned int width, const unsigned int height);

/*
--------------------------
 -> 3D Mesh of Vertices <- 
--------------------------
*/

vmesh_t* vmesh_new();
vmesh_t* vmesh_shape_plane(const unsigned int width, const unsigned int height);
void vmesh_combine(vmesh_t* m1, vmesh_t* m2);
void vmesh_scale(vmesh_t* mesh, float f);
void vmesh_move(vmesh_t* mesh, vec3 add);
vmesh_t* vmesh_shape_cube(const unsigned int size);
void vmesh_normalize_positions(vmesh_t* mesh);
vmesh_t* vmesh_shape_sphere(unsigned int size);
void vmesh_smooth_optim(vmesh_t** m);
void vmesh_smooth_sphere(vmesh_t** m);
void vmesh_min_max_height(vmesh_t* mesh, float* min, float* max);
void vmesh_height_color_gradient(vmesh_t* mesh);
void vmesh_free(vmesh_t* mesh);
vmesh_t* vmesh_load(const char* path);
vmesh_t* vmesh_load_optim(const char* path);

/*
--------------------------
 -> 3D Mesh of Vectors <- 
--------------------------
*/

model_t* model_load(const char* path);
void model_free(model_t* model);

/*
-------------------------------
 -> Mass Mesh Write OBJ File <- 
-------------------------------
*/

void mesh_write_file(mesh_t* mesh, const char* path);
void vmesh_write_file(vmesh_t* mesh, const char* path);
void mesh_write_file_quick(mesh_t* mesh, const char* path);
void vmesh_write_file_quick(vmesh_t* mesh, const char* path);

/*
------------------------
 -> Mesh Type Casting <- 
------------------------
*/

vmesh_t* mesh_to_vmesh(mesh_t* m);
vmesh_t* mesh_to_vmesh_optim(mesh_t* m);
mesh_t* vmesh_to_mesh(vmesh_t* m);

#ifdef __cplusplus
}
#endif
#endif
