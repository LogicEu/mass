#include <fract.h>
#include <stdio.h>

/***************************
 -> formating to stdout   <- 
***************************/

void ivec2_print(ivec2 v)
{
    printf("%d, %d\n", v.x, v.y);
}

void ivec3_print(ivec3 v)
{
    printf("%d, %d, %d\n", v.x, v.y, v.z);
}

void ivec4_print(ivec4 v)
{
    printf("%d, %d, %d, %d\n", v.x, v.y, v.z, v.w);
}

void vec2_print(vec2 v)
{
    printf("%f, %f\n", v.x, v.y);
}

void vec3_print(vec3 v)
{
    printf("%f, %f, %f\n", v.x, v.y, v.z);
}

void vec4_print(vec4 v)
{
    printf("%f, %f, %f, %f\n", v.x, v.y, v.z, v.w);
}

void mat2_print(mat2 m)
{
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            printf("%f ", m.data[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

void mat3_print(mat3 m)
{
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            printf("%f ", m.data[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

void mat4_print(mat4 m)
{
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            printf("%f ", m.data[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}