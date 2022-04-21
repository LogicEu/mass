#include <mass.h>

/* --- Procedural Opertations --- */

static const float quad[] = {
    1.0f,   1.0f,
    1.0f,   0.0f,
    0.0f,   1.0f,
    1.0f,   0.0f,
    0.0f,   0.0f,
    0.0f,   1.0f
};

void mesh_push_quad_z(mesh_t* mesh, const float x, const float y)
{
    for (size_t i = 0; i < 6; i++) {
        vec3 pos = {
            quad[i * 2] + x, 
            0.0f, 
            quad[i * 2 + 1] + y
        };

        array_push(&mesh->vertices, &pos);
    }
}

void imesh_push_quad_z(imesh_t* mesh, const float x, const float y)
{
    for (size_t i = 0; i < 6; i++) {
        vec3 pos = {
            quad[i * 2] + x, 
            0.0f, 
            quad[i * 2 + 1] + y
        };

        table_push(&mesh->vertices, &pos);
    }
}

void mesh_push_quad_xyz(mesh_t* mesh, const vec3 off, const vec3 xvec, const vec3 yvec)
{
    for (size_t i = 0; i < 6; i++) {
        vec3 pos = {
            quad[i * 2] * xvec.x + quad[i * 2 + 1] * yvec.x + off.x, 
            quad[i * 2] * xvec.y + quad[i * 2 + 1] * yvec.y + off.y, 
            quad[i * 2] * xvec.z + quad[i * 2 + 1] * yvec.z + off.z
        };
        
        array_push(&mesh->vertices, &pos);
    }
}

void imesh_push_quad_xyz(imesh_t* mesh, const vec3 off, const vec3 xvec, const vec3 yvec)
{
    for (size_t i = 0; i < 6; i++) {
        vec3 pos = {
            quad[i * 2] * xvec.x + quad[i * 2 + 1] * yvec.x + off.x, 
            quad[i * 2] * xvec.y + quad[i * 2 + 1] * yvec.y + off.y, 
            quad[i * 2] * xvec.z + quad[i * 2 + 1] * yvec.z + off.z
        };
        
        table_push(&mesh->vertices, &pos);
    }
}

void mesh_push_plane_z(mesh_t* mesh, const size_t xsize, const size_t ysize)
{
    for (size_t x = 0; x < xsize; x++) {
        for (size_t y = 0; y < ysize; y++) {
            mesh_push_quad_z(mesh, (float)(int)x, (float)(int)y);
        }
    }
}

void imesh_push_plane_z(imesh_t* mesh, const size_t xsize, const size_t ysize)
{
    for (size_t x = 0; x < xsize; x++) {
        for (size_t y = 0; y < ysize; y++) {
            imesh_push_quad_z(mesh, (float)(int)x, (float)(int)y);
        }
    }
}

void mesh_push_plane_xyz(mesh_t* mesh, const size_t xsize, const size_t ysize, const vec3 off, const vec3 xvec, const vec3 yvec)
{
    for (size_t x = 0; x < xsize; x++) {
        for (size_t y = 0; y < ysize; y++) {
            float fx = (float)(int)x, fy = (float)(int)y;
            vec3 offset = {
                fx * xvec.x + fy * yvec.x + off.x, 
                fx * xvec.y + fy * yvec.y + off.y, 
                fx * xvec.z + fy * yvec.z + off.z
            };

            mesh_push_quad_xyz(mesh, offset, xvec, yvec);
        }
    }
}

void imesh_push_plane_xyz(imesh_t* mesh, const size_t xsize, const size_t ysize, const vec3 off, const vec3 xvec, const vec3 yvec)
{
    for (size_t x = 0; x < xsize; x++) {
        for (size_t y = 0; y < ysize; y++) {
            float fx = (float)(int)x, fy = (float)(int)y;
            vec3 offset = {
                fx * xvec.x + fy * yvec.x + off.x, 
                fx * xvec.y + fy * yvec.y + off.y, 
                fx * xvec.z + fy * yvec.z + off.z
            };

            imesh_push_quad_xyz(mesh, offset, xvec, yvec);
        }
    }
}

