#include <mass.h>
#include <stdlib.h>
#include <string.h>

/*
--------------------------
 -> 3D Mesh of Vertices <- 
--------------------------
*/

vmesh_t vmesh_new()
{   
    vmesh_t mesh;
    mesh.vertices = array_create(sizeof(vertex_t));
    mesh.indices = array_create(sizeof(unsigned int));
    mesh.type = OBJ_V;
    return mesh;
}

vmesh_t vmesh_shape_plane(const unsigned int width, const unsigned int height)
{
    vmesh_t mesh = vmesh_new();
    vertex_array_push_plane_z(&mesh, width, height);
    vmesh_move(&mesh, vec3_new(-(float)width * 0.5, 0.0, -(float)height * 0.5));
    return mesh;
}

void vmesh_combine(vmesh_t* m1, vmesh_t* m2)
{
    const unsigned int size = m1->vertices.size;
    for (unsigned int i = 0; i < m2->vertices.size; i++) {
        array_push(&m1->vertices, array_index(&m2->vertices, i));
    }

    for (unsigned int i = 0; i < m2->indices.size; i++) {
        unsigned int k = *(unsigned int*)array_index(&m2->indices, i) + size;
        array_push(&m1->indices, &k);
    }

    array_cut(&m1->vertices);
    array_cut(&m1->indices);
    vmesh_free(m2);
}

void vmesh_scale(vmesh_t* mesh, float f)
{
    for (unsigned int i = 0; i < mesh->vertices.size; i++) {
        vertex_t* v = (vertex_t*)array_index(&mesh->vertices, i);
        v->position = vec3_mult(v->position, f);
    }
}

void vmesh_move(vmesh_t* mesh, vec3 add)
{
    for (unsigned int i = 0; i < mesh->vertices.capacity; i++) {
        vertex_t* v = (vertex_t*)array_index(&mesh->vertices, i);
        v->position = vec3_add(v->position, add);
    }
}

vmesh_t vmesh_shape_cube(const unsigned int size)
{
    vmesh_t mesh;
    mesh.type = OBJ_VN;
    mesh.vertices = array_reserve(sizeof(vertex_t), 36 * size * size);
    mesh.indices = array_reserve(sizeof(unsigned int), 36 * size * size);

    vertex_array_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, size, 0.0f), vec3_new(1.0f, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f)); //Up
    vertex_array_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f), vec3_new(1.0f, 0.0f, 0.0f)); //Down
    vertex_array_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(1.0f, 0.0f, 0.0f), vec3_new(0.0f, 1.0f, 0.0f)); //Front
    vertex_array_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, size), vec3_new(0.0f, 1.0f, 0.0f), vec3_new(1.0f, 0.0f, 0.0f)); //Back
    vertex_array_push_plane_xyz(&mesh, size, size, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, 1.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f)); //Right
    vertex_array_push_plane_xyz(&mesh, size, size, vec3_new(size, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, 1.0f), vec3_new(0.0f, 1.0f, 0.0f)); //Left

    array_cut(&mesh.indices);
    array_cut(&mesh.vertices);

    vmesh_move(&mesh, vec3_uni(-(float)size * 0.5f));
    vertex_array_set_face_normal(&mesh.vertices, &mesh.indices);
    return mesh;
}

vmesh_t vmesh_shape_hex(const vec3 size)
{
    vmesh_t mesh;
    mesh.type = OBJ_VN;
    mesh.vertices = array_reserve(sizeof(vertex_t), 36);
    mesh.indices = array_reserve(sizeof(unsigned int), 36);

    vertex_array_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, size.y, 0.0f), vec3_new(size.x, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, size.z)); //Up
    vertex_array_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, size.z), vec3_new(size.x, 0.0f, 0.0f)); //Down
    vertex_array_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(size.x, 0.0f, 0.0f), vec3_new(0.0f, size.y, 0.0f)); //Front
    vertex_array_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, size.z), vec3_new(0.0f, size.y, 0.0f), vec3_new(size.x, 0.0f, 0.0f)); //Back
    vertex_array_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0f, 0.0f, 0.0f), vec3_new(0.0f, size.y, 0.0f), vec3_new(0.0f, 0.0f, size.z)); //Right
    vertex_array_push_plane_xyz(&mesh, 1, 1, vec3_new(size.x, 0.0f, 0.0f), vec3_new(0.0f, 0.0f, size.z), vec3_new(0.0f, size.y, 0.0f)); //Left

    array_cut(&mesh.indices);
    array_cut(&mesh.vertices);

    vmesh_move(&mesh, vec3_mult(size, -0.5));
    vertex_array_set_face_normal(&mesh.vertices, &mesh.indices);
    return mesh;
}

