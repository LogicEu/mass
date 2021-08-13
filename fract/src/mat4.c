#include <fract.h>

/******************************************
 -> 4x4 matrix functions and operations  <- 
******************************************/

mat4 mat4_translate(mat4 mat, vec3 v)
{
    mat4 m = mat;
    m.data[3][0] = v.x;
    m.data[3][1] = v.y;
    m.data[3][2] = v.z;
    return m;
}

mat4 mat4_scale(mat4 mat, vec3 v)
{
    mat4 s = mat4_new();
    s.data[0][0] = v.x;
    s.data[1][1] = v.y;
    s.data[2][2] = v.z;
    s.data[3][3] = 1.0f;
    mat4 m = mat4_mult(s, mat);
    return m;
}

mat4 mat4_mult(mat4 m1, mat4 m2)
{
    mat4 m;
    m.data[0][0] = m1.data[0][0] * m2.data[0][0] + m1.data[1][0] * m2.data[0][1] + m1.data[2][0] * m2.data[0][2] + m1.data[3][0] * m2.data[0][3];
    m.data[0][1] = m1.data[0][1] * m2.data[0][0] + m1.data[1][1] * m2.data[0][1] + m1.data[2][1] * m2.data[0][2] + m1.data[3][1] * m2.data[0][3];
    m.data[0][2] = m1.data[0][2] * m2.data[0][0] + m1.data[1][2] * m2.data[0][1] + m1.data[2][2] * m2.data[0][2] + m1.data[3][2] * m2.data[0][3];
    m.data[0][3] = m1.data[0][3] * m2.data[0][0] + m1.data[1][3] * m2.data[0][1] + m1.data[2][3] * m2.data[0][2] + m1.data[3][3] * m2.data[0][3];
    m.data[1][0] = m1.data[0][0] * m2.data[1][0] + m1.data[1][0] * m2.data[1][1] + m1.data[2][0] * m2.data[1][2] + m1.data[3][0] * m2.data[1][3];
    m.data[1][1] = m1.data[0][1] * m2.data[1][0] + m1.data[1][1] * m2.data[1][1] + m1.data[2][1] * m2.data[1][2] + m1.data[3][1] * m2.data[1][3];
    m.data[1][2] = m1.data[0][2] * m2.data[1][0] + m1.data[1][2] * m2.data[1][1] + m1.data[2][2] * m2.data[1][2] + m1.data[3][2] * m2.data[1][3];
    m.data[1][3] = m1.data[0][3] * m2.data[1][0] + m1.data[1][3] * m2.data[1][1] + m1.data[2][3] * m2.data[1][2] + m1.data[3][3] * m2.data[1][3];
    m.data[2][0] = m1.data[0][0] * m2.data[2][0] + m1.data[1][0] * m2.data[2][1] + m1.data[2][0] * m2.data[2][2] + m1.data[3][0] * m2.data[2][3];
    m.data[2][1] = m1.data[0][1] * m2.data[2][0] + m1.data[1][1] * m2.data[2][1] + m1.data[2][1] * m2.data[2][2] + m1.data[3][1] * m2.data[2][3];
    m.data[2][2] = m1.data[0][2] * m2.data[2][0] + m1.data[1][2] * m2.data[2][1] + m1.data[2][2] * m2.data[2][2] + m1.data[3][2] * m2.data[2][3];
    m.data[2][3] = m1.data[0][3] * m2.data[2][0] + m1.data[1][3] * m2.data[2][1] + m1.data[2][3] * m2.data[2][2] + m1.data[3][3] * m2.data[2][3];
    m.data[3][0] = m1.data[0][0] * m2.data[3][0] + m1.data[1][0] * m2.data[3][1] + m1.data[2][0] * m2.data[3][2] + m1.data[3][0] * m2.data[3][3];
    m.data[3][1] = m1.data[0][1] * m2.data[3][0] + m1.data[1][1] * m2.data[3][1] + m1.data[2][1] * m2.data[3][2] + m1.data[3][1] * m2.data[3][3];
    m.data[3][2] = m1.data[0][2] * m2.data[3][0] + m1.data[1][2] * m2.data[3][1] + m1.data[2][2] * m2.data[3][2] + m1.data[3][2] * m2.data[3][3];
    m.data[3][3] = m1.data[0][3] * m2.data[3][0] + m1.data[1][3] * m2.data[3][1] + m1.data[2][3] * m2.data[3][2] + m1.data[3][3] * m2.data[3][3];
    return m;
}

