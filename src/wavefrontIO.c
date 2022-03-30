#include <mass.h>
#include <stdio.h>
#include <string.h>

/*
------------------------------------
 -> Wavefront OBJ File Format I/O <- 
------------------------------------
*/

void wavefront_file_load(const char* path, obj_flag* type, 
    array_t* v_index, array_t* u_index, array_t* n_index,
    array_t* temp_positions, array_t* temp_uvs, array_t* temp_normals)
{
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Could not open file '%s'\n", path);
        return;
    }

    printf("Reading '%s' file.\n", path);
    while (1) {
        char line_header[128];
        int res = fscanf(file, "%s", line_header);
        if (res == EOF) break;
        if (!strcmp("#", line_header)) continue;
        else if (!strcmp("vt", line_header)) {
            vec2 v;
            fscanf(file, "%f %f", &v.x, &v.y);
            array_push(temp_uvs, &v);
            *type = OBJ_VTN;
        }
        else if (!strcmp("vn", line_header)) {
            vec3 v;
            fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);
            array_push(temp_normals, &v);
            if (*type == OBJ_V) *type = OBJ_VN;
        }
        else if (!strcmp("v", line_header)) {
            vec3 v;
            fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);
            array_push(temp_positions, &v);
        }
        else if (!strcmp("f", line_header)) {
            if (*type == OBJ_VTN) {
                unsigned int vertex_index[3], uv_index[3], normal_index[3];
                int matches = fscanf(file, "%u/%u/%u %u/%u/%u %u/%u/%u\n", &vertex_index[0], &uv_index[0], &normal_index[0], 
                                                                            &vertex_index[1], &uv_index[1], &normal_index[1], 
                                                                            &vertex_index[2], &uv_index[2], &normal_index[2]);
                if (matches == 9) {
                    array_push(v_index, &vertex_index[0]);
                    array_push(v_index, &vertex_index[1]);
                    array_push(v_index, &vertex_index[2]);
                    array_push(u_index, &uv_index[0]);
                    array_push(u_index, &uv_index[1]);
                    array_push(u_index, &uv_index[2]);
                    array_push(n_index, &normal_index[0]);
                    array_push(n_index, &normal_index[1]);
                    array_push(n_index, &normal_index[2]);
                } else {
                    printf("Our wavefront loader won't accept this format.\n");
                    return;
                }
            } else if (*type == OBJ_VN) {
                unsigned int vertex_index[3], normal_index[3];
                int matches = fscanf(file, "%u//%u %u//%u %u//%u\n", &vertex_index[0], &normal_index[0], 
                                                                    &vertex_index[1], &normal_index[1],
                                                                    &vertex_index[2], &normal_index[2]);
                if (matches == 6) {
                    array_push(v_index, &vertex_index[0]);
                    array_push(v_index, &vertex_index[1]);
                    array_push(v_index, &vertex_index[2]);
                    array_push(n_index, &normal_index[0]);
                    array_push(n_index, &normal_index[1]);
                    array_push(n_index, &normal_index[2]);
                }
            } else {
                unsigned int vertex_index[3];
                int matches = fscanf(file, "%u %u %u/\n", &vertex_index[0], &vertex_index[1], &vertex_index[2]);
                if (matches == 3) {
                    array_push(v_index, &vertex_index[0]);
                    array_push(v_index, &vertex_index[1]);
                    array_push(v_index, &vertex_index[2]);
                } else {
                    printf("Our wavefront loader won't accept this format.\n");
                    return;
                }
            }
        }
    }
    fclose(file);
}

void wavefront_file_write(const char* path, const unsigned int size,
    array_t* temp_vertices, array_t* temp_uvs, array_t* temp_normals,
    array_t* v_index, array_t* u_index, array_t* n_index)
{
    FILE* file = fopen(path, "w");
    if (!file) {
        printf("Could not open file '%s'.\n", path);
        return;
    }
    printf("Writing obj file '%s'.\n", path);

    if (temp_vertices != NULL) {
        for (unsigned int i = 0; i < temp_vertices->size; i++) {
            vec3 v = *(vec3*)array_index(temp_vertices, i);
            fprintf(file, "v %f %f %f\n", v.x, v.y, v.z);
        }
    }
    if (temp_uvs != NULL) {
        for (unsigned int i = 0; i < temp_uvs->size; i++) {
            vec2 v = *(vec2*)array_index(temp_uvs, i);
            fprintf(file, "vt %f %f\n", v.x, v.y);
        }
    }
    if (temp_normals != NULL) {
        for (unsigned int i = 0; i < temp_normals->size; i++) {
            vec3 v = *(vec3*)array_index(temp_normals, i);
            fprintf(file, "vn %f %f %f\n", v.x, v.y, v.z);
        }
    }
    if (n_index != NULL && u_index != NULL) {
        for (unsigned int i = 0; i < size; i++) {
            fprintf(file, "f");
            for (unsigned int j = 0; j < 3; j++) {
                unsigned int v = *(unsigned int*)array_index(v_index, i * 3 + j);
                unsigned int vt = *(unsigned int*)array_index(u_index, i * 3 + j);
                unsigned int vn = *(unsigned int*)array_index(n_index, i * 3 + j);
                fprintf(file, " %u/%u/%u", v + 1, vt + 1, vn + 1);
            }
            fprintf(file, "\n");
        }
    }
    else if (n_index != NULL) {
        for (unsigned int i = 0; i < size; i++) {
            fprintf(file, "f");
            for (unsigned int j = 0; j < 3; j++) {
                unsigned int v = *(unsigned int*)array_index(v_index, i * 3 + j);
                unsigned int vn = *(unsigned int*)array_index(n_index, i * 3 + j);
                fprintf(file, " %u//%u", v + 1, vn + 1);
            }
            fprintf(file, "\n");
        }
    } else {
        for (unsigned int i = 0; i < size; i++) {
            fprintf(file, "f");
            for (unsigned int j = 0; j < 3; j++) {
                unsigned int one = *(unsigned int*)array_index(v_index, i * 3 + j);
                fprintf(file, " %u", one + 1);
            }
            fprintf(file, "\n");
        }
    }
    fclose(file);
    printf("Succesfully writed '%s' file.\n", path);

    array_free(v_index);
    array_free(u_index);
    array_free(n_index);
    array_free(temp_vertices);
    array_free(temp_uvs);
    array_free(temp_normals);
}