void mesh_normals_get_face(mesh_t* mesh)
{
    array_clear(&mesh->normals);

    vec3* pos = mesh->vertices.data;
    
    const size_t triangles = mesh->vertices.size / 3;
    for (size_t i = 0; i < triangles; i++) {
        
        vec3 vu = _vec3_sub(pos[i * 3 + 1], pos[i * 3]);
        vec3 vv = _vec3_sub(pos[i * 3 + 2], pos[i * 3]);
        vec3 n = _vec3_cross(vu, vv);
        n = vec3_norm(n);
        
        array_push(&mesh->normals, &n);
        array_push(&mesh->normals, &n);
        array_push(&mesh->normals, &n);
    }
}

void imesh_normals_get_face(imesh_t* mesh)
{
    array_clear(&mesh->normals.indices);
    array_clear(&mesh->normals.values);

    vec3* pos = mesh->vertices.values.data;
    size_t* indices = mesh->vertices.indices.data;
    
    const size_t triangles = mesh->vertices.indices.size / 3;
    for (size_t i = 0; i < triangles; i++) {
        
        size_t p1 = indices[i * 3 + 0];
        size_t p2 = indices[i * 3 + 1];
        size_t p3 = indices[i * 3 + 2];

        vec3 vu = _vec3_sub(pos[p2], pos[p1]);
        vec3 vv = _vec3_sub(pos[p3], pos[p1]);
        vec3 n = _vec3_cross(vu, vv);
        n = vec3_norm(n);
        
        size_t search = table_push(&mesh->normals, &n);
        array_push(&mesh->normals.indices, &search);
        array_push(&mesh->normals.indices, &search);
    }
}

void mesh_merge(mesh_t* restrict m1, mesh_t* restrict m2)
{
    const size_t vsize = m2->vertices.size;
    vec3* v = m2->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        array_push(&m1->vertices, v + i);
    }

    const size_t nsize = m2->normals.size;
    vec3* n = m2->vertices.data;
    for (size_t i = 0; i < nsize; i++) {
        array_push(&m1->normals, n + i);
    }

    const size_t uvsize = m2->uvs.size;
    vec2* uv = m2->vertices.data;
    for (size_t i = 0; i < uvsize; i++) {
        array_push(&m1->vertices, uv + i);
    }

    mesh_free(m2);
}

void imesh_merge(imesh_t* restrict m1, imesh_t* restrict m2)
{
    const size_t vsize = m2->vertices.indices.size;
    size_t* vindices = m2->vertices.indices.data;
    vec2* v = m2->vertices.values.data;
    for (size_t i = 0; i < vsize; ++i) {
        table_push(&m1->vertices, v + vindices[i]);
    }

    const size_t nsize = m2->normals.indices.size;
    size_t* nindices = m2->vertices.indices.data;
    vec3* n = m2->normals.values.data;
    for (size_t i = 0; i < nsize; ++i) {
        table_push(&m1->normals, n + nindices[i]);
    }

    const size_t uvsize = m2->uvs.indices.size;
    size_t* uvindices = m2->vertices.indices.data;
    vec2* uv = m2->uvs.values.data;
    for (size_t i = 0; i < uvsize; ++i) {
        table_push(&m1->uvs, uv + uvindices[i]);
    }

    imesh_free(m2);
}

void mesh_scale(mesh_t* mesh, const float f)
{
    const size_t vsize = mesh->vertices.size;
    vec3* v = mesh->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec3_mult(v[i], f);
    }
}

void imesh_scale(imesh_t* mesh, const float f)
{
    const size_t vsize = mesh->vertices.values.size;
    vec3* v = mesh->vertices.values.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec3_mult(v[i], f);
    }
}

void mesh_move(mesh_t* mesh, const vec3 add)
{
    const size_t vsize = mesh->vertices.size;
    vec3* v = mesh->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec3_add(v[i], add);
    }
}

void imesh_move(imesh_t* mesh, const vec3 add)
{
    const size_t vsize = mesh->vertices.values.size;
    vec3* v = mesh->vertices.values.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec3_add(v[i], add);
    }
}

void mesh_normalize(mesh_t* mesh)
{
    const size_t vsize = mesh->vertices.size;
    vec3* v = mesh->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec3_norm(v[i]);
    }
}

void imesh_normalize(imesh_t* mesh)
{
    const size_t vsize = mesh->vertices.values.size;
    vec3* v = mesh->vertices.values.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec3_norm(v[i]);
    }
}