mat4 mat4_mult_vec4(mat4 mat, vec4 v)
{
    mat4 m = mat;
    m.data[0][0] *= v.x;
    m.data[0][1] *= v.x;
    m.data[0][2] *= v.x;
    m.data[0][3] *= v.x;
    m.data[1][0] *= v.y;
    m.data[1][1] *= v.y;
    m.data[1][2] *= v.y;
    m.data[1][3] *= v.y;
    m.data[2][0] *= v.z;
    m.data[2][1] *= v.z;
    m.data[2][2] *= v.z;
    m.data[2][3] *= v.z;
    m.data[3][0] *= v.w;
    m.data[3][1] *= v.w;
    m.data[3][2] *= v.w;
    m.data[3][3] *= v.w;
    return m;
}

mat4 mat4_mult_vec3(mat4 mat, vec3 v)
{
    mat4 m = mat;
    m.data[0][0] *= v.x;
    m.data[0][1] *= v.x;
    m.data[0][2] *= v.x;
    m.data[0][3] *= v.x;
    m.data[1][0] *= v.y;
    m.data[1][1] *= v.y;
    m.data[1][2] *= v.y;
    m.data[1][3] *= v.y;
    m.data[2][0] *= v.z;
    m.data[2][1] *= v.z;
    m.data[2][2] *= v.z;
    m.data[2][3] *= v.z;
    return m;
}

mat4 mat4_rot(mat4 mat, float deg, vec3 rot_axis)
{
    float c = cosf(deg);
    float s = sinf(deg);

    vec3 axis = vec3_norm(rot_axis);
	vec3 temp = vec3_mult(axis, 1.0f - c);

    mat4 rot = mat4_new();
    rot.data[0][0] = c + temp.x * axis.x;
    rot.data[0][1] = temp.x * axis.y + s * axis.z;
    rot.data[0][2] = temp.x * axis.z - s * axis.y;
    rot.data[1][0] = temp.y * axis.x - s * axis.z;
    rot.data[1][1] = c + temp.y * axis.y;
    rot.data[1][2] = temp.y * axis.z + s * axis.x;
    rot.data[2][0] = temp.z * axis.x + s * axis.y;
    rot.data[2][1] = temp.z * axis.y - s * axis.x;
    rot.data[2][2] = c + temp.z * axis.z;

    mat4 m = mat4_new();
    m.data[0][0] = mat.data[0][0] * rot.data[0][0] + mat.data[1][0] * rot.data[0][1] + mat.data[2][0] * rot.data[0][2];
    m.data[0][1] = mat.data[0][1] * rot.data[0][0] + mat.data[1][1] * rot.data[0][1] + mat.data[2][1] * rot.data[0][2];
    m.data[0][2] = mat.data[0][2] * rot.data[0][0] + mat.data[1][2] * rot.data[0][1] + mat.data[2][2] * rot.data[0][2];
    m.data[1][0] = mat.data[0][0] * rot.data[1][0] + mat.data[1][0] * rot.data[1][1] + mat.data[2][0] * rot.data[1][2];
    m.data[1][1] = mat.data[0][1] * rot.data[1][0] + mat.data[1][1] * rot.data[1][1] + mat.data[2][1] * rot.data[1][2];
    m.data[1][2] = mat.data[0][2] * rot.data[1][0] + mat.data[1][2] * rot.data[1][1] + mat.data[2][2] * rot.data[1][2];
    m.data[2][0] = mat.data[0][0] * rot.data[2][0] + mat.data[1][0] * rot.data[2][1] + mat.data[2][0] * rot.data[2][2];
    m.data[2][1] = mat.data[0][1] * rot.data[2][0] + mat.data[1][1] * rot.data[2][1] + mat.data[2][1] * rot.data[2][2];
    m.data[2][2] = mat.data[0][2] * rot.data[2][0] + mat.data[1][2] * rot.data[2][1] + mat.data[2][2] * rot.data[2][2];
    m.data[3][0] = mat.data[3][0];
    m.data[3][1] = mat.data[3][1];
    m.data[3][2] = mat.data[3][2];
    m.data[3][3] = mat.data[3][3];
    return m;
}

