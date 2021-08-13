#include <mass.h>
#include <stdlib.h>
#include <string.h>

/*
------------------------
 -> Mesh Type Casting <- 
------------------------
*/

vmesh_t* mesh_to_vmesh(mesh_t* m)
{
    vmesh_t* mesh = (vmesh_t*)malloc(sizeof(vmesh_t));
    mesh->vertices = array_new(256, sizeof(vertex_t));
    mesh->indices = NULL;

    if (m->vertices != NULL && m->normals != NULL && m->uvs != NULL) {
        mesh->type = OBJ_VTN;
    }
    else if (m->vertices != NULL && m->normals != NULL) {
        mesh->type = OBJ_VN;
    }
    else mesh->type = OBJ_V;

    if (mesh->type == OBJ_VTN) {
        mesh->indices = array_new(m->vertices->used, sizeof(unsigned int));
        for (unsigned int i = 0; i < m->vertices->used; i++) {
            unsigned int found = 0;
            unsigned int k = mesh->vertices->used;
            vertex_t vertex;
            vertex.position = *(vec3*)array_index(m->vertices, i); 
            vertex.uv = *(vec2*)array_index(m->uvs, i); 
            vertex.normal = *(vec3*)array_index(m->normals, i);

            for (unsigned int j = 0; j < mesh->vertices->used; j++) {
                if (!memcmp(&vertex, array_index(mesh->vertices, j), sizeof(vertex_t))) {
                    k = (unsigned int)j;
                    found++;
                    break;
                }
            }

            if (!found) array_push(mesh->vertices, &vertex);
            array_push(mesh->indices, &k);
        }
        array_cut(mesh->vertices);
    }
    else if (mesh->type != OBJ_V) {
        const vec2 uv = {0.0f, 0.0f};
        mesh->indices = array_new(m->vertices->used, sizeof(unsigned int));
        for (unsigned int i = 0; i < m->vertices->used; i++) {
            unsigned int found = 0;
            unsigned int k = mesh->vertices->used;
            vertex_t vertex;
            vertex.position = *(vec3*)array_index(m->vertices, i); 
            vertex.uv = uv;
            vertex.normal = *(vec3*)array_index(m->normals, i);

            for (unsigned int j = 0; j < mesh->vertices->used; j++) {
                if (!memcmp(&vertex, array_index(mesh->vertices, j), sizeof(vertex_t))) {
                    k = (unsigned int)j;
                    found++;
                    break;
                }
            }

            if (!found) array_push(mesh->vertices, &vertex);
            array_push(mesh->indices, &k);
        }
        array_cut(mesh->vertices);
    } else {
        const vec2 uv = {0.0f, 0.0f};
        const vec3 norm = {0.0f, 1.0f, 0.0f};
        mesh->indices = array_new(m->vertices->used, sizeof(unsigned int));
        for (unsigned int i = 0; i < m->vertices->used; i++) {
            unsigned int found = 0;
            unsigned int k = mesh->vertices->used;
            vertex_t vertex;
            vertex.position = *(vec3*)array_index(m->vertices, i); 
            vertex.uv = uv;
            vertex.normal = norm;

            for (unsigned int j = 0; j < mesh->vertices->used; j++) {
                if (!memcmp(&vertex, array_index(mesh->vertices, j), sizeof(vertex_t))) {
                    k = (unsigned int)j;
                    found++;
                    break;
                }
            }

            if (!found) array_push(mesh->vertices, &vertex);
            array_push(mesh->indices, &k);
        }
        array_cut(mesh->vertices);
    }

    if (mesh->indices == NULL) {
        array_free(mesh->vertices);
        free(mesh);
        return NULL;
    } 
    return mesh;
} 

vmesh_t* mesh_to_vmesh_optim(mesh_t* m)
{
    vmesh_t* mesh = NULL;
    if (m->vertices != NULL && m->normals != NULL) {
        const vec2 uv = {0.0f, 0.0f};

        mesh = (vmesh_t*)malloc(sizeof(vmesh_t));
        mesh->vertices = array_new(m->vertices->used, sizeof(vertex_t));
        mesh->indices = array_new(m->vertices->used, sizeof(unsigned int));
        mesh->type = OBJ_VN;

        for (unsigned int i = 0; i < m->vertices->used; i++) {
            unsigned int k = mesh->vertices->used;
            vec3* pos = (vec3*)array_index(m->vertices, i);
            vec3 norm = *(vec3*)array_index(m->normals, i);
            for (unsigned int j = 0; j < mesh->vertices->used; j++) {
                vertex_t* vptr = (vertex_t*)array_index(mesh->vertices, j);
                if (!memcmp(pos, &vptr->position, sizeof(vec3))) {
                    norm = vec3_add(vptr->normal, norm);
                    k = (unsigned int)j;
                    break;
                }
            }

            if (k == mesh->vertices->used) {
                vertex_t v = vertex_new(*pos, uv, norm);
                array_push(mesh->vertices, &v);
            }
            array_push(mesh->indices, &k);
        }
        array_cut(mesh->vertices);
        vertex_array_normalize_normals(mesh->vertices->data, mesh->vertices->used);
    }
    return mesh;
}

mesh_t* vmesh_to_mesh(vmesh_t* m)
{
    mesh_t* mesh = (mesh_t*)malloc(sizeof(mesh_t));
    mesh->normals = NULL;
    mesh->uvs = NULL;

    mesh->vertices = array_new(m->indices->used, sizeof(vec3));
    if (m->type == OBJ_VTN) {
        mesh->uvs = array_new(m->indices->used, sizeof(vec2));
    }
    if (m->type == OBJ_VN || m->type == OBJ_VTN) {
        mesh->normals = array_new(m->indices->used, sizeof(vec3));
    } 

    for (unsigned int i = 0; i < m->indices->used; i++) {
        vertex_t v = *(vertex_t*)array_index(m->vertices, i);
        vec3 pos = v.position;
        array_push(mesh->vertices, &pos);
        if (m->type == OBJ_VTN) {
            vec2 uv = v.uv;
            array_push(mesh->uvs, &uv);
        }
        if (m->type == OBJ_VN || m->type == OBJ_VTN) {
            vec3 norm = v.normal;
            array_push(mesh->normals, &norm);
        }
    }
    return mesh;
}