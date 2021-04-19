#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>


#define LIMITE 50

//creo estructura de semaforos 
struct semaforos {
    sem_t sem_mezclar;
	sem_t sem_salar;
	sem_t sem_agregarCarne;
	sem_t sem_empanar;
	sem_t sem_cocinar;
	sem_t sem_agregarMilanesa;
	sem_t sem_cortarPan;
	sem_t sem_agra¿egarOtros;
	sem_t sem_aux;
	
	//poner demas semaforos aqui
};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];
   
};

//creo los parametros de los hilos 
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[8];
};

//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}
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
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_mezclar);
	
    pthread_exit(NULL);
}

void* mezclar(void *data) {
	sem_wait(&mydata->semaforos_param.sem_mezclar)
	//creo el nombre de la accion de la funcion 
	char *accion = "mezclar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_salar);
	
    pthread_exit(NULL);
}

void* salar(void *data) {
	sem_wait(&mydata->semaforos_param.sem_salar)
	//creo el nombre de la accion de la funcion 
	char *accion = "salar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_agregarCarne);
	
    pthread_exit(NULL);
}
void* agregarCarne(void *data) {
	sem_wait(&mydata->semaforos_param.sem_agregarCarne)
	//creo el nombre de la accion de la funcion 
	char *accion = "agregarCarne";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_empanar);
	
    pthread_exit(NULL);
}

void* empanar(void *data) {
	sem_wait(&mydata->semaforos_param.sem_empanar)
	//creo el nombre de la accion de la funcion 
	char *accion = "empanar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_cocinar);
	
    pthread_exit(NULL);
}

void* cocinar(void *data) {
	sem_wait(&mydata->semaforos_param.sem_empanar)
	//creo el nombre de la accion de la funcion 
	char *accion = "cocinar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_agregarMilanesa);
	
    pthread_exit(NULL);
}

void* hornearPan(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "hornearPan";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_cortarPan);
	
    pthread_exit(NULL);
}
void* cortarPan(void *data) {
	sem_wait(&mydata->semaforos_param.sem_cortarPan)
	//creo el nombre de la accion de la funcion 
	char *accion = "cortarPan";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_aux);
	sem_post(&mydara->semaforos_param.sem_agregarOtros);
	
    pthread_exit(NULL);
}

void* agregarOtros(void *data) {
	sem_wait(&mydata->semaforos_param.sem_cortarPan)
	//creo el nombre de la accion de la funcion 
	char *accion = "agregarOtros";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
   
	
    pthread_exit(NULL);
}
void* agregarMilanesa(void *data) {
	sem_wait(&mydata->semaforos_param.sem_aux);
	sem_wait(&mydata->semaforos_param.sem_cocinar);
	//creo el nombre de la accion de la funcion 
	char *accion = "empanar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
	
    pthread_exit(NULL);
}