void vmesh_normalize_positions(vmesh_t* mesh)
{
    for (unsigned int i = 0; i < mesh->vertices.size; i++) {
        vertex_t* v = (vertex_t*)array_index(&mesh->vertices, i);
        v->position = vec3_norm(v->position);
    }
}

vmesh_t vmesh_shape_sphere(unsigned int size)
{
    vmesh_t mesh = vmesh_shape_cube(size);
    vmesh_normalize_positions(&mesh);
    vertex_array_set_face_normal(&mesh.vertices, &mesh.indices);
    return mesh;
}

void vmesh_smooth_optim(vmesh_t* m)
{
    vmesh_t* mm = m;
    if (m != NULL && mm != NULL) {
        vmesh_t mesh;
        mesh.vertices = array_reserve(sizeof(vertex_t), mm->vertices.size);
        mesh.indices = array_reserve(sizeof(unsigned int), mm->vertices.size);
        mesh.type = OBJ_VN;

        for (unsigned int i = 0; i < mm->indices.size; i++) {
            unsigned int n = *(unsigned int*)array_index(&mm->indices, i);
            vertex_t* v = (vertex_t*)array_index(&mm->vertices, n);
            unsigned int k = mesh.vertices.size, found = 0;
            for (unsigned int j = 0; j < mesh.vertices.size; j++) {
                vertex_t* vptr = (vertex_t*)array_index(&mesh.vertices, j);
                if (!memcmp(&v->position, &vptr->position, sizeof(vec3))) {
                    v->normal = vec3_add(v->normal, vptr->normal);
                    k = (unsigned int)j;
                    found++;
                    break;
                }
            }
            if (!found) array_push(&mesh.vertices, v);
            array_push(&mesh.indices, &k);
        }
        array_cut(&mesh.vertices);
        vmesh_free(mm);
        *m = mesh;
    }
}

void vmesh_smooth_sphere(vmesh_t* m)
{
    vmesh_t* mm = m;
    if (m != NULL && mm != NULL) {
        vmesh_t mesh;
        mesh.vertices = array_reserve(sizeof(vertex_t), mm->vertices.size);
        mesh.indices = array_reserve(sizeof(unsigned int), mm->vertices.size);
        
        if (mm->type != OBJ_VTN) mesh.type = OBJ_VN;
        else mesh.type = OBJ_VTN;

        for (unsigned int i = 0; i < mm->indices.size; i++) {
            unsigned int n = *(unsigned int*)array_index(&mm->indices, i);
            vertex_t* v = (vertex_t*)array_index(&mm->vertices, n);

            unsigned int k = mesh.vertices.size, found = 0;
            for (unsigned int j = 0; j < mesh.vertices.size; j++) {
                vertex_t* vptr = (vertex_t*)array_index(&mesh.vertices, j);
                if (!memcmp(&v->position, &vptr->position, sizeof(vec3))) {
                    k = (unsigned int)j;
                    found++;
                    break;
                }
            }
            if (!found) {
                v->normal = v->position;
                array_push(&mesh.vertices, v);
            }
            array_push(&mesh.indices, &k);
        }
        array_cut(&mesh.vertices);
        vmesh_free(mm);
        *m = mesh;
    }
}

void vmesh_min_max_height(vmesh_t* mesh, float* min, float* max)
{
    float max_ret = -1000.0f;
    float min_ret = 1000.0f;
    vertex_t* v = (vertex_t*)mesh->vertices.data;
    for (vertex_t* end = v + mesh->vertices.size; v != end; v++) {
        float height = v->position.y;
        if (height > max_ret) max_ret = height;
        if (height < min_ret) min_ret = height;
    }
    *max = max_ret;
    *min = min_ret;
}

void vmesh_height_color_gradient(vmesh_t* mesh)
{
    mesh->type = OBJ_VTN;
    float max, min;
    vmesh_min_max_height(mesh, &min, &max);
    vertex_t* v = (vertex_t*)mesh->vertices.data;
    for (vertex_t* end = v + mesh->vertices.size; v != end; v++) {
        float height = v->position.y;
        float g = inverse_lerpf(min, max, height);
        v->uv = vec2_new(g, 0.0f);
    }
}

void vmesh_free(vmesh_t* mesh)
{
    array_free(&mesh->vertices);
    array_free(&mesh->indices);
}

