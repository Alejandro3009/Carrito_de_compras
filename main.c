#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hashmap.h"
#include "list.h"

typedef struct {
    char nombre[32];
    char marca[16];
    char tipo[16];
    unsigned int stock;
    unsigned int precio;
} Producto;