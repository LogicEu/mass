#include <mass.h>

/* --- Indexed 3D Mesh --- */

iMesh3D imesh3D_create(void)
{
    iMesh3D mesh;

    mesh.vertices = table_create(sizeof(vec3));
    mesh.normals = table_create(sizeof(vec3));
    mesh.uvs = table_create(sizeof(vec2));

    return mesh;
}

iMesh3D imesh3D_load(const char* path)
{
    return imesh3D_load_wavefront(path);
}

iMesh3D imesh3D_shape_quad(const vec3 position)
{   
    iMesh3D mesh = imesh3D_create();
    imesh3D_push_quad_z(&mesh, position);
    return mesh;
}

iMesh3D imesh3D_shape_plane(const size_t width, const size_t height)
{   
    iMesh3D mesh = imesh3D_create();

    for (size_t x = 0; x < width; x++) {
        for (size_t z = 0; z < height; z++) {
            imesh3D_push_quad_z(&mesh, vec3_new((float)(int)x, 0.0, (float)(int)z));
        }
    }

    return mesh;
}

iMesh3D imesh3D_shape_cube(const size_t size)
{
    iMesh3D mesh = imesh3D_create();

    imesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, size, 0.0f), vec3_new(1.0f, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f)); //Up
    imesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f), vec3_new(1.0f, 0.0f, 0.0f)); //Down
    imesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(1.0f, 0.0f, 0.0f), vec3_new(0.0f, 1.0f, 0.0f)); //Front
    imesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, size), vec3_new(0.0f, 1.0f, 0.0f), vec3_new(1.0f, 0.0f, 0.0f)); //Back
    imesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, 1.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f)); //Right
    imesh3D_push_plane_xyz(&mesh, size, size, vec3_new(size, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f), vec3_new(0.0f, 1.0f, 0.0f)); //Left

    imesh3D_move(&mesh, vec3_uni(-(float)size * 0.5f));

    return mesh;
}

iMesh3D imesh3D_shape_hex(const vec3 size)
{
    iMesh3D mesh;

    imesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, size.y, 0.0f), vec3_new(size.x, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, size.z)); //Up
    imesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, size.z), vec3_new(size.x, 0.0f, 0.0f)); //Down
    imesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(size.x, 0.0f, 0.0f), vec3_new(0.0f, size.y, 0.0f)); //Front
    imesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, size.z), vec3_new(0.0f, size.y, 0.0f), vec3_new(size.x, 0.0f, 0.0f)); //Back
    imesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, size.y, 0.0f), vec3_new(0.0f, 0.0f, size.z)); //Right
    imesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(size.x, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, size.z), vec3_new(0.0f, size.y, 0.0f)); //Left

    imesh3D_move(&mesh, vec3_mult(size, -0.5));

    return mesh;
}

iMesh3D imesh3D_shape_sphere(const size_t size)
{
    iMesh3D mesh = imesh3D_shape_cube(size);
    imesh3D_normalize(&mesh);
    imesh3D_normalize_faces(&mesh);
    return mesh;
}

void imesh3D_save(const iMesh3D* mesh, const char* path)
{
    imesh3D_save_wavefront(mesh, path);
}

void imesh3D_free(iMesh3D* mesh)
{
    table_free(&mesh->vertices);
    table_free(&mesh->normals);
    table_free(&mesh->uvs);
}

/* --- Procedural Opertations --- */

void imesh3D_push_quad_z(iMesh3D* mesh, const vec3 position)
{
    const vec2* quad = mass_shape_quad();
    for (size_t i = 0; i < 6; i++) {
        vec3 pos = {
            quad[i].x + position.x, 
            position.y, 
            quad[i].y + position.z
        };

        table_push(&mesh->vertices, &pos);
    }
}

void imesh3D_push_quad_xyz(iMesh3D* mesh, const vec3 position, const vec3 xvec, const vec3 yvec)
{
    const vec2* quad = mass_shape_quad();
    for (size_t i = 0; i < 6; i++) {
        vec3 pos = {
            quad[i].x * xvec.x + quad[i].y * yvec.x + position.x, 
            quad[i].x * xvec.y + quad[i].y * yvec.y + position.y, 
            quad[i].x * xvec.z + quad[i].y * yvec.z + position.z
        };
        
        table_push(&mesh->vertices, &pos);
    }
}

