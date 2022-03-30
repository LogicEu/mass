#include <mass.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
-------------------------------
 -> Mass Mesh Write OBJ File <- 
-------------------------------
*/

void mesh_write_file(mesh_t* mesh, const char* path)
{
    if (!mesh) return;
    printf("Writing file '%s'\n", path);

    array_t temp_vertices = array_create(sizeof(vec3));
    array_t temp_uvs = array_create(sizeof(vec2));
    array_t temp_normals = array_create(sizeof(vec3));
    array_t v_index = array_create(sizeof(size_t));
    array_t u_index = array_create(sizeof(size_t));
    array_t n_index = array_create(sizeof(size_t));

    if (mesh->vertices.size) {
        array_map_indexed(&mesh->vertices, &temp_vertices, &v_index);
    }
    if (mesh->uvs.size) {
        array_map_indexed(&mesh->uvs, &temp_uvs, &u_index);
    }
    if (mesh->normals.size) {
        array_map_indexed(&mesh->normals, &temp_normals, &n_index);
    }

    wavefront_file_write(path, v_index.size / 3, &temp_vertices, &temp_uvs, &temp_normals, &v_index, &u_index, &n_index);
}

void mesh_write_file_quick(mesh_t* mesh, const char* path)
{
    if (!mesh) return;
    printf("Writing file '%s'\n", path);

    FILE* file = fopen(path, "w");
    if (!file) {
        printf("Could not open '%s' file.\n", path);
        exit(-1);
    }

    if (mesh->vertices.size) {
        for (unsigned int i = 0; i < mesh->vertices.size; i++) {
            vec3 v = *(vec3*)array_index(&mesh->vertices, i);
            fprintf(file, "v %f %f %f\n", v.x, v.y, v.z);
        }
    }
    if (mesh->uvs.size) {
        for (unsigned int i = 0; i < mesh->uvs.size; i++) {
            vec2 v = *(vec2*)array_index(&mesh->uvs, i);
            fprintf(file, "vt %f %f\n", v.x, v.y);
        }
    }
    if (mesh->normals.size) {
        for (unsigned int i = 0; i < mesh->normals.size; i++) {
            vec3 v = *(vec3*)array_index(&mesh->normals, i);
            fprintf(file, "vn %f %f %f\n", v.x, v.y, v.z);
        }
    }
    const unsigned int size = mesh->vertices.size / 3;
    if (mesh->normals.size && mesh->uvs.size) {
        for (unsigned int i = 0; i < size; i++) {
            fprintf(file, "f");
            for (unsigned int j = 0; j < 3; j++) {
                unsigned int k = i * 3 + j + 1;
                fprintf(file, " %u/%u/%u", k, k, k);
            }
            fprintf(file, "\n");
        }
    }
    else if (mesh->normals.size) {
        for (unsigned int i = 0; i < size; i++) {
            fprintf(file, "f");
            for (unsigned int j = 0; j < 3; j++) {
                unsigned int k = i * 3 + j + 1;
                fprintf(file, " %u//%u", k, k);
            }
            fprintf(file, "\n");
        }
    } else {
        for (unsigned int i = 0; i < size; i++) {
            fprintf(file, "f");
            for (unsigned int j = 0; j < 3; j++) {
                fprintf(file, " %u", i * 3 + j + 1);
            }
            fprintf(file, "\n");
        }
    }
    fclose(file);
    printf("Succesfully writed '%s' file.\n", path);
}

