#include <mass.h>

/* --- iMesh 3D --- */

imesh_t imesh_create(void)
{
    imesh_t mesh;

    mesh.vertices = table_create(sizeof(vec3));
    mesh.normals = table_create(sizeof(vec3));
    mesh.uvs = table_create(sizeof(vec2));

    return mesh;
}

imesh_t imesh_load(const char* path)
{
    return imesh_load_wavefront(path);
}

imesh_t imesh_shape_quad(const float x, const float y)
{   
    imesh_t mesh = imesh_create();
    array_resize(&mesh.vertices.indices, 6);
    array_resize(&mesh.vertices.values, 4);
    imesh_push_quad_z(&mesh, x, y);
    return mesh;
}

imesh_t imesh_shape_plane(const size_t width, const size_t height)
{   
    imesh_t mesh = imesh_create();
    array_resize(&mesh.vertices.indices, width * height * 6);

    for (size_t x = 0; x < width; x++) {
        for (size_t z = 0; z < height; z++) {
            imesh_push_quad_z(&mesh, (float)(int)x, (float)(int)z);
        }
    }

    return mesh;
}

imesh_t imesh_shape_cube(const size_t size)
{
    imesh_t mesh = imesh_create();
    array_resize(&mesh.vertices.indices, 36 * size * size);

    imesh_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, size, 0.0f), vec3_new(1.0f, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f)); //Up
    imesh_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f), vec3_new(1.0f, 0.0f, 0.0f)); //Down
    imesh_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(1.0f, 0.0f, 0.0f), vec3_new(0.0f, 1.0f, 0.0f)); //Front
    imesh_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, size), vec3_new(0.0f, 1.0f, 0.0f), vec3_new(1.0f, 0.0f, 0.0f)); //Back
    imesh_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, 1.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f)); //Right
    imesh_push_plane_xyz(&mesh, size, size, vec3_new(size, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f), vec3_new(0.0f, 1.0f, 0.0f)); //Left

    imesh_move(&mesh, vec3_uni(-(float)size * 0.5f));

    return mesh;
}

imesh_t imesh_shape_hex(const vec3 size)
{
    imesh_t mesh;
    array_resize(&mesh.vertices.indices, 36);
    array_resize(&mesh.vertices.values, 8);

    imesh_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, size.y, 0.0f), vec3_new(size.x, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, size.z)); //Up
    imesh_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, size.z), vec3_new(size.x, 0.0f, 0.0f)); //Down
    imesh_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(size.x, 0.0f, 0.0f), vec3_new(0.0f, size.y, 0.0f)); //Front
    imesh_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, size.z), vec3_new(0.0f, size.y, 0.0f), vec3_new(size.x, 0.0f, 0.0f)); //Back
    imesh_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, size.y, 0.0f), vec3_new(0.0f, 0.0f, size.z)); //Right
    imesh_push_plane_xyz(&mesh, 1, 1, vec3_new(size.x, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, size.z), vec3_new(0.0f, size.y, 0.0f)); //Left

    imesh_move(&mesh, vec3_mult(size, -0.5));

    return mesh;
}

imesh_t imesh_shape_sphere(const size_t size)
{
    imesh_t mesh = imesh_shape_cube(size);
    imesh_normalize(&mesh);
    imesh_normals_get_face(&mesh);
    return mesh;
}

void imesh_save(const imesh_t* mesh, const char* path)
{
    return imesh_save_wavefront(mesh, path);
}

void imesh_free(imesh_t* mesh)
{
    table_free(&mesh->vertices);
    table_free(&mesh->normals);
    table_free(&mesh->uvs);
}