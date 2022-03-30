#include <mass.h>
#include <stdlib.h>
#include <string.h>

/*
------------------------
 -> Mesh Type Casting <- 
------------------------
*/

vmesh_t mesh_to_vmesh(mesh_t* m)
{
    vmesh_t mesh;
    mesh.vertices = array_create(sizeof(vertex_t));
    mesh.indices = array_create(sizeof(unsigned int));

    if (m->vertices.size && m->normals.size && m->uvs.size) {
        mesh.type = OBJ_VTN;
    }
    else if (m->vertices.size && m->normals.size) {
        mesh.type = OBJ_VN;
    }
    else mesh.type = OBJ_V;

    if (mesh.type == OBJ_VTN) {
        for (unsigned int i = 0; i < m->vertices.size; i++) {
            unsigned int found = 0;
            unsigned int k = mesh.vertices.size;
            vertex_t vertex;
            vertex.position = *(vec3*)array_index(&m->vertices, i); 
            vertex.uv = *(vec2*)array_index(&m->uvs, i); 
            vertex.normal = *(vec3*)array_index(&m->normals, i);

            for (unsigned int j = 0; j < mesh.vertices.size; j++) {
                if (!memcmp(&vertex, array_index(&mesh.vertices, j), sizeof(vertex_t))) {
                    k = (unsigned int)j;
                    found++;
                    break;
                }
            }

            if (!found) array_push(&mesh.vertices, &vertex);
            array_push(&mesh.indices, &k);
        }
        array_cut(&mesh.vertices);
    }
    else if (mesh.type != OBJ_V) {
        const vec2 uv = {0.0f, 0.0f};
        for (unsigned int i = 0; i < m->vertices.size; i++) {
            unsigned int found = 0;
            unsigned int k = mesh.vertices.size;
            vertex_t vertex;
            vertex.position = *(vec3*)array_index(&m->vertices, i); 
            vertex.uv = uv;
            vertex.normal = *(vec3*)array_index(&m->normals, i);

            for (unsigned int j = 0; j < mesh.vertices.size; j++) {
                if (!memcmp(&vertex, array_index(&mesh.vertices, j), sizeof(vertex_t))) {
                    k = (unsigned int)j;
                    found++;
                    break;
                }
            }

            if (!found) array_push(&mesh.vertices, &vertex);
            array_push(&mesh.indices, &k);
        }
        array_cut(&mesh.vertices);
    } else {
        const vec2 uv = {0.0f, 0.0f};
        const vec3 norm = {0.0f, 1.0f, 0.0f};
        for (unsigned int i = 0; i < m->vertices.size; i++) {
            unsigned int found = 0;
            unsigned int k = mesh.vertices.size;
            vertex_t vertex;
            vertex.position = *(vec3*)array_index(&m->vertices, i); 
            vertex.uv = uv;
            vertex.normal = norm;

            for (unsigned int j = 0; j < mesh.vertices.size; j++) {
                if (!memcmp(&vertex, array_index(&mesh.vertices, j), sizeof(vertex_t))) {
                    k = (unsigned int)j;
                    found++;
                    break;
                }
            }

            if (!found) array_push(&mesh.vertices, &vertex);
            array_push(&mesh.indices, &k);
        }
        array_cut(&mesh.vertices);
    }

    if (!mesh.indices.size) {
        array_free(&mesh.vertices);
    } 
    return mesh;
} 

vmesh_t mesh_to_vmesh_optim(mesh_t* m)
{
    vmesh_t mesh;
    if (m->vertices.size && m->normals.size) {
        const vec2 uv = {0.0f, 0.0f};

        mesh.vertices = array_reserve(sizeof(vertex_t), m->vertices.size);
        mesh.indices = array_reserve(sizeof(unsigned int), m->vertices.size);
        mesh.type = OBJ_VN;

        for (unsigned int i = 0; i < m->vertices.size; i++) {
            unsigned int k = mesh.vertices.size;
            vec3* pos = (vec3*)array_index(&m->vertices, i);
            vec3 norm = *(vec3*)array_index(&m->normals, i);
            for (unsigned int j = 0; j < mesh.vertices.size; j++) {
                vertex_t* vptr = (vertex_t*)array_index(&mesh.vertices, j);
                if (!memcmp(pos, &vptr->position, sizeof(vec3))) {
                    norm = vec3_add(vptr->normal, norm);
                    k = (unsigned int)j;
                    break;
                }
            }

            if (k == mesh.vertices.size) {
                vertex_t v = vertex_new(*pos, uv, norm);
                array_push(&mesh.vertices, &v);
            }
            array_push(&mesh.indices, &k);
        }
        array_cut(&mesh.vertices);
        vertex_array_normalize_normals(mesh.vertices.data, mesh.vertices.size);
        return mesh;
    }
    return vmesh_new();
}

mesh_t vmesh_to_mesh(vmesh_t* m)
{
    mesh_t mesh;
    mesh.vertices = array_reserve(sizeof(vec3), m->indices.size);
    
    if (m->type == OBJ_VTN) {
        mesh.uvs = array_reserve(sizeof(vec2), m->indices.size);
    } else mesh.uvs = array_create(sizeof(vec2));
    
    if (m->type == OBJ_VN || m->type == OBJ_VTN) {
        mesh.normals = array_reserve(sizeof(vec3), m->indices.size);
    } else mesh.normals = array_create(sizeof(vec3));

    for (unsigned int i = 0; i < m->indices.size; i++) {
        vertex_t v = *(vertex_t*)array_index(&m->vertices, i);
        vec3 pos = v.position;
        array_push(&mesh.vertices, &pos);
        if (m->type == OBJ_VTN) {
            vec2 uv = v.uv;
            array_push(&mesh.uvs, &uv);
        }
        if (m->type == OBJ_VN || m->type == OBJ_VTN) {
            vec3 norm = v.normal;
            array_push(&mesh.normals, &norm);
        }
        return mesh;
    }
    return mesh_new();
}