void imesh3D_push_plane_z(iMesh3D* mesh, const size_t xsize, const size_t ysize)
{
    for (size_t x = 0; x < xsize; x++) {
        for (size_t y = 0; y < ysize; y++) {
            imesh3D_push_quad_z(mesh, vec3_new((float)(int)x, 0.0, (float)(int)y));
        }
    }
}

void imesh3D_push_plane_xyz(iMesh3D* mesh, const size_t xsize, const size_t ysize, const vec3 position, const vec3 xvec, const vec3 yvec)
{
    for (size_t x = 0; x < xsize; x++) {
        for (size_t y = 0; y < ysize; y++) {
            float fx = (float)(int)x, fy = (float)(int)y;
            vec3 offset = {
                fx * xvec.x + fy * yvec.x + position.x, 
                fx * xvec.y + fy * yvec.y + position.y, 
                fx * xvec.z + fy * yvec.z + position.z
            };

            imesh3D_push_quad_xyz(mesh, offset, xvec, yvec);
        }
    }
}

void imesh3D_normalize_faces(iMesh3D* mesh)
{
    table_free(&mesh->normals);

    vec3* pos = mesh->vertices.data;
    size_t* indices = table_indices(&mesh->vertices);
    
    const size_t triangles = table_indices_size(&mesh->vertices) / 3;
    for (size_t i = 0; i < triangles; i++) {
        
        size_t p1 = indices[i * 3 + 0];
        size_t p2 = indices[i * 3 + 1];
        size_t p3 = indices[i * 3 + 2];

        vec3 vu = _vec3_sub(pos[p2], pos[p1]);
        vec3 vv = _vec3_sub(pos[p3], pos[p1]);
        vec3 n = _vec3_cross(vu, vv);
        n = vec3_norm(n);
        
        size_t search = table_push(&mesh->normals, &n) - 1;
        table_push_index(&mesh->normals, search);
        table_push_index(&mesh->normals, search);
    }
}

/* this function frees the second mesh passed in */
void imesh3D_merge(iMesh3D* restrict m1, iMesh3D* restrict m2)
{
    const size_t vsize = table_indices_size(&m2->vertices);
    size_t* vindices = m2->vertices.indices;
    vec2* v = m2->vertices.data;
    for (size_t i = 0; i < vsize; ++i) {
        table_push(&m1->vertices, v + vindices[i]);
    }

    const size_t nsize = table_indices_size(&m2->normals);
    size_t* nindices = m2->normals.indices;
    vec3* n = m2->normals.data;
    for (size_t i = 0; i < nsize; ++i) {
        table_push(&m1->normals, n + nindices[i]);
    }

    const size_t uvsize = table_indices_size(&m2->uvs);
    size_t* uvindices = m2->uvs.indices;
    vec2* uv = m2->uvs.data;
    for (size_t i = 0; i < uvsize; ++i) {
        table_push(&m1->uvs, uv + uvindices[i]);
    }

    imesh3D_free(m2);
}

void imesh3D_scale(iMesh3D* mesh, const float f)
{
    const size_t vsize = mesh->vertices.size;
    vec3* v = mesh->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec3_mult(v[i], f);
    }
}

void imesh3D_move(iMesh3D* mesh, const vec3 add)
{
    const size_t vsize = mesh->vertices.size;
    vec3* v = mesh->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec3_add(v[i], add);
    }
}

void imesh3D_normalize(iMesh3D* mesh)
{
    const size_t vsize = mesh->vertices.size;
    vec3* v = mesh->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec3_norm(v[i]);
    }
}

iMesh3D mesh3D_to_imesh3D(const Mesh3D* mesh)
{
    iMesh3D m;
    m.vertices = table_compress(mesh->vertices.data, mesh->vertices.bytes, mesh->vertices.size);
    m.normals = table_compress(mesh->normals.data, mesh->normals.bytes, mesh->normals.size);
    m.uvs = table_compress(mesh->uvs.data, mesh->uvs.bytes, mesh->uvs.size);
    return m;
}
