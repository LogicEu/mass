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
            table_push_data(&mesh.uvs, &v);
        }
        else if (!strcmp("vn", line_header)) {
            vec3 v;
            fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);
            table_push_data(&mesh.normals, &v);
        }
        else if (!strcmp("v", line_header)) {
            vec3 v;
            fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);
            table_push_data(&mesh.vertices, &v);
        }
        else if (!strcmp("f", line_header)) {
            
            if (mesh.uvs.size) {
                
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
                        table_push_index(&mesh.vertices, vindices[i] - 1);
                        table_push_index(&mesh.uvs, uvindices[i] - 1);
                        table_push_index(&mesh.normals, nindices[i] - 1);
                    }
                } 
                
                else {
                    fprintf(stderr, "libmass won't recognize this format. I'm sorry.\n");
                    break;
                }
            } 
            
            else if (mesh.normals.size) {

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
                        table_push_index(&mesh.vertices, vindices[i] - 1);
                        table_push_index(&mesh.normals, nindices[i] - 1);
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
                        table_push_index(&mesh.vertices, vindices[i] - 1);
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

    size_t* vindex = mesh->vertices.indices;
    size_t* nindex = mesh->normals.indices;
    size_t* uvindex = mesh->uvs.indices;

    const size_t size = table_indices_size(&mesh->vertices) / 3;

    for (size_t i = 0; i < size; ++i) {
        fprintf(file, "f");
        for (size_t j = i * 3; j < i * 3 + 3; ++j) {
            if (nsize && uvsize) {
                fprintf(file, " %zu/%zu/%zu", vindex[j], uvindex[j], nindex[j]);
            }
            else if (nsize) {
                fprintf(file, " %zu//%zu", vindex[j], nindex[j]);
            }
            else {
                fprintf(file, " %zu", vindex[j]);
            }
        }
        fprintf(file, "\n");
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