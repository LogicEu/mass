#include <mass.h>

/* --- 3D Mesh --- */

Mesh3D mesh3D_create(void)
{
    Mesh3D mesh;

    mesh.vertices = vector_create(sizeof(vec3));
    mesh.normals = vector_create(sizeof(vec3));
    mesh.uvs = vector_create(sizeof(vec2));

    return mesh;
}

Mesh3D mesh3D_load(const char* path)
{
    iMesh3D m = imesh3D_load(path);
    Mesh3D mesh = imesh3D_to_mesh3D(&m);
    imesh3D_free(&m);
    return mesh;
}

Mesh3D mesh3D_shape_quad(const vec3 position)
{   
    Mesh3D mesh = mesh3D_create();
    vector_resize(&mesh.vertices, 6);
    mesh3D_push_quad_z(&mesh, position);
    return mesh;
}

Mesh3D mesh3D_shape_plane(const size_t width, const size_t height)
{   
    Mesh3D mesh = mesh3D_create();
    vector_resize(&mesh.vertices, width * height * 6);

    for (size_t x = 0; x < width; x++) {
        for (size_t z = 0; z < height; z++) {
            mesh3D_push_quad_z(&mesh, vec3_new((float)(int)x, 0.0, (float)(int)z));
        }
    }

    return mesh;
}

Mesh3D mesh3D_shape_cube(const size_t size)
{
    Mesh3D mesh = mesh3D_create();
    vector_resize(&mesh.vertices, 36 * size * size);

    mesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, size, 0.0f), vec3_new(1.0f, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f)); //Up
    mesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f), vec3_new(1.0f, 0.0f, 0.0f)); //Down
    mesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(1.0f, 0.0f, 0.0f), vec3_new(0.0f, 1.0f, 0.0f)); //Front
    mesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, size), vec3_new(0.0f, 1.0f, 0.0f), vec3_new(1.0f, 0.0f, 0.0f)); //Back
    mesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, 1.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f)); //Right
    mesh3D_push_plane_xyz(&mesh, size, size, vec3_new(size, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f), vec3_new(0.0f, 1.0f, 0.0f)); //Left

    mesh3D_move(&mesh, vec3_uni(-(float)size * 0.5f));

    return mesh;
}

Mesh3D mesh3D_shape_hex(const vec3 size)
{
    Mesh3D mesh = mesh3D_create();
    vector_resize(&mesh.vertices, 36);

    mesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, size.y, 0.0f), vec3_new(size.x, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, size.z)); //Up
    mesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, size.z), vec3_new(size.x, 0.0f, 0.0f)); //Down
    mesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(size.x, 0.0f, 0.0f), vec3_new(0.0f, size.y, 0.0f)); //Front
    mesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, size.z), vec3_new(0.0f, size.y, 0.0f), vec3_new(size.x, 0.0f, 0.0f)); //Back
    mesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, size.y, 0.0f), vec3_new(0.0f, 0.0f, size.z)); //Right
    mesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(size.x, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, size.z), vec3_new(0.0f, size.y, 0.0f)); //Left

    mesh3D_move(&mesh, vec3_mult(size, -0.5));

    return mesh;
}

Mesh3D mesh3D_shape_sphere(const size_t size)
{
    Mesh3D mesh = mesh3D_shape_cube(size);
    mesh3D_normalize(&mesh);
    mesh3D_normalize_faces(&mesh);
    return mesh;
}

void mesh3D_save(const Mesh3D* mesh, const char* path)
{
    iMesh3D m = mesh3D_to_imesh3D(mesh);
    imesh3D_save(&m, path);
    imesh3D_free(&m);
}

void mesh3D_save_quick(const Mesh3D* mesh, const char* path)
{
    mesh3D_save_wavefront(mesh, path);
}

void mesh3D_free(Mesh3D* mesh)
{
    vector_free(&mesh->vertices);
    vector_free(&mesh->normals);
    vector_free(&mesh->uvs);
}

/* --- Procedural Opertations --- */

void mesh3D_push_quad_z(Mesh3D* mesh, const vec3 position)
{
    const vec2* quad = mass_shape_quad();
    for (size_t i = 0; i < 6; i++) {
        vec3 pos = {
            quad[i].x + position.x, 
            position.y, 
            quad[i].y + position.z
        };

        vector_push(&mesh->vertices, &pos);
    }
}

