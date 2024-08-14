#include "heap.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

int comparacion(const void *a, const void *b) {
    if (*(int*)a == *(int*)b) return 0;
    if (*(int*)a < *(int*)b) return -1;
    return 1;
}

static void pruebas_heap_vacio(void) {
    printf("\n--INICIO DE PRUEBAS CON HEAP VACIO--\n");
	
	heap_t *heap = heap_crear(comparacion);
	/* Prueba de correcta creacion del heap */
    print_test("El heap se creo correctamente", heap != NULL);
	
    /* Inicio de pruebas */
    print_test("Heap recien creado esta vacio", heap_esta_vacio(heap));
    print_test("Ver el maximo de heap vacio devuelve NULL", heap_ver_max(heap) == NULL);
    print_test("Ver cantidad es igual a 0", heap_cantidad(heap) == 0);
    print_test("Desencolar heap vacio devuelve NULL", heap_desencolar(heap) == NULL);
    print_test("Heap sigue vacio", heap_esta_vacio(heap));
    
    /* Destruyo el heap*/
    heap_destruir(heap, NULL);
}

static void pruebas_heap_algunos_elementos(void) {
    printf("\n--INICIO DE PRUEBAS CON ALGUNOS ELEMENTOS--\n");

    heap_t *heap = heap_crear(comparacion);
    /* Prueba de correcta creacion del heap */
    print_test("El heap se creo correctamente", heap != NULL);
    
    /* Asignacion de los datos */
    int dato1 = 20, dato2 = 45, dato3 = 12;

    /* Pruebo encolar elementos y que el maximo sea el correcto */
    print_test("Encolar dato1 es true", heap_encolar(heap, &dato1));
    print_test("Ver el maximo de heap es dato1", heap_ver_max(heap) == &dato1);
    print_test("Ver cantidad es igual a 1", heap_cantidad(heap) == 1);
    print_test("Encolar dato2 es true", heap_encolar(heap, &dato2));
    print_test("Ver el maximo de heap es dato2", heap_ver_max(heap) == &dato2);
    print_test("Ver cantidad es igual a 2", heap_cantidad(heap) == 2);
    print_test("Encolar dato3 es true", heap_encolar(heap, &dato3));
    print_test("Ver el maximo de heap es dato2", heap_ver_max(heap) == &dato2);
    print_test("Ver cantidad es igual a 3", heap_cantidad(heap) == 3);
    
    /* Pruebo desencolar y que sea lo esperado */
    print_test("Desencolar es igual a dato2", heap_desencolar(heap) == &dato2);
    print_test("Ver cantidad es igual a 2", heap_cantidad(heap) == 2);
    print_test("Ver el maximo de heap es dato1", heap_ver_max(heap) == &dato1); 
    print_test("Desencolar es igual a dato1", heap_desencolar(heap) == &dato1);
    print_test("Ver cantidad es igual a 1", heap_cantidad(heap) == 1);
    print_test("Desencolar es igual a dato3", heap_desencolar(heap) == &dato3);
    print_test("Heap ahora esta vacio", heap_esta_vacio(heap));
    print_test("Desencolar con el heap vacio devuelve NULL", heap_desencolar(heap) == NULL);
    print_test("El heap sigue estando vacio", heap_esta_vacio(heap));
    print_test("La cantidad del heap es 0", heap_cantidad(heap) == 0);

    /* Destruyo el heap*/
    heap_destruir(heap, NULL);
}

static void pruebas_heap_con_null(void) {
	printf("\n--INICIO DE PRUEBAS DE HEAP CON NULL--\n");
	
	heap_t *heap = heap_crear(comparacion);
    /* Prueba de correcta creacion del heap */
    print_test("El heap se creo correctamente", heap != NULL);
    
    /* Pruebo encolar NULL y que no se rompa el heap */
    print_test("Encolar NULL es true", heap_encolar(heap, NULL));
    print_test("Ver el maximo de heap es NULL", heap_ver_max(heap) == NULL);
    print_test("Ver cantidad es igual a 1", heap_cantidad(heap) == 1);
    print_test("Desencolar devuelve NULL", heap_desencolar(heap) == NULL);
    print_test("Heap ahora esta vacio", heap_esta_vacio(heap));
    print_test("La cantidad del heap es 0", heap_cantidad(heap) == 0);
    
    /* Destruyo el heap */
    heap_destruir(heap, NULL);
}

