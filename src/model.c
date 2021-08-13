#include <mass.h>
#include <stdlib.h>

/*
-------------------------
 -> 3D Model Structure <- 
-------------------------
*/

model_t* model_load(const char* path)
{
    model_t* model = (model_t*)malloc(sizeof(model_t));
    model->id = 0;
    model->mesh = mesh_load(path);
    if (model->mesh->vertices != NULL) model->size = model->mesh->vertices->size;
    else model->size = 0;
    return model;
}

void model_free(model_t* model)
{
    mesh_free(model->mesh);
    free(model);
}