void vmesh_write_file(vmesh_t* mesh, const char* path)
{
    if (!mesh) return;
    
    array_t temp_vertices = array_reserve(sizeof(vec3), mesh->vertices.size);
    array_t v_index = array_reserve(mesh->indices.bytes, mesh->indices.size);
    array_t temp_uvs = array_create(sizeof(vec2));
    array_t u_index = array_create(sizeof(size_t));
    array_t temp_normals = array_create(sizeof(vec3));
    array_t n_index = array_create(sizeof(size_t));

    for (unsigned int i = 0; i < mesh->indices.size; i++) {
        unsigned int n = *(unsigned int*)array_index(&mesh->indices, i);
        vertex_t* v = (vertex_t*)array_index(&mesh->vertices, n);

        unsigned int k = temp_vertices.size, found = 0;
        for (unsigned int j = 0; j < temp_vertices.size; j++) {
            vec3 pos = *(vec3*)array_index(&temp_vertices, j);
            if (!memcmp(&v->position, &pos, sizeof(vec3))) {
                found++;
                k = (unsigned int)j;
                break;
            }
        }
        if (!found) array_push(&temp_vertices, &v->position);
        array_push(&v_index, &k);

        if (mesh->type == OBJ_VTN) {
            unsigned k = temp_uvs.size, found = 0;
            for (unsigned int j = 0; j < temp_uvs.size; j++) {
                vec2 uv = *(vec2*)array_index(&temp_uvs, j);
                if (!memcmp(&v->uv, &uv, sizeof(vec2))) {
                    found++;
                    k = (unsigned int)j;
                    break;
                }
            }
            if (!found) array_push(&temp_uvs, &v->uv);
            array_push(&u_index, &k);
        }

        if (mesh->type != OBJ_V) {
            unsigned k = temp_normals.size, found = 0;
            for (unsigned int j = 0; j < temp_normals.size; j++) {
                vec3 norm = *(vec3*)array_index(&temp_normals, j);
                if (!memcmp(&v->normal, &norm, sizeof(vec3))) {
                    found++;
                    k = (unsigned int)j;
                    break;
                }
            }
            if (!found) array_push(&temp_normals, &v->normal);
            array_push(&n_index, &k);
        }
    }

    wavefront_file_write(path, mesh->indices.size / 3, &temp_vertices, 
                &temp_uvs, &temp_normals, &v_index, &u_index, &n_index);
}

void vmesh_write_file_quick(vmesh_t* mesh, const char* path) 
{
    if (!mesh) return;
    
    printf("Writing obj file '%s'\n", path);

    FILE* file = fopen(path, "w");
    if (!file) {
        printf("Could not open '%s' file.\n", path);
        exit(-1);
    }

    for (unsigned int i = 0; i < mesh->vertices.size; i++) {
        vertex_t v = *(vertex_t*)array_index(&mesh->vertices, i);
        fprintf(file, "v %f %f %f\n", v.position.x, v.position.y, v.position.z);
    }

    if (mesh->type == OBJ_VTN) {
        for (unsigned int i = 0; i < mesh->vertices.size; i++) {
            vertex_t v = *(vertex_t*)array_index(&mesh->vertices, i);
            fprintf(file, "vt %f %f\n", v.uv.x, v.uv.y);
        }
    }

    if (mesh->type == OBJ_VTN || mesh->type == OBJ_VN) {
        for (unsigned int i = 0; i < mesh->vertices.size; i++) {
            vertex_t v = *(vertex_t*)array_index(&mesh->vertices, i);
            fprintf(file, "vn %f %f %f\n", v.normal.x, v.normal.y, v.normal.z);
        }
    }

    const unsigned int size = mesh->indices.size / 3;
    if (mesh->type == OBJ_VTN) {
        for (unsigned int i = 0; i < size; i++) {
            fprintf(file, "f");
            for (unsigned int j = 0; j < 3; j++) {
                unsigned int k = *(unsigned int*)array_index(&mesh->indices, i * 3 + j);
                k ++;
                fprintf(file, " %u/%u/%u", k, k, k);
            }
            fprintf(file, "\n");
        }
    }
    else if (mesh->type == OBJ_VN) {
        for (unsigned int i = 0; i < size; i++) {
            fprintf(file, "f");
            for (unsigned int j = 0; j < 3; j++) {
                unsigned int k = *(unsigned int*)array_index(&mesh->indices, i * 3 + j);
                k ++;
                fprintf(file, " %u//%u", k, k);
            }
            fprintf(file, "\n");
        }
    } 
    else {
        for (unsigned int i = 0; i < size; i++) {
            fprintf(file, "f");
            for (unsigned int j = 0; j < 3; j++) {
                unsigned int k = *(unsigned int*)array_index(&mesh->indices, i * 3 + j);
                k ++;
                fprintf(file, " %u", k);
            }
            fprintf(file, "\n");
        }
    }
    fclose(file);
    printf("Succesfully writed '%s' file\n", path);
}