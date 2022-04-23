
// mapCarritos
//     carritos
//         productos
//     mapN
//         lista productos
//     mapT
//         lista productos
//     mapM
//         lista productos

// productos es el mismo en los mapas de busqueda

agregarProductoCarritos(hashmap);

void agregarProducto(Producto tipoProducto, list * productos)
{
    printf("Introduzca el producto que desea agregar utilizando el siguiente formato:\n");
    printf("\'Nombre,Tipo,Marca,Stock,Precio'\n");

    char linea[1024];
    
    getchar(); // elimina el buffer
    scanf("%99[^\n]", linea); // lee todo hasta encontrar un \n


    while (C != NULL)
    {
        strtok(linear , ",");
        if (strcmp(C->nombre, producto->nombre) == 0)
        {
            strtok(linear , ",");
            if (strcmp(C->marca, producto->marca) == 0)
            {
                printf("Este producto ya existe en la lista, se aumentara el stock\n");
                C->stock += 1;
                esperarEnter();
                return;
            }
        }
        C = nextList(listaGlobal);
    }
    pushBack(listaGlobal, producto);

    insertMapas(tipoProducto->mapNombre,nombre,ptrProducto);
        
    insertMapas(tipoProducto->mapTipo,tipo,ptrProducto);

    insertMapas(tipoProducto->mapMarca,marca,ptrProducto);

    printf("Producto añadido exitosamente.\n");
    esperarEnter();
    return;
}

void muestraProductosTipo(tiporoducto->mapTipo){
    producto = firstMap(map)
    While(producto != NULL){    //cambiar, no se me ocurre como era
        if(casilla->buckets != NULL) mostrarProducto();
        producto = nextMap(map);
    }
   
}

void agregaProductoCarrito
{
    
}
void concretarCompra