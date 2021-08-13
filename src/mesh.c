#include <mass.h>
#include <stdlib.h>

/*
--------------------------
 -> 3D Mesh of Vectors <- 
--------------------------
*/

mesh_t* mesh_load(const char* path)
{
    obj_flag type = OBJ_V;
    array_t* v_index = array_new(16, sizeof(unsigned int));
    array_t* u_index = array_new(16, sizeof(unsigned int));
    array_t* n_index = array_new(16, sizeof(unsigned int));
    array_t* temp_positions = array_new(16, sizeof(vec3));
    array_t* temp_normals = array_new(16, sizeof(vec3));
    array_t* temp_uvs = array_new(16, sizeof(vec2));

    wavefront_file_load(path, &type, v_index, u_index, n_index, temp_positions, temp_uvs, temp_normals);

    mesh_t* mesh = (mesh_t*)malloc(sizeof(mesh_t));
    mesh->vertices = vertex_array_by_index_array(v_index, temp_positions);
    mesh->uvs = vertex_array_by_index_array(u_index, temp_uvs);
    mesh->normals = vertex_array_by_index_array(n_index, temp_normals);
    if (mesh->normals == NULL) mesh->normals = vec3_face_normal_array(mesh->vertices);
    return mesh;
}

void mesh_free(mesh_t* mesh)
{
    array_free(mesh->vertices);
    array_free(mesh->uvs);
    array_free(mesh->normals);
    free(mesh);
}

mesh_t* mesh_shape_quad(float x, float y)
{   
    mesh_t* mesh = (mesh_t*)malloc(sizeof(mesh_t));
    mesh->vertices = array_new(6, sizeof(vec3));
    vec3_position_array_push_quad_z(mesh->vertices, x, y);
    mesh->uvs = NULL;
    mesh->normals = NULL;
    return mesh;
}

mesh_t* mesh_shape_plane(const unsigned int width, const unsigned int height)
{   
    mesh_t* mesh = (mesh_t*)malloc(sizeof(mesh_t));
    mesh->vertices = array_new(width * height * 6, sizeof(vec3));
    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int z = 0; z < height; z++) {
            vec3_position_array_push_quad_z(mesh->vertices, (float)x, (float)z);
        }
    }
    mesh->uvs = NULL;
    mesh->normals = NULL;
    return mesh;
}