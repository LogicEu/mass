#include <mass.h>

/* --- Indexed 2D Mesh --- */

iMesh2D imesh2D_create(void)
{
    iMesh2D mesh;

    mesh.vertices = table_create(sizeof(vec2));
    mesh.uvs = table_create(sizeof(vec2));

    return mesh;
}

iMesh2D imesh2D_shape_quad(const vec2 position)
{   
    iMesh2D mesh = imesh2D_create();
    imesh2D_push_quad(&mesh, position);
    return mesh;
}

iMesh2D imesh2D_shape_plane(const size_t width, const size_t height)
{   
    iMesh2D mesh = imesh2D_create();

    for (size_t x = 0; x < width; x++) {
        for (size_t y = 0; y < height; y++) {
            imesh2D_push_quad(&mesh, vec2_new((float)(int)x, (float)(int)y));
        }
    }

    return mesh;
}

iMesh2D imesh2D_shape_polygon(const size_t sides)
{
    iMesh2D mesh = imesh2D_create();
    
    const float offset = 2 * M_PI / (float)sides;
    
    vec2 center = {0.0f, 0.0f};
    vec2 p1 = {cosf(0.0), sinf(0.0)};
    
    for (size_t i = 1; i < sides; ++i) {
        
        float rot = (float)i * offset;
        vec2 p2 = {cosf(rot), sinf(rot)};

        table_push(&mesh.vertices, &center);
        table_push(&mesh.vertices, &p1);
        table_push(&mesh.vertices, &p2);

        p1 = p2;
    }

    return mesh;
}

void imesh2D_free(iMesh2D* mesh)
{
    table_free(&mesh->vertices);
    table_free(&mesh->uvs);
}

/* --- Procedural Opertations --- */

void imesh2D_push_quad(iMesh2D* mesh, const vec2 position)
{
    const vec2* quad = mass_shape_quad();
    for (size_t i = 0; i < 6; i++) {
        vec2 pos = {
            quad[i].x + position.x,
            quad[i].y + position.y
        };

        table_push(&mesh->vertices, &pos);
    }
}

void imesh2D_push_plane(iMesh2D* mesh, const size_t xsize, const size_t ysize)
{
    for (size_t x = 0; x < xsize; x++) {
        for (size_t y = 0; y < ysize; y++) {
            imesh2D_push_quad(mesh, vec2_new((float)(int)x, (float)(int)y));
        }
    }
}

/* this function frees the second mesh passed in */
void imesh2D_merge(iMesh2D* restrict m1, iMesh2D* restrict m2)
{
    const size_t vsize = table_indices_size(&m2->vertices);
    size_t* vindices = m2->vertices.indices;
    vec2* v = m2->vertices.data;
    for (size_t i = 0; i < vsize; ++i) {
        table_push(&m1->vertices, v + vindices[i]);
    }

    const size_t uvsize = table_indices_size(&m2->uvs);
    size_t* uvindices = m2->uvs.indices;
    vec2* uv = m2->uvs.data;
    for (size_t i = 0; i < uvsize; ++i) {
        table_push(&m1->uvs, uv + uvindices[i]);
    }

    imesh2D_free(m2);
}

void imesh2D_scale(iMesh2D* mesh, const float f)
{
    const size_t vsize = mesh->vertices.size;
    vec2* v = mesh->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec2_mult(v[i], f);
    }
}

void imesh2D_move(iMesh2D* mesh, const vec2 add)
{
    const size_t vsize = mesh->vertices.size;
    vec2* v = mesh->vertices.data;
    for (size_t i = 0; i < vsize; i++) {
        v[i] = vec2_add(v[i], add);
    }
}