vmesh_t vmesh_load(const char* path)
{
    obj_flag type = OBJ_V;
    array_t v_index = array_create(sizeof(unsigned int));
    array_t u_index = array_create(sizeof(unsigned int));
    array_t n_index = array_create(sizeof(unsigned int));
    array_t temp_positions = array_create(sizeof(vec3));
    array_t temp_normals = array_create(sizeof(vec3));
    array_t temp_uvs = array_create(sizeof(vec2));

    wavefront_file_load(path, &type, &v_index, &u_index, &n_index, &temp_positions, &temp_uvs, &temp_normals);

    vmesh_t mesh;
    mesh.vertices = array_reserve(sizeof(vertex_t), temp_positions.size);
    mesh.indices = array_reserve(sizeof(unsigned int), v_index.size);
    mesh.type = type;

    for (unsigned int i = 0; i < v_index.size; i++) {
        vec3 pos, norm;
        vec2 uv;
        
        unsigned int v = *(unsigned int*)array_index(&v_index, i);
        pos = *(vec3*)array_index(&temp_positions, v - 1);

        if (mesh.type == OBJ_VTN) {
            unsigned int vt = *(unsigned int*)array_index(&u_index, i);
            uv = *(vec2*)array_index(&temp_uvs, vt - 1);
        } else uv = vec2_new(0.0f, 0.0f);

        if (mesh.type != OBJ_V) {
            unsigned int vn = *(unsigned int*)array_index(&n_index, i);
            norm = *(vec3*)array_index(&temp_normals, vn - 1);
        } else norm = vec3_new(0.0f, 1.0f, 0.0f);

        vertex_t vertex = vertex_new(pos, uv, norm);
        unsigned int k = mesh.vertices.size, found = 0;
        for (unsigned int j = 0; j < mesh.vertices.size; j++) {
            vertex_t* v = (vertex_t*)array_index(&mesh.vertices, j);
            if (!memcmp(&vertex, v, sizeof(vertex_t))) {
                k = j;
                found++;
                break;
            }
        }
        if (!found) array_push(&mesh.vertices, &vertex);
        array_push(&mesh.indices, &k);
    }

    array_free(&temp_positions);
    array_free(&temp_uvs);
    array_free(&temp_normals);
    array_free(&v_index);
    array_free(&u_index);
    array_free(&n_index);

    array_cut(&mesh.vertices);
    return mesh;
}

vmesh_t vmesh_load_optim(const char* path)
{
    obj_flag type = OBJ_V;
    array_t v_index = array_create(sizeof(unsigned int));
    array_t u_index = array_create(sizeof(unsigned int));
    array_t n_index = array_create(sizeof(unsigned int));
    array_t temp_positions = array_create(sizeof(vec3));
    array_t temp_normals = array_create(sizeof(vec3));
    array_t temp_uvs = array_create(sizeof(vec2));

    wavefront_file_load(path, &type, &v_index, &u_index, &n_index, &temp_positions, &temp_uvs, &temp_normals);

    vmesh_t mesh;
    mesh.vertices = array_reserve(sizeof(vertex_t), temp_positions.size);
    mesh.indices = array_reserve(sizeof(unsigned int), v_index.size);
    mesh.type = type;

    for (unsigned int i = 0; i < v_index.size; i++) {
        vec3 pos, norm;
        vec2 uv;
        
        unsigned int v = *(unsigned int*)array_index(&v_index, i);
        pos = *(vec3*)array_index(&temp_positions, v - 1);

        if (mesh.type == OBJ_VTN) {
            unsigned int vt = *(unsigned int*)array_index(&u_index, i);
            uv = *(vec2*)array_index(&temp_uvs, vt - 1);
        } else uv = vec2_new(0.0f, 0.0f);

        if (mesh.type != OBJ_V) {
            unsigned int vn = *(unsigned int*)array_index(&n_index, i);
            norm = *(vec3*)array_index(&temp_normals, vn - 1);
        } else norm = vec3_new(0.0f, 1.0f, 0.0f);

        vertex_t vertex = vertex_new(pos, uv, norm);
        unsigned int k = mesh.vertices.size, found = 0;
        for (unsigned int j = 0; j < mesh.vertices.size; j++) {
            vertex_t* v = (vertex_t*)array_index(&mesh.vertices, j);
            if (!memcmp(&pos, &v->position, sizeof(vec3))) {
                k = (unsigned int)j;
                found++;
                break;
            }
        }
        if (!found) array_push(&mesh.vertices, &vertex);
        array_push(&mesh.indices, &k);
    }

    array_free(&temp_positions);
    array_free(&temp_uvs);
    array_free(&temp_normals);
    array_free(&v_index);
    array_free(&u_index);
    array_free(&n_index);

    array_cut(&mesh.vertices);
    return mesh;
}