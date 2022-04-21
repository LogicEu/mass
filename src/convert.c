#include <mass.h>
#include <stdlib.h>
#include <string.h>

/* --- Mesh Type Convertions --- */

mesh_t imesh_to_mesh(const imesh_t* mesh)
{
    mesh_t m;
    m.vertices = table_decompress(&mesh->vertices);
    m.normals = table_decompress(&mesh->normals);
    m.uvs = table_decompress(&mesh->uvs);
    return m;
}

imesh_t mesh_to_imesh(const mesh_t* mesh)
{
    imesh_t m;
    m.vertices = table_compress(&mesh->vertices);
    m.normals = table_compress(&mesh->normals);
    m.uvs = table_compress(&mesh->uvs);
    return m;
}