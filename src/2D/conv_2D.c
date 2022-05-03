#include <mass.h>

/* --- 2D Mesh Type Convertions --- */

Mesh2D imesh2D_to_mesh2D(const iMesh2D* mesh)
{
    Mesh2D m;
    m.vertices = table_decompress(&mesh->vertices);
    m.uvs = table_decompress(&mesh->uvs);
    return m;
}

iMesh2D mesh2D_to_imesh2D(const Mesh2D* mesh)
{
    iMesh2D m;
    m.vertices = table_compress(&mesh->vertices);
    m.uvs = table_compress(&mesh->uvs);
    return m;
}