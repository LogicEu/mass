#ifndef FRACT_MATH_H
#define FRACT_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

/*====================================================
>>>>>>>>>>>>>>  FRACT MATH LIBRARY   >>>>>>>>>>>>>>>>
ANSI C99 header only math library for game-development
fully compatible with OpenGL implementation of vector 
and matrix structures and operations.
=========================================== @eulogic*/

#include <math.h>
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif
    
typedef struct {
    float x, y;
} vec2;

typedef struct {
    float x, y, z;
} vec3;

typedef struct { 
    float x, y, z, w;
} vec4;

typedef struct { 
    int x, y;
} ivec2;

typedef struct { 
    int x, y, z;
} ivec3;

typedef struct { 
    int x, y, z, w;
} ivec4;

typedef struct { 
    float data[2][2];
} mat2;

typedef struct { 
    float data[3][3];
} mat3;

typedef struct { 
    float data[4][4];
} mat4;

#define _absf(f) ((f) * ((f) >= 0.0) - (f) * ((f) < 0.0))
#define _signf(f) (((f) >= 0.0) - ((f) < 0.0))
#define _minf(a, b) ((a) * ((a) <= (b)) + (b) * ((b) < (a)))
#define _maxf(a, b) ((a) * ((a) >= (b)) + (b) * ((b) > (a)))
#define _lerpf(a, b, t) (((a) * (1.0 - (t))) + ((b) * (t)))
#define _smooth_lerpf(a, b, t) _lerpf(a, b, t * t * (3 - 2 * t))
#define _inverse_lerpf(a, v, val) (((val) - (a)) / ((b) - (a)))

#define _vec2_new(x, y) {x, y}
#define _vec2_uni(f) {f, f}
#define _vec2_cmp(a, b) (a.x == b.x && a.y == b.y)
#define _vec2_unicmp(v, f) (v.x == f && v.y == f)
#define _vec2_op(a, op, b) _vec2_new(a.x op b.x, a.y op b.y)
#define _vec2_fop(v, fop, f) _vec2_new(v.x fop f, v.y fop f)
#define _vec2_add(a, b) _vec2_op(a, +, b)
#define _vec2_sub(a, b) _vec2_op(a, -, b)
#define _vec2_prod(a, b) _vec2_op(a, *, b)
#define _vec2_mult(v, f) _vec2_fop(v, *, f)
#define _vec2_div(v, f) _vec2_fop(v, /, f)
#define _vec2_neg(v) _vec2_new(-v.x, -v.y)
#define _vec2_dot(a, b) (a.x * b.x + a.y * b.y)
#define _vec2_sqmag(v) _vec2_dot(v, v)
#define _vec2_mag(v) sqrtf(_vec2_sqmag(v))
#define _vec2_normal(v) _vec2_mult(v, 1.0 / _vec2_mag(v))
#define _vec2_cross(a, b) {a.y - b.y, a.x - b.x};

#define _vec3_new(x, y, z) {x, y, z}
#define _vec3_uni(f) {f, f, f}
#define _vec3_cmp(a, b) (a.x == b.x && a.y == b.y && a.z == b.z)
#define _vec3_unicmp(v, f) (v.x == f && v.y == f && v.z == f)
#define _vec3_op(a, op, b) _vec3_new(a.x op b.x, a.y op b.y, a.z op b.z)
#define _vec3_fop(v, fop, f) _vec3_new(v.x fop f, v.y fop f, v.z fop f)
#define _vec3_add(a, b) _vec3_op(a, +, b)
#define _vec3_sub(a, b) _vec3_op(a, -, b)
#define _vec3_prod(a, b) _vec3_op(a, *, b)
#define _vec3_mult(v, f) _vec3_fop(v, *, f)
#define _vec3_div(v, f) _vec3_fop(v, /, f)
#define _vec3_neg(v) _vec3_new(-v.x, -v.y, -v.z)
#define _vec3_dot(a, b) (a.x * b.x + a.y * b.y + a.z * b.z)
#define _vec3_sqmag(v) _vec3_dot(v, v)
#define _vec3_mag(v) sqrtf(_vec3_sqmag(v))
#define _vec3_normal(v) _vec3_mult(v, 1.0 / _vec3_mag(v))
#define _vec3_cross(a, b) {a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y}

