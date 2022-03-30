#include <mass.h>
#include <stdlib.h>
#include <string.h>

/*
-----------------------------
 -> Useful Array Extension <- 
-----------------------------
*/

array_t array_map(array_t* buffer)
{
    array_t map = array_create(buffer->bytes);
    char* p1 = buffer->data;
    for (char* end = p1 + buffer->size * buffer->bytes; p1 != end; p1 += buffer->bytes) {
        unsigned int found = 0;
        char* p2 = map.data;
        for (char* end = p2 + map.size * map.bytes; p2 != end; p2 += map.bytes) {
            if (!memcmp(p1, p2, buffer->bytes)){
                found++;
                break;
            }
        }
        if (!found) array_push(&map, p1);
    }
    array_cut(&map);
    return map;
}

void array_map_indexed(array_t* buffer, array_t* map, array_t* indices)
{
    if (!buffer) return;

    *map = array_reserve(buffer->size, buffer->bytes);
    *indices = array_reserve(buffer->size, sizeof(unsigned int));
    
    char* p1 = buffer->data;
    for (char* end = p1 + buffer->size * buffer->bytes; p1 != end; p1 += buffer->bytes) {
        unsigned int j = 0, k = map->size, found = 0;
        char* p2 = map->data;
        for (char* end = p2 + map->size * map->bytes; p2 != end; p2 += map->bytes) {
            if (!memcmp(p1, p2, buffer->bytes)){
                found++;
                k = j;
                break;
            }
            j++;
        }
        if (!found) array_push(map, p1);
        array_push(indices, &k);
    }
    array_cut(map);
}

void array_push_indexed(array_t* map, array_t* indices, void* data)
{
    if (!indices || !map) return;

    unsigned int j = 0, k = map->size, found = 0;
    char* p = map->data;
    for (char* end = p + map->size * map->bytes; p != end; p += map->bytes) {
        if (!memcmp(p, data, map->bytes)) {
            k = j;
            found++;
        }
        j++;
    }
    if (!found) array_push(map, data);
    array_push(indices, &k);
}
