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
    if (mesh == NULL) return;
    printf("Writing file '%s'\n", path);

    array_t* temp_vertices = NULL;
    array_t* temp_uvs = NULL;
    array_t* temp_normals = NULL;
    array_t* v_index = NULL;
    array_t* u_index = NULL;
    array_t* n_index = NULL;

    if (mesh->vertices != NULL) {
        array_map_indexed(mesh->vertices, &temp_vertices, &v_index);
    }
    if (mesh->uvs != NULL) {
        array_map_indexed(mesh->uvs, &temp_uvs, &u_index);
    }
    if (mesh->normals != NULL) {
        array_map_indexed(mesh->normals, &temp_normals, &n_index);
    }

    wavefront_file_write(path, v_index->used / 3, temp_vertices, 
                temp_uvs, temp_normals, v_index, u_index, n_index);
}

void mesh_write_file_quick(mesh_t* mesh, const char* path)
{
    if (mesh == NULL) return;
    printf("Writing file '%s'\n", path);

    FILE* file = fopen(path, "w");
    if (!file) {
        printf("Could not open '%s' file.\n", path);
        exit(-1);
    }
    if (mesh->vertices != NULL) {
        for (unsigned int i = 0; i < mesh->vertices->used; i++) {
            vec3 v = *(vec3*)array_index(mesh->vertices, i);
            fprintf(file, "v %f %f %f\n", v.x, v.y, v.z);
        }
    }
    if (mesh->uvs != NULL) {
        for (unsigned int i = 0; i < mesh->uvs->used; i++) {
            vec2 v = *(vec2*)array_index(mesh->uvs, i);
            fprintf(file, "vt %f %f\n", v.x, v.y);
        }
    }
    if (mesh->normals != NULL) {
        for (unsigned int i = 0; i < mesh->normals->used; i++) {
            vec3 v = *(vec3*)array_index(mesh->normals, i);
            fprintf(file, "vn %f %f %f\n", v.x, v.y, v.z);
        }
    }
    const unsigned int size = mesh->vertices->used / 3;
    if (mesh->normals != NULL && mesh->uvs != NULL) {
        for (unsigned int i = 0; i < size; i++) {
            fprintf(file, "f");
            for (unsigned int j = 0; j < 3; j++) {
                unsigned int k = i * 3 + j + 1;
                fprintf(file, " %u/%u/%u", k, k, k);
            }
            fprintf(file, "\n");
        }
    }
    else if (mesh->normals != NULL) {
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
    if (mesh == NULL) return;
    array_t* temp_vertices = array_new(mesh->vertices->used, sizeof(vec3));
    array_t* v_index = array_new(mesh->indices->used, mesh->indices->bytes);
    array_t* temp_uvs = NULL;
    array_t* u_index = NULL;
    array_t* temp_normals = NULL;
    array_t* n_index = NULL;

    if (mesh->type == OBJ_VTN) {
        temp_uvs = array_new(mesh->vertices->used, sizeof(vec2));
        u_index = array_new(mesh->indices->used, mesh->indices->bytes);
    }
    if (mesh->type != OBJ_V) {
        temp_normals = array_new(mesh->vertices->used, sizeof(vec3));
        n_index = array_new(mesh->indices->used, mesh->indices->bytes);
    }

    for (unsigned int i = 0; i < mesh->indices->used; i++) {
        unsigned int n = *(unsigned int*)array_index(mesh->indices, i);
        vertex_t* v = (vertex_t*)array_index(mesh->vertices, n);

        unsigned int k = temp_vertices->used, found = 0;
        for (unsigned int j = 0; j < temp_vertices->used; j++) {
            vec3 pos = *(vec3*)array_index(temp_vertices, j);
            if (!memcmp(&v->position, &pos, sizeof(vec3))) {
                found++;
                k = (unsigned int)j;
                break;
            }
        }
        if (!found) array_push(temp_vertices, &v->position);
        array_push(v_index, &k);

        if (mesh->type == OBJ_VTN) {
            unsigned k = temp_uvs->used, found = 0;
            for (unsigned int j = 0; j < temp_uvs->used; j++) {
                vec2 uv = *(vec2*)array_index(temp_uvs, j);
                if (!memcmp(&v->uv, &uv, sizeof(vec2))) {
                    found++;
                    k = (unsigned int)j;
                    break;
                }
            }
            if (!found) array_push(temp_uvs, &v->uv);
            array_push(u_index, &k);
        }

        if (mesh->type != OBJ_V) {
            unsigned k = temp_normals->used, found = 0;
            for (unsigned int j = 0; j < temp_normals->used; j++) {
                vec3 norm = *(vec3*)array_index(temp_normals, j);
                if (!memcmp(&v->normal, &norm, sizeof(vec3))) {
                    found++;
                    k = (unsigned int)j;
                    break;
                }
            }
            if (!found) array_push(temp_normals, &v->normal);
            array_push(n_index, &k);
        }
    }

    wavefront_file_write(path, mesh->indices->used / 3, temp_vertices, 
                temp_uvs, temp_normals, v_index, u_index, n_index);
}

void vmesh_write_file_quick(vmesh_t* mesh, const char* path) 
{
    if (mesh == NULL) return;
    
    printf("Writing obj file '%s'\n", path);

    FILE* file = fopen(path, "w");
    if (!file) {
        printf("Could not open '%s' file.\n", path);
        exit(-1);
    }

    for (unsigned int i = 0; i < mesh->vertices->used; i++) {
        vertex_t v = *(vertex_t*)array_index(mesh->vertices, i);
        fprintf(file, "v %f %f %f\n", v.position.x, v.position.y, v.position.z);
    }

    if (mesh->type == OBJ_VTN) {
        for (unsigned int i = 0; i < mesh->vertices->used; i++) {
            vertex_t v = *(vertex_t*)array_index(mesh->vertices, i);
            fprintf(file, "vt %f %f\n", v.uv.x, v.uv.y);
        }
    }

    if (mesh->type == OBJ_VTN || mesh->type == OBJ_VN) {
        for (unsigned int i = 0; i < mesh->vertices->used; i++) {
            vertex_t v = *(vertex_t*)array_index(mesh->vertices, i);
            fprintf(file, "vn %f %f %f\n", v.normal.x, v.normal.y, v.normal.z);
        }
    }

    const unsigned int size = mesh->indices->used / 3;
    if (mesh->type == OBJ_VTN) {
        for (unsigned int i = 0; i < size; i++) {
            fprintf(file, "f");
            for (unsigned int j = 0; j < 3; j++) {
                unsigned int k = *(unsigned int*)array_index(mesh->indices, i * 3 + j);
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
                unsigned int k = *(unsigned int*)array_index(mesh->indices, i * 3 + j);
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
                unsigned int k = *(unsigned int*)array_index(mesh->indices, i * 3 + j);
                k ++;
                fprintf(file, " %u", k);
            }
            fprintf(file, "\n");
        }
    }
    fclose(file);
    printf("Succesfully writed '%s' file\n", path);
}