#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Map.h"
#include "list.h"

typedef struct {
    char nombre[32];
    char marca[16];
    char tipo[16];
    unsigned int stock;
    unsigned int precio;
} Producto;

typedef struct {
    Map *mapaNombre;
    Map *mapaTipo;
    Map *mapaMarca;
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
void menuAgregarACarrito(MapasGlobales *, List *);
void menuEliminarCarrito(List *);
void menuComprar(MapasGlobales *, List *);
void menuMostrarCarritos(List *);

int main()
{
    MapasGlobales *mapas = (MapasGlobales *) malloc (sizeof(MapasGlobales));
    mapas->mapaNombre = createMap(20);
    mapas->mapaTipo = createMap(20);
    mapas->mapaMarca = createMap(20);

    List *carritos = createList();

    while (1)
    {
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

        if(opcion == 0) break;
        if(opcion == 1) menuImportar(mapas);
        if(opcion == 2) menuExportar(mapas);
        if(opcion == 3) menuAgregar(mapas);
        if(opcion == 4) menuBuscarTipo(mapas);
        if(opcion == 5) menuBuscarMarca(mapas);
        if(opcion == 6) menuBuscarNombre(mapas);
        if(opcion == 7) menuMostrarProductos(mapas);
        if(opcion == 8) menuAgregarACarrito(mapas, carritos);
        if(opcion == 9) menuEliminarCarrito(carritos);
        if(opcion == 10) menuComprar(mapas, carritos);
        if(opcion == 11) menuMostrarCarritos(carritos);
    }
    
    return 0;
}

void esperarEnter()
{
    printf("Presione ENTER para continuar\n");
    getchar();getchar();
}

void insertarMapas(MapasGlobales *mapas, Producto *producto)
{
    insertMap(mapas->mapaNombre, producto->nombre, producto);

    Producto *busqueda = searchMap(mapas->mapaTipo, producto->tipo);
    if (!busqueda)
    {
        List *list = createList();
        insertMap(mapas->mapaTipo, producto->tipo, list);
        pushBack(list, producto);
    }
    else pushBack(busqueda, producto);

    busqueda = searchMap(mapas->mapaMarca, producto->marca);
    if (!busqueda)
    {
        List *list = createList();
        insertMap(mapas->mapaMarca, producto->marca, list);
        pushBack(list, producto);
    }
    else pushBack(busqueda, producto);
}

void mostrarProducto(Producto *producto){
    printf("%s,",producto->nombre);
    printf("%s,",producto->marca);
    printf("%s,",producto->tipo);
    printf("%s,",producto->stock);
    printf("%s\n",producto->precio);
}

Producto *crearProducto(char *nombre, char *tipo, char *marca, char *stock, char *precio)
{
    Producto *producto = (Producto*) malloc (sizeof(Producto));

    strcpy(producto->nombre, nombre);
    strcpy(producto->tipo, tipo);
    strcpy(producto->marca, marca);

    producto->stock = atoi(stock); // convierte string a int
    producto->precio = atoi(precio);
}

void menuImportar(MapasGlobales *mapas)
{
    char nombreArchivo[64];
    printf("Ingrese el nombre del archivo CSV: ");
    scanf("%s", &nombreArchivo);

    FILE *fp = fopen(nombreArchivo, "r");
    char *extension = strrchr(nombreArchivo, '.'); // retorna la posición del ultimo '.'
    if (strcmp(extension, ".csv") != 0 || !fp)
    {
        printf("El archivo introducido no es válido.\n");
        esperarEnter();
        return;
    }

    char linea[1024];
    while(1){
        fgets(linea, 1023, fp);
        if (feof(fp)) break;
        char *nombre = strtok(linea, ",\n");
        char *marca = strtok(NULL, ",\n");
        char *tipo = strtok(NULL, ",\n");
        char *stock = strtok(NULL, ",\n");
        char *precio = strtok(NULL, ",\n");
        Producto *producto = crearProducto(nombre, marca, tipo, stock, precio);
        insertarMapas(mapas,producto);
    }
    printf("Archivo importado.\n");
    fclose(fp);
    esperarEnter();
    return;
}

void menuExportar(MapasGlobales *mapas) {
    char nombreArchivo[64];

    printf("Escriba el nombre del archivo a exportar: ");
    scanf("%s",&nombreArchivo);

    FILE *fp = fopen(nombreArchivo,"w");
    Producto *aux = firstMap(mapas->mapaNombre);
    Producto *producto = aux;
    
    while(1){
        if(aux == NULL)break;
        
        fputs(producto->nombre, fp);
        fputs(",", fp);
        fputs(producto->marca, fp);
        fputs(",", fp);
        fputs(producto->tipo, fp);
        fputs(",", fp);
        fputs(producto->stock, fp);
        fputs(",", fp);
        fputs(producto->precio, fp);   

        aux = nextMap(mapas->mapaNombre);
        producto = aux;

        fputs("\n", fp);
    }
    fclose(fp);
    esperarEnter();
    return;
}

void menuAgregar(MapasGlobales *mapas)
{
    printf("Introduzca el producto que desea agregar utilizando el siguiente formato:\n");
    printf("\'Nombre,Tipo,Marca,Stock,Precio\'\n");

    char linea[512];
    
    getchar(); // elimina el buffer
    scanf("%99[^\n]", linea); // lee todo hasta encontrar un \n
    char *nombre = strtok(linea, ",\n");
    char *marca = strtok(NULL, ",\n");
    char *tipo = strtok(NULL, ",\n");
    char *stock = strtok(NULL, ",\n");
    char *precio = strtok(NULL, ",\n");

    Producto *busqueda = searchMap(mapas->mapaNombre, nombre);
    if (busqueda) // si se encontró un producto
    {
        int stockInt = atoi(stock);

        busqueda->stock += stockInt;

        printf("Este producto ya existe en la lista, se aumentó el stock\n");
        esperarEnter();
        return;
    }

    Producto *producto = crearProducto(nombre, marca, tipo, stock, precio);

    insertarMapas(mapas, producto);

    printf("Producto añadido exitosamente.\n");
    esperarEnter();
    return;
}

void menuBuscarTipo(MapasGlobales *mapas)
{

}

void menuBuscarMarca(MapasGlobales *mapas)
{
    printf("Por favor ingrese el nombre del producto que esta buscando");

    char linea[512];
    scanf("%99[^\n]", linea);
    List *lista = searchMap(mapas->mapaNombre,linea);
    Producto *producto = firstList(lista);

    if(producto != NULL){
        printf("aca esta una lista de productos de la marca que ha buscado\n\n");
        while(producto != NULL)
        {
            printf("%s,", producto->nombre);
            printf("%s,", producto->marca);
            printf("%s,", producto->tipo);
            printf("%u,", producto->stock);
            printf("%u\n", producto->precio);
            
            producto = nextList(lista);
        }
    }
    else printf("La marca del producto que usted esta buscando no existe");
    getchar();getchar();
}

void menuBuscarNombre(MapasGlobales *mapas)
{
    printf("Por favor ingrese el nombre del producto que esta buscando");

    char linea[512];
    scanf("%99[^\n]", linea);

    Producto *producto = searchMap(mapas->mapaNombre,linea);

    if(producto != NULL){
        mostrarProducto(producto);
    }
    else printf("El nombre del producto que usted esta buscando no existe");

    getchar();getchar();
}

void menuMostrarProductos(MapasGlobales *mapas)
{
    Map *mapa = mapas->mapaNombre;
    Producto *aux = firstMap(mapa);
    mostrarProducto(aux);
    while(nextMap(mapa)!=0){
        aux = nextMap(mapa);
        mostrarProducto(aux);
    }
}

void menuAgregarACarrito(MapasGlobales *mapas, List *carritos)
{

}

void menuEliminarCarrito(List *carritos)
{

}

void menuComprar(MapasGlobales *mapas, List *carritos)
{

}

void menuMostrarCarritos(List *carritos)
{

}