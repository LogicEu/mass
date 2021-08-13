#include <fract.h>

/*********************************************
 -> three dimensional floating point vector <- 
*********************************************/

vec3 vec3_new(float x, float y, float z)
{
    vec3 ret = {x, y, z};
    return ret;
}

vec3 vec3_uni(float f)
{
    vec3 ret = {f, f, f};
    return ret;
}

int vec3_cmp(vec3 v1, vec3 v2)
{
    return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
}

vec3 vec3_add(vec3 v1, vec3 v2)
{
    vec3 ret = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
    return ret;
}

vec3 vec3_sub(vec3 v1, vec3 v2)
{
    vec3 ret = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
    return ret;
}

vec3 vec3_neg(vec3 v)
{
    vec3 ret = {-v.x, -v.y, -v.z};
    return ret;
}

vec3 vec3_mult(vec3 v, float f)
{
    vec3 ret = {v.x * f, v.y * f, v.z * f};
    return ret;
}

vec3 vec3_div(vec3 v, float f)
{
    vec3 ret = {v.x / f, v.y / f, v.z / f};
    return ret;
}

float vec3_sqmag(vec3 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float vec3_mag(vec3 v)
{
    return fast_sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float vec3_sqdist(vec3 v1, vec3 v2)
{
    return vec3_sqmag(vec3_sub(v1, v2));
}

float vec3_dist(vec3 v1, vec3 v2)
{
    return vec3_mag(vec3_sub(v1, v2));
}

float vec3_dot(vec3 v1, vec3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3 vec3_cross(vec3 v1, vec3 v2)
{
    vec3 ret = {
        v1.y * v2.z - v2.y * v1.z,
        v1.z * v2.x - v2.z * v1.x,
        v1.x * v2.y - v2.x * v1.y
    };
    return ret;
}

vec3 vec3_norm(vec3 v)
{
    return vec3_mult(v, fast_inverse_sqrt(vec3_dot(v, v)));
}

vec3 vec3_normf(vec3 v)
{
    return vec3_mult(v, faster_inverse_sqrt(vec3_dot(v, v)));
}

vec3 vec3_normal(vec3 v)
{
    return vec3_mult(v, 1.0f / sqrtf(vec3_sqmag(v)));
}

vec3 vec3_prod(vec3 a, vec3 b)
{
    vec3 ret = {a.x * b.x, a.y * b.y, a.z * b.z};
    return ret;
}

void vec3_move(vec3* v, vec3 move)
{
    v->x += move.x;
    v->y += move.y;
    v->z += move.z;
}

void vec3_scale(vec3* v, float scale)
{
    v->x *= scale;
    v->y *= scale;
    v->z *= scale;
}