void mesh3D_push_quad_xyz(Mesh3D* mesh, const vec3 position, const vec3 xvec, const vec3 yvec)
{
    const vec2* quad = mass_shape_quad();
    for (size_t i = 0; i < 6; i++) {
        vec3 pos = {
            quad[i].x * xvec.x + quad[i].y * yvec.x + position.x, 
            quad[i].x * xvec.y + quad[i].y * yvec.y + position.y, 
            quad[i].x * xvec.z + quad[i].y * yvec.z + position.z
        };
        
        vector_push(&mesh->vertices, &pos);
    }
}

void mesh3D_push_plane_z(Mesh3D* mesh, const size_t xsize, const size_t ysize)
{
    for (size_t x = 0; x < xsize; x++) {
        for (size_t y = 0; y < ysize; y++) {
            mesh3D_push_quad_z(mesh, vec3_new((float)(int)x, 0.0, (float)(int)y));
        }
    }
}

void mesh3D_push_plane_xyz(Mesh3D* mesh, const size_t xsize, const size_t ysize, const vec3 position, const vec3 xvec, const vec3 yvec)
{
    for (size_t x = 0; x < xsize; x++) {
        for (size_t y = 0; y < ysize; y++) {
            float fx = (float)(int)x, fy = (float)(int)y;
            vec3 offset = {
                fx * xvec.x + fy * yvec.x + position.x, 
                fx * xvec.y + fy * yvec.y + position.y, 
                fx * xvec.z + fy * yvec.z + position.z
            };

            mesh3D_push_quad_xyz(mesh, offset, xvec, yvec);
        }
    }
}

void mesh3D_normalize_faces(Mesh3D* mesh)
{
    vector_clear(&mesh->normals);

    vec3* pos = mesh->vertices.data;
    
    const size_t triangles = mesh->vertices.size / 3;
    for (size_t i = 0; i < triangles; i++) {
        
        vec3 vu = _vec3_sub(pos[i * 3 + 1], pos[i * 3]);
        vec3 vv = _vec3_sub(pos[i * 3 + 2], pos[i * 3]);
        vec3 n = _vec3_cross(vu, vv);
        n = vec3_norm(n);
        
        vector_push(&mesh->normals, &n);
        vector_push(&mesh->normals, &n);
        vector_push(&mesh->normals, &n);
    }
}

void mesh3D_merge(Mesh3D* restrict m1, Mesh3D* restrict m2)
{
    const size_t vsize = m2->vertices.size;
    vec3* v = m2->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        vector_push(&m1->vertices, v + i);
    }

    const size_t nsize = m2->normals.size;
    vec3* n = m2->vertices.data;
    for (size_t i = 0; i < nsize; i++) {
        vector_push(&m1->normals, n + i);
    }

    const size_t uvsize = m2->uvs.size;
    vec2* uv = m2->vertices.data;
    for (size_t i = 0; i < uvsize; i++) {
        vector_push(&m1->vertices, uv + i);
    }

    mesh3D_free(m2);
}

void mesh3D_scale(Mesh3D* mesh, const float f)
{
    const size_t vsize = mesh->vertices.size;
    vec3* v = mesh->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec3_mult(v[i], f);
    }
}

void mesh3D_move(Mesh3D* mesh, const vec3 add)
{
    const size_t vsize = mesh->vertices.size;
    vec3* v = mesh->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec3_add(v[i], add);
    }
}

void mesh3D_normalize(Mesh3D* mesh)
{
    const size_t vsize = mesh->vertices.size;
    vec3* v = mesh->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec3_norm(v[i]);
    }
}

Mesh3D imesh3D_to_mesh3D(const iMesh3D* mesh)
{
    Mesh3D m;
    size_t vertcount, uvcount, normcount;
    void* vertdata = table_decompress(&mesh->vertices, &vertcount);
    void* normdata = table_decompress(&mesh->normals, &normcount);
    void* uvdata = table_decompress(&mesh->uvs, &uvcount);
    m.vertices = vector_wrap(vertdata, sizeof(vec3), vertcount);
    m.normals = vector_wrap(normdata, sizeof(vec3), normcount);
    m.uvs = vector_wrap(uvdata, sizeof(vec2), uvcount);
    return m;
}
