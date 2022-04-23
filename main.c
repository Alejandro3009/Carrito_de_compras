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

typedef struct {
    HashMap *mapaNombre;
    HashMap *mapaTipo;
    HashMap *mapaMarca;
} MapasGlobales;

typedef struct
{
    char nombre[32];
    List * productos;
} carrito;

void menuImportar(MapasGlobales *);
void menuExportar(MapasGlobales *);
void menuAgregar(MapasGlobales *);
void menuBuscarTipo(MapasGlobales *);
void menuBuscarMarca(MapasGlobales *);
void menuBuscarNombre(MapasGlobales *);
void menuMostrarProductos(MapasGlobales *);
void menuAgregarACarrito(MapasGlobales *);
void menuEliminarCarrito(MapasGlobales *);
void menuComprar(MapasGlobales *);
void menuMostrarCarritos(MapasGlobales *);

int main()
{
    MapasGlobales *mapas = (MapasGlobales *) malloc (sizeof(MapasGlobales));
    mapas->mapaNombre = createMap(20);
    mapas->mapaTipo = createMap(20);
    mapas->mapaMarca = createMap(20);

    printf("    |  Menu  |\nPara elegir una opción introduzca el número correspondiente:\n");

    printf("  1. Importar archivo\n");
    printf("  2. Exportar archivo\n");
    printf("  3. Agregar producto\n");
    printf("  4. Buscar tipo de producto\n");
    printf("  5. Buscar productos por marca\n");
    printf("  6. Buscar productos por nombre\n");
    printf("  7. Mostrar todos los productos\n");
    printf("  8. Agregar producto al carrito\n");
    printf("  9. Eliminar carrito\n");
    printf("  10. Concretar compra del carrito\n");
    printf("  11. Mostrar todos los carritos\n");
    printf("  0. Salir\n");
    
    int opcion;
    scanf("%i", &opcion);

    while(1)
    {
        if(opcion == 0) break;
        if(opcion == 1) menuImportar(mapas);
        if(opcion == 2) menuExportar(mapas);
        if(opcion == 3) menuAgregar(mapas);
        if(opcion == 4) menuBuscarTipo(mapas);
        if(opcion == 5) menuBuscarMarca(mapas);
        if(opcion == 6) menuBuscarTipo(mapas);
        if(opcion == 7) menuImportar(mapas);
        if(opcion == 8) menuImportar(mapas);
        if(opcion == 9) menuImportar(mapas);
        if(opcion == 10) menuImportar(mapas);
        if(opcion == 11) menuImportar(mapas);
    }
    
    return 0;
}

void menuImportar(MapasGlobales *mapas)
{

}

void menuExportar(MapasGlobales *mapas)
{

}

void menuAgregar(MapasGlobales *mapas)
{

}

void menuBuscarTipo(MapasGlobales *mapas)
{

}

void menuBuscarMarca(MapasGlobales *mapas)
{

}

void menuBuscarNombre(MapasGlobales *mapas)
{

}

void menuMostrarProductos(MapasGlobales *mapas)
{

}

void menuAgregarACarrito(MapasGlobales *mapas)
{

}

void menuEliminarCarrito(MapasGlobales *mapas)
{

}

void menuComprar(MapasGlobales *mapas)
{

}

void menuMostrarCarritos(MapasGlobales *mapas)
{

}