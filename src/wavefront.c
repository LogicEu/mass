#include <mass.h>
#include <stdio.h>
#include <string.h>

/* --- Wavefront OBJ File Format I/O --- */

imesh_t imesh_load_wavefront(const char* path)
{
    imesh_t mesh = imesh_create();

    FILE* file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "libmass not open file '%s'.\n", path);
        return mesh;
    }

    char line_header[128];

    while (fscanf(file, "%s", line_header) != EOF) {

        if (!strcmp("#", line_header)) {
            continue;
        }
        
        if (!strcmp("vt", line_header)) {
            vec2 v;
            fscanf(file, "%f %f", &v.x, &v.y);
            array_push(&mesh.uvs.values, &v);
        }
        else if (!strcmp("vn", line_header)) {
            vec3 v;
            fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);
            array_push(&mesh.normals.values, &v);
        }
        else if (!strcmp("v", line_header)) {
            vec3 v;
            fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);
            array_push(&mesh.vertices.values, &v);
        }
        else if (!strcmp("f", line_header)) {
            
            if (mesh.uvs.values.size) {
                
                size_t vindices[3]; 
                size_t uvindices[3];
                size_t nindices[3];

                int matches = fscanf(
                    file, 
                    "%zu/%zu/%zu %zu/%zu/%zu %zu/%zu/%zu\n", 
                    &vindices[0], 
                    &uvindices[0], 
                    &nindices[0], 
                    &vindices[1], 
                    &uvindices[1], 
                    &nindices[1], 
                    &vindices[2], 
                    &uvindices[2], 
                    &nindices[2]
                );

                if (matches == 9) {
                    for (size_t i = 0; i < 3; ++i) {
                        --vindices[i];
                        --uvindices[i];
                        --nindices[i];
                        
                        array_push(&mesh.vertices.indices, &vindices[i]);
                        array_push(&mesh.uvs.indices, &uvindices[i]);
                        array_push(&mesh.normals.indices, &nindices[i]);
                    }
                } 
                
                else {
                    fprintf(stderr, "libmass won't recognize this format. I'm sorry.\n");
                    break;
                }
            } 
            
            else if (mesh.normals.values.size) {

                size_t vindices[3]; 
                size_t nindices[3];

                int matches = fscanf(
                    file, 
                    "%zu//%zu %zu//%zu %zu//%zu\n", 
                    &vindices[0], 
                    &nindices[0], 
                    &vindices[1], 
                    &nindices[1],
                    &vindices[2], 
                    &nindices[2]
                );

                if (matches == 6) {
                    for (size_t i = 0; i < 3; ++i) {
                        --vindices[i];
                        --nindices[i];
                        array_push(&mesh.vertices.indices, &vindices[i]);
                        array_push(&mesh.normals.indices, &nindices[i]);
                    }
                }

                else {
                    fprintf(stderr, "libmass won't recognize this format. I'm sorry.\n");
                    break;
                }
            } 
            
            else {
                
                size_t vindices[3];
                
                int matches = fscanf(
                    file, 
                    "%zu %zu %zu/\n", 
                    &vindices[0], 
                    &vindices[1], 
                    &vindices[2]
                );

                if (matches == 3) {
                    for (size_t i = 0; i < 3; ++i) {
                        --vindices[i];
                        array_push(&mesh.vertices.indices, &vindices[i]);
                    }
                } 
                
                else {
                    fprintf(stderr, "libmass won't recognize this format. I'm sorry.\n");
                    break;
                }
            }
        }
    }
    
    fclose(file);
    return mesh;
}

void imesh_save_wavefront(const imesh_t* restrict mesh, const char* restrict path)
{
    FILE* file = fopen(path, "w");
    if (!file) {
        fprintf(stderr, "libmass could not write file '%s'.\n", path);
        return;
    }

    fprintf(file, "# mass 3D model");

    vec3* v = mesh->vertices.values.data;
    const size_t vsize =  mesh->vertices.values.size;
    for (size_t i = 0; i < vsize; ++i) {
        fprintf(file, "v %f %f %f\n", v[i].x, v[i].y, v[i].z);
    }

    vec3* n = mesh->normals.values.data;
    const size_t nsize = mesh->normals.values.size;
    for (size_t i = 0; i < nsize; ++i) {
        fprintf(file, "vn %f %f %f\n", n[i].x, n[i].y, n[i].z);
    }

    vec2* uv = mesh->uvs.values.data;
    const size_t uvsize = mesh->uvs.values.size;
    for (size_t i = 0; i < uvsize; ++i) {
        fprintf(file, "vt %f %f\n", uv[i].x, uv[i].y);
    }

    size_t* vindex = mesh->vertices.indices.data;
    size_t* nindex = mesh->normals.indices.data;
    size_t* uvindex = mesh->uvs.indices.data;

    const size_t size = mesh->vertices.indices.size / 3;

    if (nsize && uvsize) {
        for (size_t i = 0; i < size; ++i) {
            fprintf(file, "f");
            for (size_t j = i * 3; j < i * 3 + 3; ++j) {
                fprintf(file, " %zu/%zu/%zu", vindex[j] + 1, uvindex[j] + 1, nindex[j] + 1);
            }
            fprintf(file, "\n");
        }
    }
    else if (nsize) {
        for (size_t i = 0; i < size; ++i) {
            fprintf(file, "f");
            for (size_t j = i * 3; j < i * 3 + 3; ++j) {
                fprintf(file, " %zu//%zu", vindex[j] + 1, nindex[j] + 1);
            }
            fprintf(file, "\n");
        }
    }
    else {
        for (size_t i = 0; i < size; ++i) {
            fprintf(file, "f");
            for (size_t j = i * 3; j < i * 3 + 3; ++j) {
                fprintf(file, " %zu", vindex[j] + 1);
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

void mesh_save_wavefront(const mesh_t* restrict mesh, const char* restrict path)
{
    FILE* file = fopen(path, "w");
    if (!file) {
        fprintf(stderr, "libmass could not write file '%s'.\n", path);
        return;
    }

    fprintf(file, "# mass 3D model");

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
    
    if (nsize && uvsize) {
        for (size_t i = 0; i < size; i++) {
            fprintf(file, "f");
            for (size_t j = i * 3; j < i * 3 + 3; j++) {
                fprintf(file, " %zu/%zu/%zu", j + 1, j + 1, j + 1);
            }
            fprintf(file, "\n");
        }
    }
    else if (nsize) {
        for (size_t i = 0; i < size; i++) {
            fprintf(file, "f");
            for (size_t j = i * 3; j < i * 3 + 3; j++) {
                fprintf(file, " %zu//%zu", j + 1, j + 1);
            }
            fprintf(file, "\n");
        }
    } else {
        for (size_t i = 0; i < size; i++) {
            fprintf(file, "f");
            for (size_t j = i * 3; j < i * 3 + 3; j++) {
                fprintf(file, " %zu", j + 1);
            }
            fprintf(file, "\n");
        }
    }
    fclose(file);
}