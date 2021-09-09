#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
/*El programa comienza solicitando que se ingrese por teclado una cantidad de personas (N), continua pidiendo hasta
que la cantidad de personas ingresadas sea mayor a 0 y menor o igual al total de personas en el archivo de entrada.

Luego, genera N numeros aleatorios sin repeir que representaran el numero de la linea de la persona a leer,
 y luego escribir en el archivo de salida, y los almacena en un arreglo.

Despues, arma un arreglo de strings con las localidades del archivo codigoLocalidades donde la posicion 
en la que se almacenan representa el codigo-1 de la respectiva localidad.

Finalmente, comienza a leer las personas que correspenden segun el numero aleatorio, almacena sus
datos en variables temporales y luego los escribe en el archivo de salida como corresponde.*/

int cant_lineas(FILE *archivo){
    int lineas = 0;
    for (char c = fgetc(archivo); c != EOF; c = fgetc(archivo)) {
        if (c == '\n')
            lineas++;
    }
    return lineas;
}

void eliminar_espacios(char *array){
    int i = 0;
    while (array[i]!='\0'){
      i++;
    }
    while(array[i]!=' '){
      i--;
    }
    while(array[i]==' '){
      i--;
    }
    array[i+1]='\0';
}

void crear_array_localidades(FILE *archivo, int largo,char **localidades){
    char basura[10], buffer[100];
    for (int i=0; i < largo;++i){
        fscanf(archivo,"%[^,],",basura);
        fscanf(archivo,"%[^\n]",buffer);
        eliminar_espacios(buffer);
        localidades[i] = malloc(sizeof(char)*strlen(buffer)+1);
        strcpy(localidades[i], buffer);
    }
}

void liberar_memoria(char **array,int largo){
    for(int i = 0; i < largo; ++i){
        free(array[i]);
    }
}

void generar_personas(int cantPersonas, int arrayRandom[]){
    FILE *fPersonas,*fLocalidades,*fSalida;
    char buffer[100],genero[2]={'M','F'},interes[4]={'F','M','A','N'};
    char nombre[100], apellido[100];
    int linea = 0, codigo, sexo, sexualidad, edad, cantLineas;
    fLocalidades =fopen("codigoLocalidades.txt","r");
    cantLineas = cant_lineas(fLocalidades);
    char **localidades=(char**)malloc(sizeof(char*)*cantLineas);
    rewind(fLocalidades);
    crear_array_localidades(fLocalidades,cantLineas,localidades);
    fclose(fLocalidades);
    fPersonas =fopen("personas.txt","r");
    fSalida =fopen("fSalida.txt","w");
    for(int i = 0; i< cantPersonas; i++){
        while(linea<arrayRandom[i]){
            fgets(buffer,100,fPersonas);
            linea++;
        }
        fscanf(fPersonas,"%[^,],%[^,],%d,%d,%d,%d", nombre, apellido,&codigo,&edad,&sexo,&sexualidad);
        fprintf(fSalida,"%s, %s, %s, %d, %c, %c\n", nombre, apellido,localidades[codigo-1],edad,genero[sexo-1],interes[sexualidad-1]);
    }
    fclose(fPersonas);
    fclose(fSalida);
    liberar_memoria(localidades,cantLineas);
    free(localidades);
    free(arrayRandom);
}

void crear_array_random(int arrayRandom[],int cantPersonas,int totalPersonas){
    int *arrayIndex=(int*)malloc(sizeof(int)*totalPersonas);
    int numRandom;
    for(int i=0;i<totalPersonas;++i){
        arrayIndex[i]=0;
    }
    srand((unsigned int)time(NULL));
    for(int i=cantPersonas; i>0;--i){
        numRandom = rand() % (totalPersonas);
        while(arrayIndex[numRandom]!=0){
            numRandom = rand() % (totalPersonas);
        }
        arrayIndex[numRandom]=-1;
    }
    int idx=0;
    for(int i=0;i<totalPersonas;++i){
        if(arrayIndex[i]==-1){
            arrayRandom[idx]=i;
            idx++;
        }
    }
    free(arrayIndex);
}

int main(int argc, char **argv){
    int cantPersonas=0, lineas=0;
    FILE *fEntrada;
    fEntrada = fopen("personas.txt","r");
    lineas = cant_lineas(fEntrada);
    fclose(fEntrada);
    printf("Ingrese la cantidad de personas: ");
    scanf("%d",&cantPersonas);
    while ((cantPersonas > lineas) || (cantPersonas < 0)){
        printf("Por favor ingrese una cantidad menor a %d y mayor a 0: ", lineas);
        scanf("%d",&cantPersonas);
    }
    int *nums_random = malloc(sizeof(int)*cantPersonas);
    crear_array_random(nums_random,cantPersonas,lineas);
    generar_personas(cantPersonas,nums_random);
    return 0;
}
