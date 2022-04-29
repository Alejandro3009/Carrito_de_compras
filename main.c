#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Map.h"
#include "list.h"

typedef struct {
    char nombre[128];
    char marca[64];
    char tipo[64];
    int stock;
    int precio;
} Producto;

typedef struct {
    Map *mapaNombre;
    Map *mapaTipo;
    Map *mapaMarca;
    Map *mapaCarritos;
} MapasGlobales;

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

int is_equal(void *key1, void *key2)
{
    if (strcmp(key1, key2) == 0) return 1;
    return 0;
}

int lower_than(void *key1, void *key2)
{
    if (strcmp(key1, key2) < 0) return 1;
    return 0;
}

int main()
{
    MapasGlobales *mapas = (MapasGlobales *) malloc (sizeof(MapasGlobales));
    mapas->mapaNombre = createMap(is_equal);
    mapas->mapaTipo = createMap(is_equal);
    mapas->mapaMarca = createMap(is_equal);
    mapas->mapaCarritos = createMap(is_equal);

    setSortFunction(mapas->mapaNombre, lower_than);
    setSortFunction(mapas->mapaTipo, lower_than);
    setSortFunction(mapas->mapaMarca, lower_than);

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
        if(opcion == 8) menuAgregarACarrito(mapas);
        if(opcion == 9) menuEliminarCarrito(mapas);
        if(opcion == 10) menuComprar(mapas);
        if(opcion == 11) menuMostrarCarritos(mapas);
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

    List *busqueda = searchMap(mapas->mapaTipo, producto->tipo);
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
    printf("%i,",producto->stock);
    printf("%i\n",producto->precio);
}

Producto *crearProducto(char *nombre, char *marca, char *tipo, char *stock, char *precio)
{
    Producto *producto = (Producto*) malloc (sizeof(Producto));

    strcpy(producto->nombre, nombre);
    strcpy(producto->marca, marca);
    strcpy(producto->tipo, tipo);

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
        
        fprintf(fp, "%s,%s,%s,%i,%i", producto->nombre, producto->marca, producto->tipo, producto->stock,
                                        producto->precio);
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
    printf("Por favor ingrese el tipo de producto que esta buscando");

    char linea[512];
    getchar();
    scanf("%99[^\n]", linea);
    List *lista = searchMap(mapas->mapaTipo,linea);
    Producto *producto = firstList(lista);

    if(producto != NULL){
        while(producto != NULL)
        {
            mostrarProducto(producto);
            
            producto = nextList(lista);
        }
    }
    else printf("El tipo de producto que usted esta buscando no existe");
    esperarEnter();
}

void menuBuscarMarca(MapasGlobales *mapas)
{
    printf("Introduzca la marca que desea buscar: ");

    char linea[512];
    getchar();
    scanf("%99[^\n]", linea);
    List *lista = searchMap(mapas->mapaMarca,linea);
    Producto *producto = firstList(lista);

    if(producto != NULL){
        while(producto != NULL)
        {
            mostrarProducto(producto);
            
            producto = nextList(lista);
        }
    }
    else printf("La marca del producto que usted esta buscando no existe");
    esperarEnter();
}

void menuBuscarNombre(MapasGlobales *mapas)
{
    printf("Introduzca el nombre del producto: ");

    char linea[512];
    getchar();
    scanf("%99[^\n]", linea);

    Producto *producto = searchMap(mapas->mapaNombre,linea);

    if(producto != NULL){
        mostrarProducto(producto);
    }
    else printf("El producto que usted busca no existe.\n");

    esperarEnter();
}

void menuMostrarProductos(MapasGlobales *mapas) //f7
{
    Map *mapa = mapas->mapaNombre;
    Producto *aux = firstMap(mapa);
    while(aux){
        mostrarProducto(aux);
        aux = nextMap(mapa);
    }
    esperarEnter();
}

void menuAgregarACarrito(MapasGlobales *mapas)
{
    //hay que entregar struct producto, no lista carritos
    //Map *mapa = mapas->mapaNombre;
    int flag = 0;
    char *linea[512];
    printf("Ingrese el nombre del producto a ingresar");
    scanf("%s",&linea);

    while(mapas->mapaNombre != NULL){
        if(strcmp(mapas->mapaNombre,linea) == 0){
            pushBack(carritos,mapas->mapaNombre);
            flag = 1;
            break;
        }
        else{
            nextMap(mapas->mapaNombre);
        }
    }
    if (flag == 1){
        printf("Se ha agregado un producto al carrito con exito");
    }
    else{
        printf("No se encontro el producto deseado");
    }
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