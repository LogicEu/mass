#include <mass.h>
#include <stdlib.h>

/* --- Mesh of 3D Vectors --- */

mesh_t mesh_create(void)
{
    mesh_t mesh;

    mesh.vertices = array_create(sizeof(vec3));
    mesh.normals = array_create(sizeof(vec3));
    mesh.uvs = array_create(sizeof(vec2));

    return mesh;
}

mesh_t mesh_load(const char* path)
{
    imesh_t m = imesh_load(path);
    mesh_t mesh = imesh_to_mesh(&m);
    imesh_free(&m);
    return mesh;
}

mesh_t mesh_shape_quad(const float x, const float y)
{   
    mesh_t mesh = mesh_create();
    array_resize(&mesh.vertices, 6);
    mesh_push_quad_z(&mesh, x, y);
    return mesh;
}

mesh_t mesh_shape_plane(const size_t width, const size_t height)
{   
    mesh_t mesh = mesh_create();
    array_resize(&mesh.vertices, width * height * 6);

    for (size_t x = 0; x < width; x++) {
        for (size_t z = 0; z < height; z++) {
            mesh_push_quad_z(&mesh, (float)(int)x, (float)(int)z);
        }
    }

    return mesh;
}

mesh_t mesh_shape_cube(const size_t size)
{
    mesh_t mesh = mesh_create();
    array_resize(&mesh.vertices, 36 * size * size);

    mesh_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, size, 0.0f), vec3_new(1.0f, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f)); //Up
    mesh_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f), vec3_new(1.0f, 0.0f, 0.0f)); //Down
    mesh_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(1.0f, 0.0f, 0.0f), vec3_new(0.0f, 1.0f, 0.0f)); //Front
    mesh_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, size), vec3_new(0.0f, 1.0f, 0.0f), vec3_new(1.0f, 0.0f, 0.0f)); //Back
    mesh_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, 1.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f)); //Right
    mesh_push_plane_xyz(&mesh, size, size, vec3_new(size, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f), vec3_new(0.0f, 1.0f, 0.0f)); //Left

    mesh_move(&mesh, vec3_uni(-(float)size * 0.5f));

    return mesh;
}

mesh_t mesh_shape_hex(const vec3 size)
{
    mesh_t mesh = mesh_create();
    array_resize(&mesh.vertices, 36);

    mesh_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, size.y, 0.0f), vec3_new(size.x, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, size.z)); //Up
    mesh_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, size.z), vec3_new(size.x, 0.0f, 0.0f)); //Down
    mesh_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(size.x, 0.0f, 0.0f), vec3_new(0.0f, size.y, 0.0f)); //Front
    mesh_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, size.z), vec3_new(0.0f, size.y, 0.0f), vec3_new(size.x, 0.0f, 0.0f)); //Back
    mesh_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, size.y, 0.0f), vec3_new(0.0f, 0.0f, size.z)); //Right
    mesh_push_plane_xyz(&mesh, 1, 1, vec3_new(size.x, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, size.z), vec3_new(0.0f, size.y, 0.0f)); //Left

    mesh_move(&mesh, vec3_mult(size, -0.5));

    return mesh;
}

mesh_t mesh_shape_sphere(const size_t size)
{
    mesh_t mesh = mesh_shape_cube(size);
    mesh_normalize(&mesh);
    mesh_normals_get_face(&mesh);
    return mesh;
}

void mesh_save(const mesh_t* mesh, const char* path)
{
    imesh_t m = mesh_to_imesh(mesh);
    imesh_save(&m, path);
    imesh_free(&m);
}

void mesh_save_quick(const mesh_t* mesh, const char* path)
{
    return mesh_save_wavefront(mesh, path);
}

void mesh_free(mesh_t* mesh)
{
    array_free(&mesh->vertices);
    array_free(&mesh->normals);
    array_free(&mesh->uvs);
}