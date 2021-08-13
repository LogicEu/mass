#include <fract.h>

/********************************************
 -> floating point functions and utilities <- 
********************************************/

float signf(float f)
{
    return (float)((f >= 0.0f) - (f < 0.0f));
}

float minf(float a, float b)
{
    return a * (float)(a <= b) + b * (float)(b < a);
}

float maxf(float a, float b)
{
    return a * (float)(a >= b) + b * (float)(b > a);
}

float clampf(float val, float min, float max)
{
    float t = (val < min) * min + (val >= min) * val;
    return (t > max) * max + (t <= max) * t;
}

float lerpf(float a, float b, float t)
{
    return (a * (1.0 - t)) + (b * t);
}

float smooth_lerpf(float a, float b, float t)
{
    return lerpf(a, b, t * t * (3 - 2 * t));
}

float inverse_lerpf(float a, float b, float val)
{
    return (val - a) / (b - a);
}

float remapf(float input_a, float input_b, float out_a, float out_b, float val)
{
    float t = inverse_lerpf(input_a, input_b, val);
    return lerpf(out_a, out_b, t);
}

float smoothstep(float a, float b, float x) 
{
    x = clampf((x - a) / (b - a), 0.0f, 1.0f);
    return x * x * (3 - 2 * x);
}

float fast_inverse_sqrt(float num) // Viva Quake forever!
{
	float x = num * 0.5;
	float y = num;
	long i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float *)&i;
	y = y * (1.5 - (x * y * y));
    y = y * (1.5 - (x * y * y));
	return y;
}

float faster_inverse_sqrt(float num)
{
	float x = num * 0.5;
	float y = num;
	long i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float *)&i;
	y = y * (1.5 - (x * y * y));
    //y = y * (1.5 - (x * y * y));
	return y;
}

float fast_sqrt(float num)
{
    return num * fast_inverse_sqrt(num);
}

float faster_sqrt(float num)
{
    return num * faster_inverse_sqrt(num);
}

/******************************************************
 -> floating point angles and vector transformations <- 
******************************************************/

float rad_to_deg(float rad) 
{
    return rad * (180.0f / M_PI);
}

float deg_to_rad(float deg)
{
    return deg / (180.0f / M_PI);
}

float vec2_to_rad(vec2 v)
{
    return atan2(v.y, v.x);
}

vec2 rad_to_vec2(float rad)
{
    vec2 ret = {cosf(rad), sinf(rad)};
    return ret;
}

float vec2_2_rad(vec2 v1, vec2 v2)
{
    return atan2(v1.y - v2.y, v1.x - v2.x);
}
