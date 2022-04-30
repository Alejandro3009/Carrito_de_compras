#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Map.h"
#include "list.h"

// struct que contiene la información de un producto
typedef struct {
    char nombre[128];
    char marca[64];
    char tipo[64];
    int stock;
    int precio;
} Producto;

// struct que contiene 3 mapas de búsqueda de productos y un mapa con los carritos
typedef struct {
    Map *mapaNombre;
    Map *mapaTipo;
    Map *mapaMarca;
    Map *mapaCarritos;
} MapasGlobales;

// struct que contiene un producto y su cantidad, para usar en carritos
typedef struct {
    Producto *producto;
    int cantidad;
} ProductoCarrito;

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

// compara 2 keys tipo string
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
        printf("        |  Menu  |\nPara elegir una opción introduzca el número correspondiente:\n");

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

// espera a que el usuario presione ENTER
void esperarEnter() 
{
    printf("Presione ENTER para continuar\n");
    getchar();getchar();
}

// inserta un producto en los 3 mapas de búsqueda
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

// imprime toda la información de un producto
void mostrarProducto(Producto *producto){
    printf("%s,",producto->nombre);
    printf("%s,",producto->marca);
    printf("%s,",producto->tipo);
    printf("%i,",producto->stock);
    printf("%i\n",producto->precio);
}

// crea un producto con los datos recibidos y lo retorna
Producto *crearProducto(char *nombre, char *marca, char *tipo, char *stock, char *precio)
{
    Producto *producto = (Producto*) malloc (sizeof(Producto));

    strcpy(producto->nombre, nombre);
    strcpy(producto->marca, marca);
    strcpy(producto->tipo, tipo);

    producto->stock = atoi(stock); // convierte string a int
    producto->precio = atoi(precio);
}

// imprime la información de un carrito
void imprimirCarrito(List *carrito)
{
    int total = 0;
    ProductoCarrito *producto;
    producto = firstList(carrito);
    while (producto)
    {
        int precio = producto->cantidad * producto->producto->precio;
        total += precio;
        printf("  %s x%i  $%i\n", producto->producto->nombre, producto->cantidad, precio);
        producto = nextList(carrito);
    }
    printf("\nTotal = $%i\n", total);
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
        char *nombre = strtok(linea, ",\n"); // separa la linea introducida por ,
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
    if (busqueda) // si se encontró un producto, sólo se aumenta el stock
    {
        int stockInt = atoi(stock);

        busqueda->stock += stockInt;

        printf("Se aumentó el stock de este producto\n");
        esperarEnter();
        return;
    }

    Producto *producto = crearProducto(nombre, marca, tipo, stock, precio);

    insertarMapas(mapas, producto); // se inserta el producto en los mapas de búsqueda

    printf("Producto añadido exitosamente.\n");
    esperarEnter();
    return;
}

void menuBuscarTipo(MapasGlobales *mapas)
{
    printf("Introduzca el tipo de producto que desea buscar: ");

    char linea[512];
    getchar();
    scanf("%99[^\n]", linea);
    List *lista = searchMap(mapas->mapaTipo,linea);
    Producto *producto = firstList(lista);

    // mapaTipo es un multimapa, su valor es una lista
    if(producto != NULL){
        while(producto != NULL)
        {
            mostrarProducto(producto);
            producto = nextList(lista);
        }
    }
    else printf("El tipo de producto que usted está buscando no existe\n");
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

    // mapaMarca es un multimapa, su valor es una lista
    if(producto != NULL){
        while(producto != NULL)
        {
            mostrarProducto(producto);
            producto = nextList(lista);
        }
    }
    else printf("La marca del producto que usted esta buscando no existe\n");
    esperarEnter();
}

void menuBuscarNombre(MapasGlobales *mapas)
{
    printf("Introduzca el nombre del producto que desea buscar: ");

    char linea[512];
    getchar();
    scanf("%99[^\n]", linea);

    Producto *producto = searchMap(mapas->mapaNombre,linea);

    if(producto != NULL){
        mostrarProducto(producto);
    }
    else printf("El nombre del producto que usted esta buscando no existe\n");
    esperarEnter();
}