mat4 mat4_perspective_RH(float fov, float aspect, float near, float far)
{
    mat4 m = mat4_new();
    float tan_half_fov = tanf(fov / 2.0f);
    m.data[0][0] = 1.0f / (aspect * tan_half_fov);
    m.data[1][1] = 1.0f / tan_half_fov;
    m.data[2][2] = - (far + near) / (far - near);
    m.data[2][3] = - 1.0f;
    m.data[3][2] = - (2.0f * far * near) / (far - near);
    return m;
}

mat4 mat4_perspective_LH(float fov, float aspect, float near, float far)
{
    mat4 m = mat4_new();
    float tan_half_fov = tanf(fov / 2.0f);
    m.data[0][0] = 1.0f / (aspect * tan_half_fov);
    m.data[1][1] = 1.0f / tan_half_fov;
    m.data[2][2] = (far + near) / (far - near);
    m.data[2][3] = 1.0f;
    m.data[3][2] = - (2.0f * far * near) / (far - near);
    return m;
}

mat4 mat4_ortho(float left, float right, float bottom, float top)
{
    mat4 ret = mat4_id();
    ret.data[0][0] = 2.0f / (right - left);
    ret.data[1][1] = 2.0f / (top - bottom);
    ret.data[2][2] = - 1.0f;
    ret.data[3][0] = - (right + left) / (right - left);
    ret.data[3][1] = - (top + bottom) / (top - bottom);
    return ret;
}

mat4 mat4_perspective(float fov, float aspect, float near, float far)
{
    return mat4_perspective_RH(fov, aspect, near, far);
}

mat4 mat4_look_at_LH(vec3 eye_position, vec3 eye_direction, vec3 eye_up)
{
    mat4 m = mat4_new();
    vec3 f = vec3_norm(vec3_sub(eye_direction, eye_position));
    vec3 s = vec3_norm(vec3_cross(eye_up, f));
    vec3 u = vec3_cross(f, s);
    m.data[0][0] = s.x;
    m.data[1][0] = s.y;
    m.data[2][0] = s.z;
    m.data[0][1] = u.x;
    m.data[1][1] = u.y;
    m.data[2][1] = u.z;
    m.data[0][2] = f.x;
    m.data[1][2] = f.y;
    m.data[2][2] = f.z;
    m.data[3][0] = -vec3_dot(s, eye_position);
    m.data[3][1] = -vec3_dot(u, eye_position);
    m.data[3][2] = -vec3_dot(f, eye_position);
    m.data[3][3] = 1.0f;
    return m;
}

mat4 mat4_look_at_RH(vec3 eye_position, vec3 eye_direction, vec3 eye_up)
{
    mat4 m = mat4_new();
    vec3 f = vec3_norm(vec3_sub(eye_direction, eye_position));
    vec3 s = vec3_norm(vec3_cross(f, eye_up));
    vec3 u = vec3_cross(s, f);
    m.data[0][0] = s.x;
    m.data[1][0] = s.y;
    m.data[2][0] = s.z;
    m.data[0][1] = u.x;
    m.data[1][1] = u.y;
    m.data[2][1] = u.z;
    m.data[0][2] = -f.x;
    m.data[1][2] = -f.y;
    m.data[2][2] = -f.z;
    m.data[3][0] = -vec3_dot(s, eye_position);
    m.data[3][1] = -vec3_dot(u, eye_position);
    m.data[3][2] = vec3_dot(f, eye_position);
    m.data[3][3] = 1.0f;
    return m;
}

mat4 mat4_look_at(vec3 eye_position, vec3 eye_direction, vec3 eye_up)
{
    return mat4_look_at_RH(eye_position, eye_direction, eye_up);
}

mat4 mat4_model(vec3 translation, vec3 scale, vec3 rot_axis, float rot_degs)
{
    mat4 model = mat4_scale(mat4_id(), scale);
    model = mat4_rot(model, rot_degs, rot_axis);
    model = mat4_translate(model, translation);
    return model;
}