void* ejecutarReceta(void *i) {
	
	//variables mutex
	pthread_mutex mutex_salar;
	pthread_mutex mutex_cocinar;
	pthread_mutex mutex_hornear;
	
	//variables semaforos
	sem_t sem_mezclar;
	sem_t sem_salar;
	sem_t sem_agregarCarne;
	sem_t sem_empanar;
	sem_t sem_cocinar;
	sem_t sem_cortarPan;
	sem_t sem_agregarMilanesa;
	sem_t sem_agregarOtros;
	sem_t sem_aux;
	
	//crear variables semaforos aqui
	
	//variables hilos
	pthread_t p1;
	pthread_t p2;
	pthread_t p3;
	pthread_t p4;
	//crear variables hilos aqui
	
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
	//setear demas semaforos al struct aqui
	pthread_data->semaforos_param.sem_mezclar = sem_salar;
	pthread_data->semaforos_param.sem_mezclar = sem_agregarCarne;
	pthread_data->semaforos_param.sem_mezclar = sem_empanar;
	pthread_data->semaforos_param.sem_mezclar = sem_cocinar;
	pthread_data->semaforos_param.sem_mezclar = sem_agregarMilanesa;
	pthread_data->semaforos_param.sem_mezclar = sem_cortarPan;
	pthread_data->semaforos_param.sem_mezclar = sem_agregarOtros;
	pthread_data->semaforos_param.sem_mezclar = sem_aux;
	

	//seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ¿Se ve hardcodeado no? ¿Les parece bien?
     	strcpy(pthread_data->pasos_param[0].accion, "cortar");
	    strcpy(pthread_data->pasos_param[0].ingredientes[0], "ajo");
      strcpy(pthread_data->pasos_param[0].ingredientes[1], "perejil");


	    strcpy(pthread_data->pasos_param[1].accion, "mezclar");
	    strcpy(pthread_data->pasos_param[1].ingredientes[0], "ajo");
      strcpy(pthread_data->pasos_param[1].ingredientes[1], "perejil");
 	    strcpy(pthread_data->pasos_param[1].ingredientes[2], "huevo");
	    strcpy(pthread_data->pasos_param[1].ingredientes[3], "carne");
	
	strcpy(pthread_data->pasos_param[1].accion, "salar");
	
	strcpy(pthread_data->pasos_param[1].accion, "agregarCarne");
	
	strcpy(pthread_data->pasos_param[1].accion, "empanar");
	
	strcpy(pthread_data->pasos_param[1].accion, "cocinar");
	
	strcpy(pthread_data->pasos_param[1].accion, "agregarMilanesa");
	
	strcpy(pthread_data->pasos_param[1].accion, "hornearPan");
	
	strcpy(pthread_data->pasos_param[1].accion, "cortarPan");
	
	strcpy(pthread_data->pasos_param[1].accion, "agregarOtros");
	
	//inicializo los semaforos

	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	//inicializar demas semaforos aqui
	sem_init(&(pthread_data->semaforos_param.sem_salar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_agregarCarne),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_empanar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_cocinar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_agregarMilanesa),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_cortarPan),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_agregarOtros),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_aux),0,0);

	//inicializo los mutex
	pthread_mutex_init(&mutex_salar, NULL);
	
	pthread_mutex_init(&mutex_cocinar, NULL);
	
	pthread_mutex_init(&mutex_hornear, NULL);

	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;
    rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	//crear demas hilos aqui
	rc=pthread_create(&p2;NULL,cortar,pthread_data);
	rc=pthread_create(&p3;NULL,cortar,pthread_data);
	rc=pthread_create(&p4;NULL,cortar,pthread_data);
	
	rc=pthread_create(&p1;NULL,mezclar,pthread_data);
	rc=pthread_create(&p2;NULL,mezclar,pthread_data);
	rc=pthread_create(&p3;NULL,mezclar,pthread_data);
	rc=pthread_create(&p4;NULL,mezclar,pthread_data);
	
	pthread_mutex_lock(&mutex_salar,NULL);
	rc=pthread_create(&p1;NULL,salar,pthread_data);
	pthread_mutex-unlock(&mutex_salar,NULL);
	pthread_mutex_lock(&mutex_salar,NULL);
	rc=pthread_create(&p2;NULL,salar,pthread_data);
	pthread_mutex-unlock(&mutex_salar,NULL);
	pthread_mutex_lock(&mutex_salar,NULL);
	rc=pthread_create(&p3;NULL,salar,pthread_data);
	pthread_mutex-unlock(&mutex_salar,NULL);
	pthread_mutex_lock(&mutex_salar,NULL);
	rc=pthread_create(&p4;NULL,salar,pthread_data);
	pthread_mutex-unlock(&mutex_salar,NULL);
	
	rc=pthread_create(&p1;NULL,agregarCarne,pthread_data);
	rc=pthread_create(&p2;NULL,agregarCarne,pthread_data);
	rc=pthread_create(&p3;NULL,agregarCarne,pthread_data);
	rc=pthread_create(&p4;NULL,agregarCarne,pthread_data);
	
	rc=pthread_create(&p1;NULL,empanar,pthread_data);
	rc=pthread_create(&p2;NULL,empanar,pthread_data);
	rc=pthread_create(&p3;NULL,empanar,pthread_data);
	rc=pthread_create(&p4;NULL,empanar,pthread_data);
	
	pthread_mutex_lock(&mutex_cocinar,NULL);
	rc=pthread_create(&p1;NULL,cocinar,pthread_data);
	pthread_mutex-unlock(&mutex_cocinar,NULL);
	pthread_mutex_lock(&mutex_cocinar,NULL);
	rc=pthread_create(&p2;NULL,cocinar,pthread_data);
	pthread_mutex-unlock(&mutex_cocinar,NULL);
	pthread_mutex_lock(&mutex_cocinar,NULL);
	rc=pthread_create(&p3;NULL,cocinar,pthread_data);
	pthread_mutex-unlock(&mutex_cocinar,NULL);
	pthread_mutex_lock(&mutex_cocinar,NULL);
	rc=pthread_create(&p4;NULL,cocinar,pthread_data);
	pthread_mutex-unlock(&mutex_cocinar,NULL);
	
	rc=pthread_create(&p1;NULL,agregarMilanesa,pthread_data);
	rc=pthread_create(&p2;NULL,agregarMilanesa,pthread_data);
	rc=pthread_create(&p3;NULL,agregarMilanesa,pthread_data);
	rc=pthread_create(&p4;NULL,agregarMilanesa,pthread_data);
	
	pthread_mutex_lock(&mutex_hornear,NULL);
	rc=pthread_create(&p1;NULL,hornearPan,pthread_data);
	pthread_mutex-unlock(&mutex_hornear,NULL);
	pthread_mutex_lock(&mutex_hornear,NULL);
	rc=pthread_create(&p2;NULL,hornearPan,pthread_data);
	pthread_mutex-unlock(&mutex_hornear,NULL);
	pthread_mutex_lock(&mutex_hornear,NULL);
	rc=pthread_create(&p3;NULL,hornearPan,pthread_data);
	pthread_mutex-unlock(&mutex_hornear,NULL);
	pthread_mutex_lock(&mutex_hornear,NULL);
	rc=pthread_create(&p4;NULL,hornearPan,pthread_data);
	pthread_mutex-unlock(&mutex_hornear,NULL);
	
	rc=pthread_create(&p1;NULL,cortarPan,pthread_data);
	rc=pthread_create(&p2;NULL,cortarPan,pthread_data);
	rc=pthread_create(&p3;NULL,cortarPan,pthread_data);
	rc=pthread_create(&p4;NULL,cortarPan,pthread_data);
	
	rc=pthread_create(&p1;NULL,agregarOtros,pthread_data);
	rc=pthread_create(&p2;NULL,agregarOtros,pthread_data);
	rc=pthread_create(&p3;NULL,agregarOtros,pthread_data);
	rc=pthread_create(&p4;NULL,agregarOtros,pthread_data);
	
	//join de todos los hilos
	pthread_join (p1,NULL);
	pthread_join (p2,NULL);
	pthread_join (p3,NULL);
	pthread_join (p4,NULL);
	//crear join de demas hilos


	//valido que el hilo se alla creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }

	 
	//destruccion de los semaforos 
	sem_destroy(&sem_mezclar);
	sem_destroy(&sem_salar);
	sem_destroy(&sem_agregarCarne);
	sem_destroy(&sem_empanar);
	sem_destroy(&sem_cocinar);
	sem_destroy(&sem_cortarPan);
	sem_destroy(&sem_agregarMilanesa);
	sem_destroy(&sem_agregarOtros);
	sem_destroy(&sem_aux);
	//destruir demas semaforos
	
	//destruirMutex
	pthtread_mutex_destroy(&mutex_salar,NULL);
	pthtread_mutex_destroy(&mutex_cocinar,NULL);
	pthtread_mutex_destroy(&mutex_hornear,NULL);
	
	
	//salida del hilo
	 pthread_exit(NULL);
}


int main ()
{
	//creo los nombres de los equipos 
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
//faltan equipos
  
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;

	//creo las variables los hilos de los equipos
	pthread_t equipo1; 
	pthread_t equipo2;
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
	rc = pthread_create(&equipo3,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre3);
	rc = pthread_create(&equipo4,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre4);
  //faltn inicializaciones


   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     } 

	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);
	pthread_join (equipo4,NULL);
//.. faltan joins


    pthread_exit(NULL);
}


//Para compilar:   gcc subwayArgento.c -o ejecutable -lpthread
//Para ejecutar:   ./ejecutable