void menuMostrarProductos(MapasGlobales *mapas)
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
    ProductoCarrito * productoCarrito = (ProductoCarrito*) malloc (sizeof(ProductoCarrito));
    Map *carritos = mapas->mapaCarritos;
    char linea[64];
    char *nombreCarrito = (char *) malloc (sizeof(char) * 32);
    int cant = 0;

    getchar();
    printf("Introduzca el nombre de su carrito: ");
    scanf("%99[^\n]", nombreCarrito);

    getchar();
    printf("Introduzca el nombre del producto que desea agregar: ");
    scanf("%99[^\n]", &linea);

    productoCarrito->producto = searchMap(mapas->mapaNombre, linea);
    if (!productoCarrito->producto)
    {
        printf("El producto no existe\n");
        esperarEnter();
        return;
    }

    getchar();
    printf("Introduzca la cantidad del producto que desea agregar: ");
    scanf("%i", &cant);

    List *busqueda = searchMap(carritos, nombreCarrito);
    
    productoCarrito->cantidad = cant;

    // si no existe, crea un carrito y agrega el producto a este
    if(!busqueda){
        List *productos = createList();
        pushBack(productos, productoCarrito);
        insertMap(carritos, nombreCarrito, productos);
    }
    if(busqueda){
        pushBack(busqueda, productoCarrito);
    }

    printf("Producto agregado exitosamente\n");
    esperarEnter();
}

void menuEliminarCarrito(MapasGlobales *mapas)
{
    char key[32];

    printf("Introduzca el nombre del carrito del cual desea eliminar un producto\n\n");
    scanf("%99[^\n]",&key);

    List * lista = searchMap(mapas->mapaCarritos,key);

    if(lista == NULL){
        printf("El carrito ingresado no existe \n");
        esperarEnter();
        return;
    }

    ProductoCarrito * elemento = popBack(lista);

    printf("El producto eliminado del carrito es: %s, %s",elemento->producto->nombre, elemento->producto->marca);
}

void menuComprar(MapasGlobales *mapas)
{
    char linea[32];
    getchar();
    printf("Introduzca el nombre de su carrito: ");
    scanf("%99[^\n]", linea);

    List *carrito = searchMap(mapas->mapaCarritos, linea);

    if (carrito)
    {
        printf("Productos contenidos en el carrito:\n");
        imprimirCarrito(carrito);

        printf("Introduzca S para confirmar la compra, o cualquier otra cosa para volver al menú\n");
        char opcion;
        getchar();
        scanf("%c", &opcion);

        if (opcion != 'S') return;

        ProductoCarrito *productoCarrito = firstList(carrito);
        while (productoCarrito)
        {
            // si hay stock, se resta. de lo contrario, se salta el producto
            if ((productoCarrito->producto->stock - productoCarrito->cantidad) < 0)
            {
                printf("No hay suficiente stock para %i %s, se saltará este producto\n", productoCarrito->producto->stock, 
                                                                productoCarrito->producto->nombre);
            }
            else productoCarrito->producto->stock -= productoCarrito->cantidad;
            popFront(carrito);
            productoCarrito = firstList(carrito);
        }
    }
    else
    {
        printf("No se pudo encontrar el carrito especificado\n");
        return;
    }
    printf("Compra realizada exitosamente\n");
    esperarEnter();
}

void menuMostrarCarritos(MapasGlobales *mapas)
{
    Map * mapaCarritos = mapas->mapaCarritos;
    List *listaCarrito = firstMap(mapaCarritos);

    if(listaCarrito == NULL){
        printf("No existen carritos \n");
        esperarEnter();
        return;
    }
    
    while(listaCarrito != NULL)
    {
        int contador = 0;
        ProductoCarrito * producto = firstList(listaCarrito);
        printf("Nombre del carrito: %s\n", currentKey(mapaCarritos));

        while(1)
        {
            if(producto == NULL){
                printf("Cantidad de productos en el carrito: %i\n\n", contador);
                listaCarrito = nextMap(mapaCarritos);
                break;
            }
            else{
                contador += producto->cantidad;
                producto = nextList(listaCarrito);
            }
        }
    }
    esperarEnter();
}