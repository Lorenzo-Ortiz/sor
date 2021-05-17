#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>


#define LIMITE 50

FILE* receta;

FILE* archivo; //archivo txt para guardar el proceso

int equipoGanador=0;

//creo estructura de semaforos 
struct semaforos {
    sem_t sem_mezclar;
    sem_t sem_salar;
    sem_t sem_agregarCarne;
    sem_t sem_empanar;
    sem_t sem_cocinar;
    sem_t sem_armarMilanesa;	
    sem_t sem_aux;
    sem_t sem_ganador;		

	//poner demas semaforos aqui
};

	pthread_mutex_t mutex_salar = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_cocinar = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_hornear = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t escribir = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t ganador = PTHREAD_MUTEX_INITIALIZER;


//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[8][LIMITE];
   
};

//creo los parametros de los hilos 
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[9];
};

//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {

	pthread_mutex_lock(&escribir);
	
	archivo = fopen("archivo.txt", "a"); //Abro archivo, modo append

	if (archivo == NULL){

		printf("Error al abrir archivo\n");}


	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		fprintf(archivo,"\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param);
		fprintf(archivo,"\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param);
 
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
							fprintf(archivo,"\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}

		}
	
	fprintf(archivo, "   \n" ); //Espacio

	fflush(archivo); //Libero memoria

	fclose(archivo); //cierro archivo
	pthread_mutex_unlock(&escribir);


	}

//funcion para tomar de ejemplo
void* cortar(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "cortar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 60000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_mezclar);
	
    pthread_exit(NULL);
}

void* mezclar(void *data) {
	struct parametro *mydata=data;
	sem_wait(&mydata->semaforos_param.sem_mezclar);

	//creo el nombre de la accion de la funcion 
	char *accion = "mezclar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 60000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
	
	sem_post(&mydata->semaforos_param.sem_salar);

   
	
    pthread_exit(NULL);
}

void* salar(void *data) {
	
	struct parametro *mydata=data;
	sem_wait(&mydata->semaforos_param.sem_salar);
	pthread_mutex_lock(&mutex_salar);

	//creo el nombre de la accion de la funcion 
	char *accion = "salar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 60000 );
	//doy la señal a la siguiente accion (cortar me habilita agregarCarne)
	pthread_mutex_unlock(&mutex_salar);
	
	sem_post(&mydata->semaforos_param.sem_agregarCarne);
   
	
   	 pthread_exit(NULL);
}

void* agregarCarne(void *data) {
	
	struct parametro *mydata=data;
	sem_wait(&mydata->semaforos_param.sem_agregarCarne);
	//creo el nombre de la accion de la funcion 
	char *accion = "agregarCarne";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero) 
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	archivo = fopen("archivo.txt", "a");
	printf("        Equipo %d - accion agregar carne \n \n", mydata->equipo_param);
	fprintf(archivo,"        Equipo %d - accion agregar carne \n \n", mydata->equipo_param);
	//uso sleep para simular que que pasa tiempo
	usleep( 60000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
	sem_post(&mydata->semaforos_param.sem_empanar);
	
   
	
    pthread_exit(NULL);
}

void* empanar(void *data) {
	
	struct parametro *mydata=data;
	sem_wait(&mydata->semaforos_param.sem_empanar);
	//creo el nombre de la accion de la funcion 
	char *accion = "empanar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 60000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
	sem_post(&mydata->semaforos_param.sem_cocinar);
	
   
	
    pthread_exit(NULL);
}

void* cocinar(void *data) {
	
	struct parametro *mydata=data;
	sem_wait(&mydata->semaforos_param.sem_cocinar);
	pthread_mutex_lock(&mutex_cocinar);
	
	//creo el nombre de la accion de la funcion 
	char *accion = "cocinar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 60000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
	pthread_mutex_unlock(&mutex_cocinar);
	
	sem_post(&mydata->semaforos_param.sem_armarMilanesa);
	
   
	
    pthread_exit(NULL);
}

void* hornearPan(void *data) {
	
	struct parametro *mydata=data;
	pthread_mutex_lock(&mutex_hornear);
	
	
	//creo el nombre de la accion de la funcion 
	char *accion = "hornear Pan";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 60000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
	pthread_mutex_unlock(&mutex_hornear);
	
	sem_post(&mydata->semaforos_param.sem_aux);
	
   
	
    pthread_exit(NULL);
}



void* armarMilanesa(void *data) {
	
	struct parametro *mydata=data;
	
	sem_wait(&mydata->semaforos_param.sem_aux);

	sem_wait(&mydata->semaforos_param.sem_armarMilanesa);
	
	char *accion = "armar milanesa";

	archivo = fopen("archivo.txt", "a");

	fprintf(archivo, "        Equipo %d - accion armar sandwich \n", mydata->equipo_param);

	
	//creo el nombre de la accion de la funcion 
	
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	printf("        Equipo %d - accion armar sandwich \n", mydata->equipo_param);
	printf("                 -----------ingredientes : ----------\n", mydata->equipo_param);
	printf("                 ingrediente 0 : verduras(lechuga, cebolla, pepino)\n\n", mydata->equipo_param);

	
	//fprintf(archivo,"                 -----------ingredientes : ----------\n", mydata->equipo_param);
	//fprintf(archivo,"                 ingrediente 0 : verduras(lechuga, cebolla, pepino)\n\n", mydata->equipo_param);
	//uso sleep para simular que que pasa tiempo
	usleep( 60000 );
	sem_post(&mydata->semaforos_param.sem_ganador);	
	
   
	
    pthread_exit(NULL);
}



void* ganar(void *data) {
	
	struct parametro *mydata=data;
	
	sem_wait(&mydata->semaforos_param.sem_ganador);
	
	pthread_mutex_lock(&ganador);
	
	char *accion = "Nombrar equipo ganador";
	
	imprimirAccion(mydata,accion);
	usleep( 60000 );


	if(equipoGanador==0){
	pthread_mutex_lock(&escribir);

	archivo = fopen("archivo.txt", "a");
	printf("        *************************************************************************\n", mydata->equipo_param);

	printf("        * El Equipo %d termino el Sandwich!!! Felictaciones. Tienen el trabajo!  *\n", mydata->equipo_param);
	printf("        *************************************************************************\n", mydata->equipo_param);


	printf("\n");

	fprintf(archivo,"        * El Equipo %d termino el Sandwich!!! Felictaciones. Tienen el trabajo!  *\n", mydata->equipo_param);
	fprintf(archivo, "\n");

	fclose(archivo);
	equipoGanador += *((int *) &mydata->equipo_param);

	equipoGanador++;
	

	pthread_mutex_unlock(&escribir);}
		
	//creo el nombre de la accion de la funcion 
	
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
		
	pthread_mutex_unlock(&ganador);
	
	pthread_exit(NULL);
	}


void* ejecutarReceta(void *i) {
	
	//levanto archivo
	FILE* receta = NULL;
	receta = fopen("receta.txt", "rb");
	


	//armo los pasos de la receta
	char paso1 [100]="";
	char paso2 [100]="";
	char paso3 [100]="";
	char paso4 [100]="";
	char paso5 [100]="";
	char paso6 [100]="";
	char paso7 [100]="";
	char paso8 [100]="";
	

	//asigno valor a las cadenas desde la receta
	fgets(paso1,100,receta); //cortar
	fgets(paso2,100,receta); //mezclar
	fgets(paso3,100,receta); //salar
	fgets(paso4,100,receta); //agregarCarne
	fgets(paso5,100,receta); //empanar
	fgets(paso6,100,receta); //cocinar
	fgets(paso7,100,receta); //hornear Pan
	fgets(paso8,100,receta); //armar Milanesa
	

	char lista1 [6][100];
	char lista2 [6][100];
	char lista3 [6][100];
	char lista4 [6][100];
	char lista5 [6][100];
	char lista6 [6][100];
	char lista7 [6][100];
	char lista8 [6][100];
	

	char *token1, *token2, *token3, *token4, *token5, *token6, *token7, *token8;
	
	int pos1=0;
	token1 = strtok(paso1, "|");
	while (token1 != NULL){
	token1 = strtok(NULL, "|");
	if (token1 !=NULL){    	
	  strcpy(lista1[pos1++],token1);}}

	int pos2=0;
	token2 = strtok(paso2, "|");
	while (token2 != NULL){
	token2 = strtok(NULL, "|");
	if (token2 !=NULL){    	
	  strcpy(lista2[pos2++],token2);}}

	int pos3=0;
	token3 = strtok(paso3, "|");
	while (token3 != NULL){
	token3 = strtok(NULL, "|");
	if (token3 !=NULL){    	
	  strcpy(lista3[pos3++],token3);}}

	int pos4=0;
	token4 = strtok(paso4, "|");
	while (token4 != NULL){
	token4 = strtok(NULL, "|");
	if (token4 !=NULL){    	
	  strcpy(lista4[pos4++],token4);}}

	int pos5=0;
	token5 = strtok(paso5, "|");
	while (token5 != NULL){
	token5 = strtok(NULL, "|");
	if (token5 !=NULL){    	
	  strcpy(lista5[pos5++],token5);}}

	int pos6=0;
	token6 = strtok(paso6, "|");
	while (token6 != NULL){
	token6 = strtok(NULL, "|");
	if (token6 !=NULL){    	
	  strcpy(lista6[pos6++],token6);}}

	int pos7=0;
	token7 = strtok(paso7, "|");
	while (token7 != NULL){
	token7 = strtok(NULL, "|");
	if (token7 !=NULL){    	
	  strcpy(lista7[pos7++],token7);}}

	int pos8=0;
	token8 = strtok(paso8, "|");
	while (token8 != NULL){
	token8 = strtok(NULL, "|");
	if (token8 !=NULL){    	
	  strcpy(lista8[pos8++],token8);}}

	



	pthread_mutex_t mutex_salar;
	pthread_mutex_t mutex_cocinar;
	pthread_mutex_t mutex_hornear;
	pthread_mutex_t ganador;


	
	//variables semaforos
	sem_t sem_mezclar;
    	sem_t sem_salar;
    	sem_t sem_agregarCarne;
    	sem_t sem_empanar;
    	sem_t sem_cocinar;
    	sem_t sem_armarMilanesa;	
    	sem_t sem_aux;
	sem_t sem_ganador;	
	//crear variables semaforos aqui
	
	//variables hilos
	pthread_t p1; 
	//crear variables hilos aqui
	pthread_t p2; 
	pthread_t p3; 
	pthread_t p4; 
	pthread_t p5; 
	pthread_t p6; 
	pthread_t p7;
	pthread_t p8;  
	pthread_t p9; 
	
	
	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);
	
	printf("Ejecutando equipo %d \n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));
	

	//seteo los valores al struct
	
	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	pthread_data->semaforos_param.sem_salar = sem_salar;
	pthread_data->semaforos_param.sem_agregarCarne = sem_agregarCarne;
	pthread_data->semaforos_param.sem_empanar = sem_empanar;
	pthread_data->semaforos_param.sem_cocinar = sem_cocinar;
	pthread_data->semaforos_param.sem_armarMilanesa = sem_armarMilanesa;
	pthread_data->semaforos_param.sem_aux = sem_aux;
	pthread_data->semaforos_param.sem_aux = sem_ganador;


	//setear demas semaforos al struct aqui
	

	//seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ¿Se ve hardcodeado no? ¿Les parece bien?
     	int h=0;

	strcpy(pthread_data->pasos_param[0].accion, "cortar");
	    strcpy(pthread_data->pasos_param[0].ingredientes[0], lista1[1]);
      strcpy(pthread_data->pasos_param[0].ingredientes[1], lista1[2]);


	    strcpy(pthread_data->pasos_param[1].accion, "mezclar");
	    strcpy(pthread_data->pasos_param[1].ingredientes[0], lista2[1]);
      strcpy(pthread_data->pasos_param[1].ingredientes[1], lista2[2]);
 	    strcpy(pthread_data->pasos_param[1].ingredientes[2], lista2[3]);
	    	
	 strcpy(pthread_data->pasos_param[2].accion, "salar");
	    strcpy(pthread_data->pasos_param[2].ingredientes[0], lista3[1]);
      strcpy(pthread_data->pasos_param[2].ingredientes[1], lista3[2]);
 	    strcpy(pthread_data->pasos_param[2].ingredientes[2], lista3[3]);

	strcpy(pthread_data->pasos_param[3].accion, "agregar Carne");
	    strcpy(pthread_data->pasos_param[3].ingredientes[0], lista4[1]);
      
	strcpy(pthread_data->pasos_param[4].accion, "empanar");
	    strcpy(pthread_data->pasos_param[4].ingredientes[0], lista5[1]);
      strcpy(pthread_data->pasos_param[4].ingredientes[1], lista5[2]);
	strcpy(pthread_data->pasos_param[4].ingredientes[2], lista5[3]);
      strcpy(pthread_data->pasos_param[4].ingredientes[3], lista5[4]);

	strcpy(pthread_data->pasos_param[5].accion, "cocinar");
	    strcpy(pthread_data->pasos_param[5].ingredientes[0], lista6[1]);

	strcpy(pthread_data->pasos_param[6].accion, "hornear Pan");
	    strcpy(pthread_data->pasos_param[6].ingredientes[0], lista7[1]);

	strcpy(pthread_data->pasos_param[7].accion, "armar Milanesa");
	    strcpy(pthread_data->pasos_param[7].ingredientes[0], lista8[1]);
      strcpy(pthread_data->pasos_param[7].ingredientes[1], lista8[2]);
	strcpy(pthread_data->pasos_param[7].ingredientes[2], lista8[3]);
      strcpy(pthread_data->pasos_param[7].ingredientes[3], lista8[4]);

	strcpy(pthread_data->pasos_param[8].accion, "Nombrar ganador");
	    	     







	
	//inicializo los semaforos

	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_salar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_agregarCarne),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_empanar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_cocinar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_armarMilanesa),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_aux),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_ganador),0,0);

	//inicializar demas semaforos aqui


	
	


	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;
    rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	//crear demas hilos aqui
	
	
	
	rc = pthread_create(&p2,                           //identificador unico
                            NULL,                          //atributos del thread
                                mezclar,            
                                pthread_data);
	
	rc=pthread_create(&p3, NULL, salar, pthread_data);
	
	rc=pthread_create(&p4, NULL, agregarCarne, pthread_data);
	
	rc=pthread_create(&p5, NULL, empanar, pthread_data);

	rc=pthread_create(&p6, NULL, cocinar, pthread_data);
	
	rc=pthread_create(&p7, NULL, hornearPan, pthread_data);

	rc=pthread_create(&p8, NULL, ganar, pthread_data);
	
	rc=pthread_create(&p9, NULL, armarMilanesa, pthread_data);

	
	
		
	
	
	
	
	//join de todos los hilos
	pthread_join (p1,NULL);
	//crear join de demas hilos
	pthread_join (p2,NULL);
	pthread_join (p3,NULL);
	pthread_join (p4,NULL);
	pthread_join (p5,NULL);
	pthread_join (p6,NULL);
	pthread_join (p7,NULL);
	pthread_join (p8,NULL);
	




	//valido que el hilo se alla creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }

	 
	//destruccion de los semaforos 
	sem_destroy(&sem_mezclar);
	//destruir demas semaforos
	sem_destroy(&sem_salar);
	sem_destroy(&sem_agregarCarne);
	sem_destroy(&sem_empanar);
	sem_destroy(&sem_cocinar);
	sem_destroy(&sem_armarMilanesa);
	sem_destroy(&sem_aux);
	sem_destroy(&sem_ganador);

	pthread_mutex_destroy(&mutex_salar);
	pthread_mutex_destroy(&mutex_cocinar);
	pthread_mutex_destroy(&mutex_hornear);
	pthread_mutex_destroy(&ganador);
	
 
	
	//salida del hilo
	 pthread_exit(NULL);
}


