#include <mass.h>

/* --- 2D Mesh --- */

Mesh2D mesh2D_create(void)
{
    Mesh2D mesh;

    mesh.vertices = array_create(sizeof(vec2));
    mesh.uvs = array_create(sizeof(vec2));

    return mesh;
}

Mesh2D mesh2D_shape_quad(const vec2 position)
{   
    Mesh2D mesh = mesh2D_create();
    array_resize(&mesh.vertices, 6);
    mesh2D_push_quad(&mesh, position);
    return mesh;
}

Mesh2D mesh2D_shape_plane(const size_t width, const size_t height)
{   
    Mesh2D mesh = mesh2D_create();
    array_resize(&mesh.vertices, width * height * 6);

    for (size_t x = 0; x < width; x++) {
        for (size_t y = 0; y < height; y++) {
            mesh2D_push_quad(&mesh, vec2_new((float)(int)x, (float)(int)y));
        }
    }

    return mesh;
}

Mesh2D mesh2D_shape_polygon(const size_t sides)
{
    Mesh2D mesh = mesh2D_create();
    
    const float offset = 2 * M_PI / (float)sides;
    
    vec2 center = {0.0f, 0.0f};
    vec2 orig = {cosf(0.0), sinf(0.0)};
    vec2 p1 = orig;
    
    for (size_t i = 1; i < sides; ++i) {
        
        float rot = (float)i * offset;
        vec2 p2 = {cosf(rot), sinf(rot)};

        array_push(&mesh.vertices, &center);
        array_push(&mesh.vertices, &p1);
        array_push(&mesh.vertices, &p2);

        p1 = p2;
    }

    array_push(&mesh.vertices, &center);
    array_push(&mesh.vertices, &p1);
    array_push(&mesh.vertices, &orig);

    return mesh;
}

void mesh2D_free(Mesh2D* mesh)
{
    array_free(&mesh->vertices);
    array_free(&mesh->uvs);
}

/* --- Procedural Opertations --- */

void mesh2D_push_quad(Mesh2D* mesh, const vec2 position)
{
    const vec2* quad = mass_shape_quad();
    for (size_t i = 0; i < 6; i++) {
        vec2 pos = {
            quad[i].x + position.x, 
            quad[i].y + position.y
        };

        array_push(&mesh->vertices, &pos);
    }
}

void mesh2D_push_plane(Mesh2D* mesh, const size_t xsize, const size_t ysize)
{
    for (size_t x = 0; x < xsize; x++) {
        for (size_t y = 0; y < ysize; y++) {
            mesh2D_push_quad(mesh, vec2_new((float)(int)x, (float)(int)y));
        }
    }
}

void mesh2D_merge(Mesh2D* restrict m1, Mesh2D* restrict m2)
{
    const size_t vsize = m2->vertices.size;
    vec2* v = m2->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        array_push(&m1->vertices, v + i);
    }

    const size_t uvsize = m2->uvs.size;
    vec2* uv = m2->vertices.data;
    for (size_t i = 0; i < uvsize; i++) {
        array_push(&m1->vertices, uv + i);
    }

    mesh2D_free(m2);
}

void mesh2D_scale(Mesh2D* mesh, const float f)
{
    const size_t vsize = mesh->vertices.size;
    vec2* v = mesh->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec2_mult(v[i], f);
    }
}

void mesh2D_move(Mesh2D* mesh, const vec2 add)
{
    const size_t vsize = mesh->vertices.size;
    vec2* v = mesh->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec2_add(v[i], add);
    }
}
