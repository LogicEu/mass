#include <mass.h>

/* --- 3D Mesh Type Convertions --- */

Mesh3D imesh3D_to_mesh3D(const iMesh3D* mesh)
{
    Mesh3D m;
    m.vertices = table_decompress(&mesh->vertices);
    m.normals = table_decompress(&mesh->normals);
    m.uvs = table_decompress(&mesh->uvs);
    return m;
}

iMesh3D mesh3D_to_imesh3D(const Mesh3D* mesh)
{
    iMesh3D m;
    m.vertices = table_compress(&mesh->vertices);
    m.normals = table_compress(&mesh->normals);
    m.uvs = table_compress(&mesh->uvs);
    return m;
}