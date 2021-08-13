#include <fract.h>

/***********************************************
 -> integer and floating point vector casting <- 
***********************************************/

ivec2 vec2_to_ivec2(vec2 v)
{
    ivec2 ret = {(int)v.x, (int)v.y};
    return ret;
}

ivec3 vec3_to_ivec3(vec3 v)
{
    ivec3 ret = {(int)v.x, (int)v.y, (int)v.z};
    return ret;
}

ivec4 vec4_to_ivec4(vec4 v)
{
    ivec4 ret = {(int)v.x, (int)v.y, (int)v.z, (int)v.w};
    return ret;
}

vec2 ivec2_to_vec2(ivec2 v)
{
    vec2 ret = {(float)v.x, (float)v.y};
    return ret;
}

vec3 ivec3_to_vec3(ivec3 v)
{
    vec3 ret = {(float)v.x, (float)v.y, (float)v.z};
    return ret;
}

vec4 ivec4_to_vec4(ivec4 v)
{
    vec4 ret = {(float)v.x, (float)v.y, (float)v.z, (float)v.w};
    return ret;
}