static void pruebas_heap_crear_con_arreglo(void) {
    printf("\n--INICIO DE PRUEBAS CREANDO HEAP CON UN ARREGLO--\n");
    
    /* Asignacion de los datos */
    int cuatro = 4, dos = 2, siete = 7, diez = 10, tres = 3, ocho = 8, cinco = 5;
    int* arreglo[] = {&cuatro, &dos, &siete, &diez, &tres, &ocho, &cinco};
    int* arreglo_ordenado[] = {&diez, &ocho, &siete, &cinco, &cuatro, &tres, &dos};
    size_t largo = 7;
    
    /* Creo el heap con el arreglo */
    heap_t* heap = heap_crear_arr((void**)arreglo, largo, comparacion);
    
    /* Prueba de correcta creacion del heap */
    print_test("El heap se creo correctamente", heap != NULL);
    print_test("El heap no esta vacio", !heap_esta_vacio(heap));
    print_test("La cantidad de elementos es 7", heap_cantidad(heap) == largo);
    print_test("Ver maximo es el 10", *(int*)heap_ver_max(heap) == diez);
    
    /* Pruebo desencolar elementos y que salgan en el orden correcto */
    bool ok = true;
    for (size_t i = 0; i < largo && ok; i++) {
    	if (heap_desencolar(heap) != arreglo_ordenado[i]) ok = false;
    }
    print_test("Todos los elementos se desencolaron en el orden esperado", ok);
    
    /* Pruebas adicionales */
    print_test("El heap esta vacio", heap_esta_vacio(heap));
    print_test("La cantidad del heap es 0", heap_cantidad(heap) == 0);
    print_test("Desencolar con el heap vacio es NULL", heap_desencolar(heap) == NULL);
    print_test("El heap sigue vacio", heap_esta_vacio(heap));
    print_test("La cantidad del heap sigue siendo 0", heap_cantidad(heap) == 0);
    
    /* Destruyo el heap */
    heap_destruir(heap, NULL);
}

// Fuente: https://stackoverflow.com/questions/42321370/fisher-yates-shuffling-algorithm-in-c
// Mas info: https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
void FisherYates(int* arreglo, size_t largo) { // implementation of Fisher Yates
    size_t i, j;
    int aux[largo];

     for (i = largo - 1; i > 0; i--) { // for loop to shuffle
         j = rand()%(largo-1); // randomise j for shuffle with Fisher Yates
         aux[i] = arreglo[j];
         arreglo[j] = arreglo[i];
         arreglo[i] = aux[i];
     }
}

static void pruebas_heap_crear_con_arreglo_con_redim(size_t tam) {
    printf("\n--INICIO DE PRUEBAS DE VOLUMEN CREANDO HEAP CON UN ARREGLO MUY GRANDE (CON REDIMENSION)--\n");
    
    /* Asignacion de los datos */
    int* arreglo = malloc(tam * sizeof(int));
    int* arreglo_ordenado = malloc(tam * sizeof(int));
    int** arreglo_punteros = malloc(tam * sizeof(int*));
    bool ok = true;
    for (int i = 0; i < tam; i++) {
        arreglo[i] = i;
        arreglo_ordenado[i] = i;
    }
    
    /* Mezclo el arreglo con el algoritmo de Fisher-Yates */
    FisherYates(arreglo, tam);
    
    for (int i = 0; i < tam; i++) {
    	arreglo_punteros[i] = &arreglo[i];
    }
    
    /* Creo el heap con el arreglo */
    heap_t* heap = heap_crear_arr((void**)arreglo_punteros, tam, comparacion);
    
    /* Prueba de correcta creacion del heap */
    print_test("El heap se creo correctamente", heap != NULL);
    print_test("El heap no esta vacio", !heap_esta_vacio(heap));
    print_test("La cantidad de elementos es correcta", heap_cantidad(heap) == tam);
    print_test("Ver maximo es correcto", *(int*)heap_ver_max(heap) == arreglo_ordenado[tam - 1]);
    
    /* Pruebo desencolar elementos y que salgan en el orden correcto */
    ok = true;
    for (size_t i = tam; i > 0 && ok; i--) {
    	if (*(int*)heap_desencolar(heap) != arreglo_ordenado[i - 1]) ok = false;
    }
    print_test("Todos los elementos se desencolaron en el orden esperado", ok);
    
    /* Pruebas adicionales */
    print_test("El heap esta vacio", heap_esta_vacio(heap));
    print_test("La cantidad del heap es 0", heap_cantidad(heap) == 0);
    print_test("Desencolar con el heap vacio es NULL", heap_desencolar(heap) == NULL);
    print_test("El heap sigue vacio", heap_esta_vacio(heap));
    print_test("La cantidad del heap sigue siendo 0", heap_cantidad(heap) == 0);
    
    /* Libero los arreglos */
    free(arreglo);
    free(arreglo_ordenado);
    free(arreglo_punteros);
    
    /* Destruyo el heap */
    heap_destruir(heap, NULL);
}

