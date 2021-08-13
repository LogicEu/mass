#include <fract.h>

/**************************************************
 -> two dimensional square matrix data structure <- 
**************************************************/

mat2 mat2_new()
{
    mat2 m;
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            m.data[x][y] = 0.0f;
        }
    }
    return m;
}

mat2 mat2_id()
{
    mat2 m = mat2_new();
    for (int i = 0; i < 2; i++) {
        m.data[i][i] = 1.0f;
    }
    return m;
}

mat3 mat3_new()
{
    mat3 m;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            m.data[x][y] = 0.0f;
        }
    }
    return m;
}

mat3 mat3_id()
{
    mat3 m = mat3_new();
    for (int i = 0; i < 3; i++) {
        m.data[i][i] = 1.0f;
    }
    return m;
}

mat4 mat4_new()
{
    mat4 m;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            m.data[x][y] = 0.0f;
        }
    }
    return m;
}

mat4 mat4_id()
{
    mat4 m = mat4_new();
    for (int i = 0; i < 4; i++) {
        m.data[i][i] = 1.0f;
    }
    return m;
}