#include <mass.h>

/* --- Common 2D & 3D Shapes --- */

#define countoff(x) (sizeof(x) / sizeof(x[0]))

static const vec2 quad[] = {
    {1.0f,   1.0f},
    {1.0f,   0.0f},
    {0.0f,   1.0f},
    {1.0f,   0.0f},
    {0.0f,   0.0f},
    {0.0f,   1.0f}
};

static const vec3 cube[] = {
    {-1.0f,  -1.0f,  -1.0f}, 
    {-1.0f,  -1.0f,  1.0f },
    {-1.0f,  1.0f,   1.0f }, 
    {1.0f,   1.0f,   -1.0f}, 
    {-1.0f,  -1.0f,  -1.0f},
    {-1.0f,  1.0f,   -1.0f}, 
    {1.0f,   -1.0f,  1.0f },
    {-1.0f,  -1.0f,  -1.0f},
    {1.0f,   -1.0f,  -1.0f},
    {1.0f,   1.0f,   -1.0f},
    {1.0f,   -1.0f,  -1.0f},
    {-1.0f,  -1.0f,  -1.0f},
    {-1.0f,  -1.0f,  -1.0f},
    {-1.0f,  1.0f,   1.0f },
    {-1.0f,  1.0f,   -1.0f},
    {1.0f,   -1.0f,  1.0f },
    {-1.0f,  -1.0f,  1.0f },
    {-1.0f,  -1.0f,  -1.0f},
    {-1.0f,  1.0f,   1.0f },
    {-1.0f,  -1.0f,  1.0f },
    {1.0f,   -1.0f,  1.0f },
    {1.0f,   1.0f,   1.0f },
    {1.0f,   -1.0f,  -1.0f},
    {1.0f,   1.0f,   -1.0f},
    {1.0f,   -1.0f,  -1.0f},
    {1.0f,   1.0f,   1.0f },
    {1.0f,   -1.0f,  1.0f },
    {1.0f,   1.0f,   1.0f },
    {1.0f,   1.0f,   -1.0f},
    {-1.0f,  1.0f,   -1.0f},
    {1.0f,   1.0f,   1.0f },
    {-1.0f,  1.0f,   -1.0f},
    {-1.0f,  1.0f,   1.0f },
    {1.0f,   1.0f,   1.0f },
    {-1.0f,  1.0f,   1.0f },
    {1.0f,   -1.0f,  1.0f }
};

static const vec2 cube_uvs[] = {
    {0.0f,   1.0f},
    {1.0f,   1.0f},
    {1.0f,   0.0f},
    {0.0f,   0.0f},
    {1.0f,   1.0f},
    {1.0f,   0.0f},
    {0.0f,   0.0f},
    {1.0f,   1.0f},
    {1.0f,   0.0f},
    {0.0f,   0.0f},
    {0.0f,   1.0f},
    {1.0f,   1.0f},
    {0.0f,   1.0f},
    {1.0f,   0.0f},
    {0.0f,   0.0f},
    {0.0f,   0.0f},
    {0.0f,   1.0f},
    {1.0f,   1.0f},
    {0.0f,   0.0f},
    {0.0f,   1.0f},
    {1.0f,   1.0f},
    {0.0f,   0.0f},
    {1.0f,   1.0f},
    {1.0f,   0.0f},
    {1.0f,   1.0f},
    {0.0f,   0.0f},
    {0.0f,   1.0f},
    {0.0f,   1.0f},
    {1.0f,   1.0f},
    {1.0f,   0.0f},
    {0.0f,   1.0f},
    {1.0f,   0.0f},
    {0.0f,   0.0f},
    {1.0f,   0.0f},
    {0.0f,   0.0f},
    {1.0f,   1.0f},
};

static const vec3 skybox[] = {     
    {-1.0f,  1.0f,   -1.0f},
    {-1.0f,  -1.0f,  -1.0f},
    {1.0f,   -1.0f,  -1.0f},
    {1.0f,   -1.0f,  -1.0f},
    {1.0f,   1.0f,   -1.0f},
    {-1.0f,  1.0f,   -1.0f},
    {-1.0f,  -1.0f,  1.0f },
    {-1.0f,  -1.0f,  -1.0f},
    {-1.0f,  1.0f,   -1.0f},
    {-1.0f,  1.0f,   -1.0f},
    {-1.0f,  1.0f,   1.0f },
    {-1.0f,  -1.0f,  1.0f },
    {1.0f,   -1.0f,  -1.0f},
    {1.0f,   -1.0f,  1.0f },
    {1.0f,   1.0f,   1.0f },
    {1.0f,   1.0f,   1.0f },
    {1.0f,   1.0f,   -1.0f},
    {1.0f,   -1.0f,  -1.0f},
    {-1.0f,  -1.0f,  1.0f },
    {-1.0f,  1.0f,   1.0f },
    {1.0f,   1.0f,   1.0f },
    {1.0f,   1.0f,   1.0f },
    {1.0f,   -1.0f,  1.0f },
    {-1.0f,  -1.0f,  1.0f },
    {-1.0f,  1.0f,   -1.0f},
    {1.0f,   1.0f,   -1.0f},
    {1.0f,   1.0f,   1.0f },
    {1.0f,   1.0f,   1.0f },
    {-1.0f,  1.0f,   1.0f },
    {-1.0f,  1.0f,   -1.0f},
    {-1.0f,  -1.0f,  -1.0f},
    {-1.0f,  -1.0f,  1.0f },
    {1.0f,   -1.0f,  -1.0f},
    {1.0f,   -1.0f,  -1.0f},
    {-1.0f,  -1.0f,  1.0f },
    {1.0f,   -1.0f,  1.0f }
};

const vec2* mass_shape_quad(void)
{
    return quad;
}

size_t mass_shape_quad_size(void)
{
    return countoff(quad);
}

const vec3* mass_shape_cube(void)
{
    return cube;
}

size_t mass_shape_cube_size(void)
{
    return countoff(cube);
}

const vec2* mass_shape_cube_uvs(void)
{
    return cube_uvs;
}

size_t mass_shape_cube_uvs_size(void)
{
    return countoff(cube_uvs);
}

const vec3* mass_shape_skybox(void)
{
    return skybox;
}

size_t mass_shape_skybox_size(void)
{
    return countoff(skybox);
}