static void pruebas_heap_volumen(size_t tam) {
    printf("\n--INICIO DE PRUEBAS DE VOLUMEN--\n");
    
    heap_t *heap = heap_crear(comparacion);
    /* Prueba de correcta creacion del heap */
    print_test("El heap se creo correctamente", heap != NULL);

    /* Pruebo encolar en todas las posiciones */
    int* arreglo = malloc(tam * sizeof(int));
    int* arreglo_ordenado = malloc(tam * sizeof(int));
    bool ok = true;
    for (int i = 0; i < tam; i++) {
        arreglo[i] = i;
        arreglo_ordenado[i] = i;
    }
    
    /* Mezclo el arreglo con el algoritmo de Fisher-Yates */
    FisherYates(arreglo, tam);
    
    for (int i = 0; i < tam && ok; i++) {
    	ok = heap_encolar(heap, &arreglo[i]);
    }
    print_test("Se pudieron encolar todos los elementos", ok);
    print_test("El heap no esta vacio", !heap_esta_vacio(heap));
    print_test("La cantidad de elementos es la esperada", heap_cantidad(heap) == tam);
    print_test("El maximo es correcto", *(int*)heap_ver_max(heap) == tam - 1);
    
    /* Pruebo desencolar los elementos y que salgan en el orden correcto */
    ok = true;
    for (size_t i = tam; i > 0 && ok; i--) {
    	if (*(int*)heap_desencolar(heap) != arreglo_ordenado[i - 1]) ok = false;
    }
    print_test("Todos los elementos se desencolaron en el orden esperado", ok);
    print_test("El heap esta vacio",heap_esta_vacio(heap));
    
    /* Libero los arreglos */
    free(arreglo);
    free(arreglo_ordenado);
    
    /* Destruyo el heap */
    heap_destruir(heap,NULL);
}

static void pruebas_heap_con_destruir(void) {
    printf("\n--INICIO DE PRUEBAS CON FUNCION DESTRUIR--\n");

    heap_t *heap = heap_crear(comparacion);
    /* Prueba de correcta creacion del heap */
    print_test("El heap se creo correctamente", heap != NULL);
	
    /* Pido memoria para los datos */
    int* dato1 = malloc(sizeof(int));
    int* dato2 = malloc(sizeof(int));
    int* dato3 = malloc(sizeof(int));
    *dato1 = 20, *dato2 = 45, *dato3 = 12;

    /* Pruebo encolar elementos */
    print_test("Encolar dato1 es true", heap_encolar(heap, dato1));
    print_test("Encolar dato2 es true", heap_encolar(heap, dato2));
    print_test("Encolar dato3 es true", heap_encolar(heap, dato3));
    print_test("Ver el maximo de heap es dato2", heap_ver_max(heap) == dato2);
    print_test("Ver cantidad es igual a 3", heap_cantidad(heap) == 3);

    /* Destruir el heap con funcion destruir */
    heap_destruir(heap, free);
}

int comparacion_str(const void *a, const void *b) {
	if (strcmp((char*)a, (char*)b) == 0) return 0;
	else if (strcmp((char*)a, (char*)b) < 0) return -1;
	return 1;
}

static void pruebas_con_strings(void) {
	printf("\n--INICIO DE PRUEBAS USANDO HEAP CON STRINGS--\n");
    
    /* Asignacion de los datos */
    char* cadenas[] = {"hola", "alejo", "casa", "heap", "prueba", "string", "heapify"};
    char* cadenas_ordenado[] = {"string", "prueba", "hola", "heapify", "heap", "casa", "alejo"};
    size_t largo = 7;
    
    heap_t* heap = heap_crear_arr((void**)cadenas, largo, comparacion_str);
    
    /* Prueba de correcta creacion del heap */
    print_test("El heap se creo correctamente", heap != NULL);
    print_test("El heap no esta vacio", !heap_esta_vacio(heap));
    print_test("La cantidad de elementos es 7", heap_cantidad(heap) == largo);
    print_test("Ver maximo es 'alejo'", (char*)heap_ver_max(heap) == cadenas_ordenado[0]);
    
    /* Pruebo desencolar elementos y que salgan en el orden correcto */
    bool ok = true;
    for (size_t i = 0; i < largo && ok; i++) {
    	if (heap_desencolar(heap) != cadenas_ordenado[i]) ok = false;
    }
    print_test("Todos los elementos se desencolaron en el orden esperado", ok);
    
    /* Pruebas adicionales */
    print_test("El heap esta vacio", heap_esta_vacio(heap));
    print_test("La cantidad del heap es 0", heap_cantidad(heap) == 0);
    print_test("Desencolar con el heap vacio es NULL", heap_desencolar(heap) == NULL);
    print_test("El heap sigue vacio", heap_esta_vacio(heap));
    print_test("La cantidad del heap sigue siendo 0", heap_cantidad(heap) == 0);
    
    /* Destruyo el heap */
    heap_destruir(heap, NULL);
}

