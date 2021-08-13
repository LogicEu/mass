#include <mass.h>

/*
-----------------------------
 -> Procedural Opertations <- 
-----------------------------
*/

void vec3_position_array_push_quad_z(array_t* positions, float x, float y)
{
    float quad[] = {
        x + 1.0f,   y + 1.0f,
        x + 1.0f,   y,
        x,          y + 1.0f,
        x + 1.0f,   y,
        x,          y,
        x,          y + 1.0f
    };

    for (unsigned int i = 0; i < 6; i++) {
        vec3 v = {quad[i * 2], 0.0f, quad[i * 2 + 1]};
        array_push(positions, &v);
    }
}

void vertex_array_push_quad_z(array_t* vertices, array_t* indices, float x, float y)
{
    float quad[] = {
        x + 1.0f,   y + 1.0f,
        x + 1.0f,   y,
        x,          y + 1.0f,
        x + 1.0f,   y,
        x,          y,
        x,          y + 1.0f
    };

    for (unsigned int i = 0; i < 6; i++) {
        vec3 pos = {quad[i * 2], 0.0f, quad[i * 2 + 1]};
        vertex_t v = vertex_new(pos, vec2_new(0.0f, 0.0f), vec3_new(0.0f, 1.0f, 0.0f));
        array_push(indices, &vertices->used);
        array_push(vertices, &v);
    }
}

void vertex_array_push_quad_xyz(array_t* vertices, array_t* indices, vec3 off, vec3 xvec, vec3 yvec)
{
    float quad[] = {
        1.0f,   1.0f,
        1.0f,   0.0f,
        0.0f,   1.0f,
        1.0f,   0.0f,
        0.0f,   0.0f,
        0.0f,   1.0f
    };

    for (unsigned int i = 0; i < 6; i++) {
        float qx = quad[i * 2];
        float qy = quad[i * 2 + 1];

        vec3 pos = {qx * xvec.x + qy * yvec.x + off.x, qx * xvec.y + qy * yvec.y + off.y, qx * xvec.z + qy * yvec.z + off.z};
        vertex_t v = vertex_new(pos, vec2_new(qx, qy), vec3_new(0.0f, 1.0f, 0.0f));
        array_push(indices, &vertices->used);
        array_push(vertices, &v);
    }
}

void vertex_array_push_plane_z(vmesh_t* mesh, const unsigned int x_size, const unsigned int y_size)
{
    for (unsigned int x = 0; x < x_size; x++) {
        for (unsigned int y = 0; y < y_size; y++) {
            vertex_array_push_quad_z(mesh->vertices, mesh->indices, (float)x, (float)y);
        }
    }
}

void vertex_array_push_plane_xyz(vmesh_t* mesh, const unsigned int x_size, const unsigned int y_size, vec3 off, vec3 xvec, vec3 yvec)
{
    for (unsigned int x = 0; x < x_size; x++) {
        for (unsigned int y = 0; y < y_size; y++) {
            vertex_array_push_quad_xyz(mesh->vertices, mesh->indices, 
                            vec3_new(x * xvec.x + y * yvec.x + off.x, x * xvec.y + y * yvec.y + off.y, x * xvec.z + y * yvec.z + off.z),
                            xvec, yvec);
        }
    }
}

array_t* vec3_face_normal_array(array_t* positions)
{
    array_t* normals = array_new(positions->used, positions->bytes);
    const unsigned int triangles = positions->used / 3;
    for (unsigned int i = 0; i < triangles; i++) {
        vec3 v1 = *(vec3*)array_index(positions, i * 3 + 0);
        vec3 v2 = *(vec3*)array_index(positions, i * 3 + 1);
        vec3 v3 = *(vec3*)array_index(positions, i * 3 + 2);
        vec3 vu = vec3_sub(v2, v1);
        vec3 vv = vec3_sub(v3, v1);
        vec3 n = vec3_cross(vu, vv);
        for(unsigned int j = 0; j < 3; j++) {
            array_push(normals, &n);
        } 
    }
    return normals;
}

void vertex_array_set_face_normal(array_t* vertices, array_t* indices)
{
    const unsigned int face_count = indices->used / 3;
    unsigned int* k = (unsigned int*)indices->data;
    for (unsigned int i = 0; i < face_count; i++) {
        vertex_t* v1 = (vertex_t*)array_index(vertices, *(k + 0));
        vertex_t* v2 = (vertex_t*)array_index(vertices, *(k + 1));
        vertex_t* v3 = (vertex_t*)array_index(vertices, *(k + 2));

        vec3 vu = vec3_sub(v2->position, v1->position);
        vec3 vv = vec3_sub(v3->position, v1->position);
        vec3 n = vec3_cross(vu, vv);

        v1->normal = n;
        v2->normal = n;
        v3->normal = n;
        k += 3;
    }
}