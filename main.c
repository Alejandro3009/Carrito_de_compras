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

void menuImportar();
void menuExportar();
void menuAgregar();
void menuBuscarTipo();
void menuBuscarMarca();
void menuBuscarNombre();
void menuMostrarProductos();
void menuAgregarACarrito();
void menuEliminarCarrito();
void menuComprar();
void menuMostrarCarritos();

int main()
{
    int opcion;

    scanf("%i", &opcion);

    printf("1. importar archivo");
    printf("2. exportar archivo");
    printf("3. agregar producto");
    printf("4. buscar tipo de producto");
    printf("5. buscar productos de marca");
    printf("6. buscar productos por nombre");
    printf("7. mostrar todos los productos");
    printf("8. agregar producto al carrito");
    printf("9. eliminar carrito");
    printf("10. concretar compra del carrito");
    printf("11. mostrar todos los carritos");
    printf("0. salir");

    while(1)
    {
        if(opcion == 0)break;
        if(opcion == 1)
        if(opcion == 2)
        if(opcion == 3)
        if(opcion == 4)
        if(opcion == 5)
        if(opcion == 6)
        if(opcion == 7)
        if(opcion == 8)
        if(opcion == 9)
        if(opcion == 10)
        if(opcion == 11)
    }
    
    return 0;
}
