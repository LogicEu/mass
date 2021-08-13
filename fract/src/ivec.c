#include <fract.h>

/*******************************
 -> integer vector structures <- 
*******************************/

ivec2 ivec2_new(int x, int y)
{
    ivec2 ret = {x, y};
    return ret;
}

ivec2 ivec2_uni(int i)
{
    ivec2 ret = {i, i};
    return ret;
}

int ivec2_cmp(ivec2 v1, ivec2 v2)
{
    return (v1.x == v2.x && v1.y == v2.y);
}

ivec3 ivec3_new(int x, int y, int z)
{
    ivec3 ret = {x, y, z};
    return ret;
}

ivec3 ivec3_uni(int i)
{
    ivec3 ret = {i, i, i};
    return ret;
}

int ivec3_cmp(ivec3 v1, ivec3 v2)
{
    return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
}

ivec4 ivec4_new(int x, int y, int z, int w)
{
    ivec4 v = {x, y, z, w};
    return v;
}

ivec4 ivec4_uni(float i)
{
    ivec4 ret = {i, i, i, i};
    return ret;
}

int ivec4_cmp(ivec4 v1, ivec4 v2)
{
    return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w);
}