static void pruebas_heap_sort(void) {
	printf("\n--INICIO DE PRUEBAS CON HEAPSORT--\n");
	
	/* Ordeno un arreglo de varios elementos con heapsort */
	int cuatro = 4, dos = 2, siete = 7, diez = 10, tres = 3, ocho = 8, cinco = 5;
	int* arreglo[] = {&cuatro, &dos, &siete, &diez, &tres, &ocho, &cinco};
	int* arreglo_ordenado[] = {&dos, &tres, &cuatro, &cinco, &siete, &ocho, &diez};
	size_t largo = 7;
	printf("Arreglo a ordenar: {4, 2, 7, 10, 3, 8, 5}\n");
	printf("Ordeno con heapsort...\n");
	
	heap_sort((void**)arreglo, largo, comparacion);
	
	bool ok = true;
	for (size_t i = 0; i < largo && ok; i++) {
		if (arreglo[i] != arreglo_ordenado[i]) ok = false; 
	}
	print_test("El arreglo quedo ordenado", ok);
	
	
	/* Ordeno un arreglo ya ordenado con heapsort */
	printf("Ordeno un arreglo ordenado con heapsort...\n");
	heap_sort((void**)arreglo, largo, comparacion);
	ok = true;
	for (size_t i = 0; i < largo && ok; i++) {
		if (arreglo[i] != arreglo_ordenado[i]) ok = false;
	}
	print_test("El arreglo sigue ordenado", ok);
	
	
	/* Ordeno un arreglo de un elemento con heapsort */
	int* arreglo2[] = {&dos};
	int* arreglo2_ordenado[] = {&dos};
	largo = 1;
	printf("Ordeno un arreglo de un elemento con heapsort...\n");
	
	heap_sort((void**)arreglo2, largo, comparacion);
	
	ok = true;
	for (size_t i = 0; i < largo && ok; i++) {
		if (arreglo2[i] != arreglo2_ordenado[i]) ok = false; 
	}
	print_test("El arreglo de un elemento fue 'ordenado'", ok);
	
	
	/* Ordeno un arreglo pasando largo 0 con heapsort */
	int* arreglo3[] = {&cuatro, &dos};
	int* arreglo3_original[] = {&cuatro, &dos};
	int* arreglo3_ordenado[] = {&dos, &cuatro};
	largo = 0;
	printf("Ordeno un arreglo pasando largo 0 con heapsort...\n");
	
	heap_sort((void**)arreglo3, largo, comparacion);
	
	ok = true;
	for (size_t i = 0; i < largo && ok; i++) {
		if (arreglo3[i] != arreglo3_original[i]) ok = false; 
	}
	print_test("El arreglo pasando largo 0 quedo igual que antes", ok);
	
	
	/* Ordeno un arreglo de dos elementos con heapsort */
	largo = 2;
	printf("Ordeno un arreglo de dos elementos con heapsort...\n");
	
	heap_sort((void**)arreglo3, largo, comparacion);
	
	ok = true;
	for (size_t i = 0; i < largo && ok; i++) {
		if (arreglo3[i] != arreglo3_ordenado[i]) ok = false; 
	}
	print_test("El arreglo de dos elementos quedo ordenado", ok);
}



/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/

void pruebas_heap_estudiante() {
    /* Ejecuta todas las pruebas unitarias. */
    pruebas_heap_vacio();
    pruebas_heap_algunos_elementos();
    pruebas_heap_con_null();
    pruebas_heap_crear_con_arreglo();
    pruebas_heap_crear_con_arreglo_con_redim(10000);
    pruebas_heap_volumen(10000);
    pruebas_heap_con_destruir();
    pruebas_con_strings();
    pruebas_heap_sort();
}

#ifndef CORRECTOR  // Para que no de conflicto con el main() del corrector.

int main(void) {
    pruebas_heap_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