int main ()
{
	FILE* receta = NULL;
	receta = fopen("receta.txt", "rb");

	archivo = fopen("archivo.txt", "w");

	if (archivo==NULL){

		printf("Error al crear el archivo. \n");
	}

	fprintf(archivo,"----------------COMIENZA EL JUEGO--------------\n");
	fprintf(archivo," \n");
	fprintf(archivo," \n");
	fflush(archivo);
	fclose(archivo);


	
	

	//creo los nombres de los equipos 
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
	int *equipoNombre4 =malloc(sizeof(*equipoNombre4));



//faltan equipos
  
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;
	*equipoNombre4 = 4;

	//creo las variables los hilos de los equipos
	pthread_t equipo1; 
	pthread_t equipo2;
	pthread_t equipo3;
	pthread_t equipo4;

//faltan hilos
  
	//inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre1); 

    rc = pthread_create(&equipo2,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre2);
  //faltn inicializaciones
	rc= pthread_create(&equipo3, NULL, ejecutarReceta, equipoNombre3);
	
	rc= pthread_create(&equipo4, NULL, ejecutarReceta, equipoNombre4);

   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     } 

	//join de todos los hilosd
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);
	pthread_join (equipo4,NULL);

//.. faltan joins
	
	

    pthread_exit(NULL);
}


//Para compilar:   gcc subwayArgento.c -o ejecutable -lpthread
//Para ejecutar:   ./ejecutable
