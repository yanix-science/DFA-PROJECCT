#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ESTADOS 100
#define MAX_ALFABETO 10
#define MAX_TRANSICIONES 100

typedef struct {
    char estados[MAX_ESTADOS][50];
    char alfabeto[MAX_ALFABETO][50];
    char estado_inicial[50];
    char estados_aceptacion[MAX_ESTADOS][50];
    char transiciones[MAX_TRANSICIONES][3][50];
    int num_estados;
    int num_alfabeto;
    int num_transiciones;
    int num_estados_aceptacion;
} DFA;

// Funciones auxiliares
int encontrar_indice_estado(DFA *dfa, const char *estado) {
    for (int i = 0; i < dfa->num_estados; i++) {
        if (strcmp(dfa->estados[i], estado) == 0) {
            return i;
        }
    }
    return -1;
}

int encontrar_indice_alfabeto(DFA *dfa, const char *simbolo) {
    for (int i = 0; i < dfa->num_alfabeto; i++) {
        if (strcmp(dfa->alfabeto[i], simbolo) == 0) {
            return i;
        }
    }
    return -1;
}

// Función para leer el archivo de configuración
void leer_config_dfa(const char *nombre_archivo, DFA *dfa) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    char *token;

    // Leer estados
    fgets(buffer, sizeof(buffer), archivo);
    token = strtok(buffer, ",\n");
    dfa->num_estados = 0;
    while (token != NULL) {
        strcpy(dfa->estados[dfa->num_estados++], token);
        token = strtok(NULL, ",\n");
    }

    // Leer alfabeto
    fgets(buffer, sizeof(buffer), archivo);
    token = strtok(buffer, ",\n");
    dfa->num_alfabeto = 0;
    while (token != NULL) {
        strcpy(dfa->alfabeto[dfa->num_alfabeto++], token);
        token = strtok(NULL, ",\n");
    }

    // Leer estado inicial
    fscanf(archivo, "%s", dfa->estado_inicial);
    fgetc(archivo);  // Leer el salto de línea restante

    // Leer estados de aceptación
    fgets(buffer, sizeof(buffer), archivo);
    token = strtok(buffer, ",\n");
    dfa->num_estados_aceptacion = 0;
    while (token != NULL) {
        strcpy(dfa->estados_aceptacion[dfa->num_estados_aceptacion++], token);
        token = strtok(NULL, ",\n");
    }

    // Leer transiciones
    dfa->num_transiciones = 0;
    while (fscanf(archivo, "%[^,],%[^,],%s", dfa->transiciones[dfa->num_transiciones][0],
                  dfa->transiciones[dfa->num_transiciones][1],
                  dfa->transiciones[dfa->num_transiciones][2]) != EOF) {
        fgetc(archivo); // Leer el salto de línea
        dfa->num_transiciones++;
    }

    fclose(archivo);
}

// Función para procesar la cadena de entrada
int procesar_cadena(DFA *dfa, const char *entrada) {
    int indice_estado_actual = encontrar_indice_estado(dfa, dfa->estado_inicial);
    if (indice_estado_actual == -1) {
        fprintf(stderr, "Estado inicial '%s' no encontrado\n", dfa->estado_inicial);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < strlen(entrada); i++) {
        int indice_simbolo = encontrar_indice_alfabeto(dfa, (char[]){entrada[i], '\0'});
        if (indice_simbolo == -1) {
            fprintf(stderr, "Símbolo '%c' no está en el alfabeto\n", entrada[i]);
            exit(EXIT_FAILURE);
        }

        int transicion_encontrada = 0;
        for (int j = 0; j < dfa->num_transiciones; j++) {
            if (strcmp(dfa->transiciones[j][0], dfa->estados[indice_estado_actual]) == 0 &&
                strcmp(dfa->transiciones[j][1], (char[]){entrada[i], '\0'}) == 0) {
                indice_estado_actual = encontrar_indice_estado(dfa, dfa->transiciones[j][2]);
                if (indice_estado_actual == -1) {
                    fprintf(stderr, "Siguiente estado '%s' no encontrado\n", dfa->transiciones[j][2]);
                    exit(EXIT_FAILURE);
                }
                transicion_encontrada = 1;
                break;
            }
        }

        if (!transicion_encontrada) {
            fprintf(stderr, "Transición no encontrada para el estado '%s' y el símbolo '%c'\n",
                    dfa->estados[indice_estado_actual], entrada[i]);
            return 0;
        }
    }

    for (int i = 0; i < dfa->num_estados_aceptacion; i++) {
        if (strcmp(dfa->estados_aceptacion[i], dfa->estados[indice_estado_actual]) == 0) {
            return 1;  // Aceptada
        }
    }

    return 0;  // Rechazada
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_config>\n", argv[0]);
        return EXIT_FAILURE;
    }

    DFA dfa;
    leer_config_dfa(argv[1], &dfa);

    printf("Ingrese la cadena a procesar: ");
    char entrada[256];
    scanf("%s", entrada);

    int resultado = procesar_cadena(&dfa, entrada);

    FILE *archivo_salida = fopen("resultado.txt", "w");
    if (resultado) {
        fprintf(archivo_salida, "La cadena fue aceptada.\n");
    } else {
        fprintf(archivo_salida, "La cadena fue rechazada.\n");
    }
    fclose(archivo_salida);

    return EXIT_SUCCESS;
}