#define _vec4_new(x, y, z, w) {x, y, z, w}
#define _vec4_uni(f) {f, f, f, f}
#define _vec4_cmp(a, b) (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w)
#define _vec4_unicmp(v, f) (v.x == f && v.y == f && v.z == f && v.w == f)
#define _vec4_op(a, op, b) _vec4_new(a.x op b.x, a.y op b.y, a.z op b.z, a.w op b.w)
#define _vec4_fop(v, fop, f) _vec4_new(v.x fop f, v.y fop f, v.z fop f, v.w fop f)
#define _vec4_add(a, b) _vec4_op(a, +, b)
#define _vec4_sub(a, b) _vec4_op(a, -, b)
#define _vec4_prod(a, b) _vec4_op(a, *, b)
#define _vec4_mult(v, f) _vec4_fop(v, *, f)
#define _vec4_div(v, f) _vec4_fop(v, /, f)
#define _vec4_neg(v) _vec4_new(-v.x, -v.y, -v.z)
#define _vec4_dot(a, b) (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w)
#define _vec4_sqmag(v) _vec4_dot(v, v)
#define _vec4_mag(v) sqrtf(_vec4_sqmag(v))
#define _vec4_normal(v) _vec4_mult(v, 1.0 / _vec4_mag(v))

/*
-----------------------------------
 -> Math Utilities and Functions <- 
-----------------------------------
*/

/*********************************************
 -> OS independent pseudo random algorithms <- 
*********************************************/

void rand_seed(unsigned int seed);
unsigned int rand_uint(unsigned int num);
unsigned int rand_num();
unsigned int rand_next();
float randf_norm();
float randf_signed();

/********************************************
 -> floating point functions and utilities <- 
********************************************/

float absf(float f);
float signf(float f);
float minf(float a, float b);
float maxf(float a, float b);
float clampf(float val, float min, float max);
float lerpf(float a, float b, float t);
float smooth_lerpf(float a, float b, float t);
float inverse_lerpf(float a, float b, float val);
float remapf(float input_a, float input_b, float out_a, float out_b, float val);
float smoothstep(float a, float b, float x);
float fast_inverse_sqrt(float num);
float faster_inverse_sqrt(float num);
float fast_sqrt(float num);
float faster_sqrt(float num);

/******************************************************
 -> floating point angles and vector transformations <- 
******************************************************/

float rad_to_deg(float rad);
float deg_to_rad(float deg);
float vec2_to_rad(vec2 v);
vec2 rad_to_vec2(float rad);

/**************************************
 -> 2D perlin noise useful functions <- 
**************************************/

float noise2d(float x, float y, int seed);
float perlin2d(float x, float y, float freq, int depth, int seed);

/*
---------------------------------------
 -> Vector Structures and Operations <- 
---------------------------------------
*/

/*******************************************
 -> two dimensional floating point vector <- 
*******************************************/

vec2 vec2_uni(float f);
vec2 vec2_new(float x, float y);
int vec2_cmp(vec2 v1, vec2 v2);
vec2 vec2_add(vec2 v1, vec2 v2);
vec2 vec2_sub(vec2 v1, vec2 v2);
vec2 vec2_neg(vec2 v);
vec2 vec2_mult(vec2 v, float f);
vec2 vec2_div(vec2 v, float f);
float vec2_mag(vec2 v);
float vec2_sqmag(vec2 v);
float vec2_sqdist(vec2 v1, vec2 v2);
float vec2_dist(vec2 v1, vec2 v2);
float vec2_dot(vec2 v1, vec2 v2);
vec2 vec2_cross(vec2 v1, vec2 v2);
vec2 vec2_norm(vec2 v);
vec2 vec2_normf(vec2 v);
vec2 vec2_normal(vec2 v);
vec2 vec2_prod(vec2 a, vec2 b);
void vec2_move(vec2* v, vec2 move);
void vec2_scale(vec2* v, float scale);

vec2 vec2_rotate(vec2 src, float rads);
vec2 vec2_rotate_around(vec2 src, vec2 center, float rads);
vec2 vec2_rotate_and_scale_around(vec2 src, vec2 center, float rads, float scale);
vec2 vec2_scale_around(vec2 src, vec2 center, float scale);

/*********************************************
 -> three dimensional floating point vector <- 
*********************************************/

vec3 vec3_uni(float f);
vec3 vec3_new(float x, float y, float z);
int vec3_cmp(vec3 v1, vec3 v2);
vec3 vec3_add(vec3 v1, vec3 v2);
vec3 vec3_sub(vec3 v1, vec3 v2);
vec3 vec3_neg(vec3 v);
vec3 vec3_mult(vec3 v, float f);
vec3 vec3_div(vec3 v, float f);
float vec3_mag(vec3 v);
float vec3_sqmag(vec3 v);
float vec3_sqdist(vec3 v1, vec3 v2);
float vec3_dist(vec3 v1, vec3 v2);
float vec3_dot(vec3 v1, vec3 v2);
vec3 vec3_cross(vec3 v1, vec3 v2);
vec3 vec3_norm(vec3 v);
vec3 vec3_normf(vec3 v);
vec3 vec3_normal(vec3 v);
vec3 vec3_prod(vec3 a, vec3 b);
void vec3_move(vec3* v, vec3 move);
void vec3_scale(vec3* v, float scale);

