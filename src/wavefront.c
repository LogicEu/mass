#include <mass.h>
#include <stdio.h>
#include <string.h>

/* --- Wavefront OBJ File Format I/O --- */

iMesh3D imesh3D_load_wavefront(const char* path)
{
    static const char* symbols = "/\n ";
    
    iMesh3D mesh = imesh3D_create();
    FILE* file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "libmass could not open file '%s'.\n", path);
        return mesh;
    }

    size_t line_count = 0;
    char line[BUFSIZ];

    while ((fgets(line, BUFSIZ, file))) {

        ++line_count;

        char* token = strtok(line, symbols);
        if (!token || *token == '#') {
            continue;
        }
        
        if (!strcmp("vt", token)) {
            vec2 vt;
            float *f = (float*)&vt;
            const size_t size = sizeof(vec2) / sizeof(float);
            for (size_t i = 0; i < size; ++i) {
                if (!(token = strtok(NULL, symbols))) {
                    fprintf(stderr, "libmass detected a problem in the .obj file '%s' at line %zu.\n", path, line_count);
                    break;
                }
                sscanf(token, "%f", f++);
            }
            table_push_data(&mesh.uvs, &vt);
        }
        else if (!strcmp("vn", token)) {
            vec3 vn;
            float *f = (float*)&vn;
            const size_t size = sizeof(vec3) / sizeof(float);
            for (size_t i = 0; i < size; ++i) {
                if (!(token = strtok(NULL, symbols))) {
                    fprintf(stderr, "libmass detected a problem in the .obj file '%s' at line %zu.\n", path, line_count);
                    break;
                }
                sscanf(token, "%f", f++);
            }
            table_push_data(&mesh.normals, &vn);
        }
        else if (!strcmp("v", token)) {
            vec3 v;
            float *f = (float*)&v;
            const size_t size = sizeof(vec3) / sizeof(float);
            for (size_t i = 0; i < size; ++i) {
                if (!(token = strtok(NULL, symbols))) {
                    fprintf(stderr, "libmass detected a problem in the .obj file '%s' at line %zu.\n", path, line_count);
                    break;
                }
                sscanf(token, "%f", f++);
            }
            table_push_data(&mesh.vertices, &v);
        }
        else if (!strcmp("f", token)) {

            size_t indices[9], count = 0;
            for (size_t i = 0; i < 9; ++i) {
                if (!(token = strtok(NULL, symbols))) {
                    break;
                }
                sscanf(token, "%zu", &indices[count++]);
            }

            if (count == 9) {
                for (size_t i = 0; i < 3; ++i) {
                    table_push_index(&mesh.vertices, indices[i * 3 + 0] - 1);
                    table_push_index(&mesh.uvs, indices[i * 3 + 1] - 1);
                    table_push_index(&mesh.normals, indices[i * 3 + 2] - 1);
                }
            } 
            else if (count == 6) {
                for (size_t i = 0; i < 3; ++i) {
                    table_push_index(&mesh.vertices, indices[i * 2 + 0] - 1);
                    table_push_index(&mesh.normals, indices[i * 2 + 1] - 1);
                }
            }
            else if (count == 3) {
                for (size_t i = 0; i < 3; ++i) {
                    table_push_index(&mesh.vertices, indices[i] - 1);
                }
            } 
            else {
                fprintf(stderr, "libmass won't recognize format of file '%s'.\n", path);
                break;
            }
        }
    }
    
    fclose(file);
    return mesh;
}

void imesh3D_save_wavefront(const iMesh3D* restrict mesh, const char* restrict path)
{
    FILE* file = fopen(path, "w");
    if (!file) {
        fprintf(stderr, "libmass could not write file '%s'.\n", path);
        return;
    }

    fprintf(file, "# mass 3D model\n");

    vec3* v = mesh->vertices.data;
    const size_t vsize =  mesh->vertices.size;
    for (size_t i = 0; i < vsize; ++i) {
        fprintf(file, "v %f %f %f\n", v[i].x, v[i].y, v[i].z);
    }

    vec3* n = mesh->normals.data;
    const size_t nsize = mesh->normals.size;
    for (size_t i = 0; i < nsize; ++i) {
        fprintf(file, "vn %f %f %f\n", n[i].x, n[i].y, n[i].z);
    }

    vec2* uv = mesh->uvs.data;
    const size_t uvsize = mesh->uvs.size;
    for (size_t i = 0; i < uvsize; ++i) {
        fprintf(file, "vt %f %f\n", uv[i].x, uv[i].y);
    }

    size_t* vindex = table_indices(&mesh->vertices);
    size_t* nindex = table_indices(&mesh->normals);
    size_t* uvindex = table_indices(&mesh->uvs);

    const size_t size = table_indices_size(&mesh->vertices) / 3;

    for (size_t i = 0; i < size; ++i) {
        fprintf(file, "f");
        for (size_t j = i * 3; j < i * 3 + 3; ++j) {
            if (nsize && uvsize) {
                fprintf(file, " %zu/%zu/%zu", vindex[j] + 1, uvindex[j] + 1, nindex[j] + 1);
            }
            else if (nsize) {
                fprintf(file, " %zu//%zu", vindex[j] + 1, nindex[j] + 1);
            }
            else {
                fprintf(file, " %zu", vindex[j] + 1);
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void mesh3D_save_wavefront(const Mesh3D* restrict mesh, const char* restrict path)
{
    FILE* file = fopen(path, "w");
    if (!file) {
        fprintf(stderr, "libmass could not write file '%s'.\n", path);
        return;
    }

    fprintf(file, "# mass 3D model\n");

    vec3* v = mesh->vertices.data;
    const size_t vsize = mesh->vertices.size;
    for (size_t i = 0; i < vsize; ++i) {
        fprintf(file, "v %f %f %f\n", v[i].x, v[i].y, v[i].z);
    }

    vec3* n = mesh->normals.data;
    const size_t nsize = mesh->normals.size;
    for (size_t i = 0; i < nsize; i++) {
        fprintf(file, "vn %f %f %f\n", n[i].x, n[i].y, n[i].z);
    }

    vec2* uv = mesh->uvs.data;
    const size_t uvsize = mesh->uvs.size;
    for (size_t i = 0; i < uvsize; i++) {
        fprintf(file, "vt %f %f\n", uv[i].x, uv[i].y);
    }

    const size_t size = mesh->vertices.size / 3;
    
     for (size_t i = 0; i < size; ++i) {
        fprintf(file, "f");
        for (size_t j = i * 3; j < i * 3 + 3; ++j) {
            const size_t n = j + 1;
            if (nsize && uvsize) {
                fprintf(file, " %zu/%zu/%zu", n, n, n);
            }
            else if (nsize) {
                fprintf(file, " %zu//%zu", n, n);
            }
            else {
                fprintf(file, " %zu", n);
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}
