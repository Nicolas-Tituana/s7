/*
 * Universidad - Programacion 1 - RC1
 * Optimizacion de produccion de componentes electronicos.
 * Arreglos paralelos, funciones y busqueda secuencial propia.
 * No utiliza struct, persistencia en archivos ni rutinas de busqueda.
 */
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PRODUCTOS 5
#define MAX_NOMBRE 40
#define TAM_LINEA 256
#define MAX_POR_UNIDAD 1000000LL
#define MAX_DEMANDA 100000LL
#define MAX_CAPACIDAD 1000000000000LL

/* Lee una linea completa y evita datos residuales del teclado. */
static void leer_linea(char destino[], int capacidad) {
    int c;
    int i = 0;
    if (fgets(destino, capacidad, stdin) == NULL) {
        puts("\nFin de entrada. Cerrando el programa.");
        exit(EXIT_SUCCESS);
    }
    while (destino[i] != '\0' && destino[i] != '\n') {
        i++;
    }
    if (destino[i] == '\n') {
        destino[i] = '\0';
    } else {
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
}

/* Validacion sin scanf: rechaza texto, numeros fuera de rango y vacios. */
static long long pedir_entero(const char *mensaje, long long minimo,
                             long long maximo) {
    char linea[TAM_LINEA];
    char *fin;
    long long valor;
    for (;;) {
        fputs(mensaje, stdout);
        leer_linea(linea, TAM_LINEA);
        errno = 0;
        fin = linea;
        valor = strtoll(linea, &fin, 10);
        if (fin == linea || errno == ERANGE) {
            printf("Dato invalido. Ingrese un entero entre %lld y %lld.\n",
                   minimo, maximo);
            continue;
        }
        while (isspace((unsigned char)*fin)) {
            fin++;
        }
        if (*fin != '\0' || valor < minimo || valor > maximo) {
            printf("Dato invalido. Ingrese un entero entre %lld y %lld.\n",
                   minimo, maximo);
            continue;
        }
        return valor;
    }
}

static void pedir_nombre(char destino[MAX_NOMBRE + 1]) {
    char linea[TAM_LINEA];
    int inicio, fin, longitud, i;
    for (;;) {
        printf("Nombre del producto (maximo %d caracteres): ", MAX_NOMBRE);
        leer_linea(linea, TAM_LINEA);
        inicio = 0;
        while (isspace((unsigned char)linea[inicio])) inicio++;
        fin = inicio;
        while (linea[fin] != '\0') fin++;
        while (fin > inicio && isspace((unsigned char)linea[fin - 1])) fin--;
        longitud = fin - inicio;
        if (longitud < 1 || longitud > MAX_NOMBRE) {
            puts("Nombre invalido: debe tener entre 1 y 40 caracteres.");
            continue;
        }
        for (i = 0; i < longitud; i++) destino[i] = linea[inicio + i];
        destino[longitud] = '\0';
        return;
    }
}

/* Comparacion propia (sin strcmp / strstr): ignora mayusculas ASCII. */
static int nombres_iguales(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (tolower((unsigned char)a[i]) !=
            tolower((unsigned char)b[i])) return 0;
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}

/* Busqueda secuencial desarrollada sin funciones de busqueda predefinidas. */
static int buscar_producto(char nombres[][MAX_NOMBRE + 1], int cantidad,
                           const char *buscado) {
    int i;
    for (i = 0; i < cantidad; i++) {
        if (nombres_iguales(nombres[i], buscado)) return i;
    }
    return -1;
}

static int solicitar_indice(char nombres[][MAX_NOMBRE + 1], int cantidad) {
    char buscado[MAX_NOMBRE + 1];
    int indice;
    if (cantidad == 0) {
        puts("No hay productos registrados.");
        return -1;
    }
    puts("Ingrese el nombre del producto a buscar.");
    pedir_nombre(buscado);
    indice = buscar_producto(nombres, cantidad, buscado);
    if (indice < 0) puts("Producto no encontrado.");
    return indice;
}

static void listar(char nombres[][MAX_NOMBRE + 1],
                   const long long tiempo[], const long long recursos[],
                   const long long demanda[], int cantidad) {
    int i;
    if (cantidad == 0) {
        puts("No hay productos registrados.");
        return;
    }
    puts("\nPRODUCTOS REGISTRADOS");
    puts("No.  Producto                       Demanda   Min/u    Rec/u");
    puts("------------------------------------------------------------");
    for (i = 0; i < cantidad; i++) {
        printf("%-4d %-30s %7lld %7lld %8lld\n", i + 1,
               nombres[i], demanda[i], tiempo[i], recursos[i]);
    }
}

static void agregar(char nombres[][MAX_NOMBRE + 1], long long tiempo[],
                    long long recursos[], long long demanda[], int *cantidad) {
    char nuevo[MAX_NOMBRE + 1];
    int indice;
    if (*cantidad >= MAX_PRODUCTOS) {
        puts("Limite alcanzado: solo se permiten cinco productos.");
        return;
    }
    pedir_nombre(nuevo);
    if (buscar_producto(nombres, *cantidad, nuevo) >= 0) {
        puts("Ya existe un producto con ese nombre. No se registro.");
        return;
    }
    indice = *cantidad;
    {
        int j = 0;
        do {
            nombres[indice][j] = nuevo[j];
        } while (nuevo[j++] != '\0');
    }
    demanda[indice] = pedir_entero("Unidades demandadas (0 a 100000): ",
                                   0, MAX_DEMANDA);
    tiempo[indice] = pedir_entero("Minutos por unidad (1 a 1000000): ",
                                  1, MAX_POR_UNIDAD);
    recursos[indice] = pedir_entero("Recursos por unidad (1 a 1000000): ",
                                    1, MAX_POR_UNIDAD);
    (*cantidad)++;
    puts("Producto registrado correctamente.");
}

static void editar(char nombres[][MAX_NOMBRE + 1], long long tiempo[],
                   long long recursos[], long long demanda[], int cantidad) {
    int indice = solicitar_indice(nombres, cantidad);
    long long opcion;
    if (indice < 0) return;
    do {
        char nuevo[MAX_NOMBRE + 1];
        int j;
        printf("\nEDITANDO: %s\n", nombres[indice]);
        puts("1. Nombre  2. Demanda  3. Tiempo por unidad");
        puts("4. Recursos por unidad  0. Volver");
        opcion = pedir_entero("Opcion: ", 0, 4);
        switch (opcion) {
            case 1:
                pedir_nombre(nuevo);
                j = buscar_producto(nombres, cantidad, nuevo);
                if (j >= 0 && j != indice) {
                    puts("Ya existe otro producto con ese nombre.");
                } else {
                    j = 0;
                    do { nombres[indice][j] = nuevo[j]; }
                    while (nuevo[j++] != '\0');
                    puts("Nombre actualizado.");
                }
                break;
            case 2:
                demanda[indice] = pedir_entero("Nueva demanda: ", 0, MAX_DEMANDA);
                puts("Demanda actualizada.");
                break;
            case 3:
                tiempo[indice] = pedir_entero("Nuevos minutos por unidad: ",
                                              1, MAX_POR_UNIDAD);
                puts("Tiempo actualizado.");
                break;
            case 4:
                recursos[indice] = pedir_entero("Nuevos recursos por unidad: ",
                                                1, MAX_POR_UNIDAD);
                puts("Recursos actualizados.");
                break;
            default: break;
        }
    } while (opcion != 0);
}

static void eliminar(char nombres[][MAX_NOMBRE + 1], long long tiempo[],
                     long long recursos[], long long demanda[], int *cantidad) {
    int indice = solicitar_indice(nombres, *cantidad);
    int i, j;
    if (indice < 0) return;
    for (i = indice; i < *cantidad - 1; i++) {
        j = 0;
        do { nombres[i][j] = nombres[i + 1][j]; }
        while (nombres[i + 1][j++] != '\0');
        tiempo[i] = tiempo[i + 1];
        recursos[i] = recursos[i + 1];
        demanda[i] = demanda[i + 1];
    }
    (*cantidad)--;
    puts("Producto eliminado; los arreglos se compactaron.");
}

static void evaluar(char nombres[][MAX_NOMBRE + 1],
                    const long long tiempo[], const long long recursos[],
                    const long long demanda[], int cantidad,
                    long long disponibleTiempo, long long disponibleRecursos,
                    int limitesConfigurados) {
    int i;
    long long totalTiempo = 0, totalRecursos = 0;
    if (cantidad == 0) {
        puts("No hay productos registrados. Primero ingrese productos.");
        return;
    }
    if (!limitesConfigurados) {
        puts("Primero configure el tiempo y los recursos disponibles (opcion 5).");
        return;
    }
    puts("\n========== REPORTE DE PRODUCCION ==========");
    for (i = 0; i < cantidad; i++) {
        long long tiempoProducto = demanda[i] * tiempo[i];
        long long recursosProducto = demanda[i] * recursos[i];
        totalTiempo += tiempoProducto;
        totalRecursos += recursosProducto;
        printf("%-22s | tiempo: %10lld min | recursos: %10lld\n",
               nombres[i], tiempoProducto, recursosProducto);
    }
    puts("--------------------------------------------");
    printf("TIEMPO REQUERIDO:       %lld minutos\n", totalTiempo);
    printf("TIEMPO DISPONIBLE:      %lld minutos\n", disponibleTiempo);
    printf("RECURSOS NECESARIOS:    %lld unidades\n", totalRecursos);
    printf("RECURSOS DISPONIBLES:   %lld unidades\n", disponibleRecursos);
    printf("DEFICIT DE TIEMPO:      %lld minutos\n",
           totalTiempo > disponibleTiempo ? totalTiempo - disponibleTiempo : 0);
    printf("DEFICIT DE RECURSOS:    %lld unidades\n",
           totalRecursos > disponibleRecursos ?
           totalRecursos - disponibleRecursos : 0);
    if (totalTiempo <= disponibleTiempo && totalRecursos <= disponibleRecursos) {
        puts("RESULTADO: DEMANDA FACTIBLE. La fabrica puede cumplirla.");
    } else {
        puts("RESULTADO: DEMANDA NO FACTIBLE con los limites actuales.");
    }
}

static void mostrar_menu(void) {
    puts("\n============== FABRICA ELECTRONICA ==============");
    puts("1. Registrar producto (hasta 5)");
    puts("2. Listar productos");
    puts("3. Buscar y editar producto");
    puts("4. Buscar y eliminar producto");
    puts("5. Configurar tiempo y recursos disponibles");
    puts("6. Calcular produccion y evaluar factibilidad");
    puts("0. Salir");
}

int main(void) {
    char nombres[MAX_PRODUCTOS][MAX_NOMBRE + 1] = {{0}};
    long long tiempo[MAX_PRODUCTOS] = {0};
    long long recursos[MAX_PRODUCTOS] = {0};
    long long demanda[MAX_PRODUCTOS] = {0};
    long long disponibleTiempo = 0, disponibleRecursos = 0;
    long long opcion;
    int cantidad = 0, limitesConfigurados = 0;
    do {
        mostrar_menu();
        opcion = pedir_entero("Seleccione una opcion: ", 0, 6);
        switch (opcion) {
            case 1:
                agregar(nombres, tiempo, recursos, demanda, &cantidad);
                break;
            case 2:
                listar(nombres, tiempo, recursos, demanda, cantidad);
                break;
            case 3:
                editar(nombres, tiempo, recursos, demanda, cantidad);
                break;
            case 4:
                eliminar(nombres, tiempo, recursos, demanda, &cantidad);
                break;
            case 5:
                disponibleTiempo = pedir_entero(
                    "Minutos disponibles (0 a 1000000000000): ",
                    0, MAX_CAPACIDAD);
                disponibleRecursos = pedir_entero(
                    "Recursos disponibles (0 a 1000000000000): ",
                    0, MAX_CAPACIDAD);
                limitesConfigurados = 1;
                puts("Limites de produccion actualizados.");
                break;
            case 6:
                evaluar(nombres, tiempo, recursos, demanda, cantidad,
                        disponibleTiempo, disponibleRecursos, limitesConfigurados);
                break;
            case 0:
                puts("Programa finalizado.");
                break;
            default: break;
        }
    } while (opcion != 0);
    return 0;
}