/********************************************
 -> four dimensional floating point vector <- 
********************************************/

vec4 vec4_uni(float f);
vec4 vec4_new(float x, float y, float z, float w);
int vec4_cmp(vec4 v1, vec4 v2);
vec4 vec4_add(vec4 v1, vec4 v2);
vec4 vec4_sub(vec4 v1, vec4 v2);
vec4 vec4_neg(vec4 v);
vec4 vec4_mult(vec4 v, float f);
vec4 vec4_div(vec4 v, float f);
float vec4_mag(vec4 v);
float vec4_sqmag(vec4 v);
float vec4_sqdist(vec4 v1, vec4 v2);
float vec4_dist(vec4 v1, vec4 v2);
float vec4_dot(vec4 v1, vec4 v2);
vec4 vec4_cross(vec4 v1, vec4 v2);
vec4 vec4_norm(vec4 v);
vec4 vec4_normf(vec4 v);
vec4 vec4_normal(vec4 v);
vec4 vec4_prod(vec4 a, vec4 b);
void vec4_move(vec4* v, vec4 move);
void vec4_scale(vec4* v, float scale);

/*******************************
 -> integer vector structures <- 
*******************************/

ivec2 ivec2_new(int x, int y);
ivec2 ivec2_uni(int i);
int ivec2_cmp(ivec2 v1, ivec2 v2);

ivec3 ivec3_new(int x, int y, int z);
ivec3 ivec3_uni(int i);
int ivec3_cmp(ivec3 v1, ivec3 v2);

ivec4 ivec4_new(int x, int y, int z, int w);
ivec4 ivec4_uni(float i);
int ivec4_cmp(ivec4 v1, ivec4 v2);

/***********************************************
 -> integer and floating point vector casting <- 
***********************************************/

ivec2 vec2_to_ivec2(vec2 v);
ivec3 vec3_to_ivec3(vec3 v);
ivec4 vec4_to_ivec4(vec4 v);

vec2 ivec2_to_vec2(ivec2 v);
vec3 ivec3_to_vec3(ivec3 v);
vec4 ivec4_to_vec4(ivec4 v);

/*
---------------------------------------
 -> Matrix Structures and Operations <- 
---------------------------------------
*/

/***************************************************
 ->   lightweight stack matrix data structures <- 
***************************************************/

mat2 mat2_new();
mat2 mat2_id();
mat3 mat3_new();
mat3 mat3_id();
mat4 mat4_new();
mat4 mat4_id();

/******************************************
 -> 4x4 matrix functions and operations  <- 
******************************************/

mat4 mat4_translate(mat4 mat, vec3 v);
mat4 mat4_scale(mat4 mat, vec3 v);
mat4 mat4_mult(mat4 m1, mat4 m2);
mat4 mat4_mult_vec4(mat4 mat, vec4 v);
mat4 mat4_mult_vec3(mat4 mat, vec3 v);
mat4 mat4_rot(mat4 mat, float deg, vec3 rot_axis);
mat4 mat4_perspective_RH(float fov, float aspect, float near, float far);
mat4 mat4_perspective_LH(float fov, float aspect, float near, float far);
mat4 mat4_perspective(float fov, float aspect, float near, float far);
mat4 mat4_ortho(float left, float right, float bottom, float top);
mat4 mat4_look_at_LH(vec3 eye_position, vec3 eye_direction, vec3 eye_up);
mat4 mat4_look_at_RH(vec3 eye_position, vec3 eye_direction, vec3 eye_up);
mat4 mat4_look_at(vec3 eye_position, vec3 eye_direction, vec3 eye_up);
mat4 mat4_model(vec3 translation, vec3 scale, vec3 rot_axis, float rot_degs);

/*
-----------------------
 -> Print Data Types <- 
-----------------------
*/

/***************************
 -> formating to stdout   <- 
***************************/

void ivec2_print(ivec2 v);
void ivec3_print(ivec3 v);
void ivec4_print(ivec4 v);
void vec2_print(vec2 v);
void vec3_print(vec3 v);
void vec4_print(vec4 v);
void mat2_print(mat2 m);
void mat3_print(mat3 m);
void mat4_print(mat4 m);

#ifdef __cplusplus
}
#endif
#endif
