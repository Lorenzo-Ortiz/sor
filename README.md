# SOR-Semáforos-1S-2021
Trabajo Práctico Semáforos primer semestre año 2021

# Alumno: Lorenzo Ortiz,



*PROYECTO*

El proyecto que este caso se está desarrollando se trata de una competencia para el armado de un sandwich de milanesa. En dicha competencia participan 4 equipos, los cuales desarrollando distintos pasos para lograr el armado de la milanesa deben ir turnandose en el uso de algunos de los instrumentos/utencillos de cocina, para lo cual se va a hacer uso de semaforos y mutex. El primero equipo en terminar tiene que ser informado y sus integrantes lograran un puesto de trabajo. 
Toda la informacion del proceso será guardado en un documento.



*ACCIONES

En primer lugar vamos a definir las acciones que tenemos que llevar a cabo, para luego convertirlas en funciones y sincronizarlas.


cortar ajo perejil.

mezclar con el huevo.

salar.

agregar carne.

empanar.

cocinar.

hornear pan.

armar milanesa.


*SEMAFOROS*

Un dato importante a tener en cuenta es que hay acciones que los equipos no van a poder implementarlas en simultaneo.
En tal caso van a tener que aguardar que, en el caso de que se esten llevando a cabo, que otro equipo termine.
Para ello resulta necesario utilizar los semaforos Mutex asi de esta forma evitamos el uso simultaneo de las acciones/funciones.
En nuestro ejercicio, las acciones limitadas en el uso son las de salar, cocinar y hornear pan.
Las demas las sincronizamos con semáforos.

# Pseudocodigo y prueba de escritorio:

    sem_aux=    	          00111111111000 

    sem_mezclar=	       	  10000000000000

    sem_salar=	          00100000000000

    sem_agrcarne=	          00001000000000

    sem_empanar=	          00000010000000

    sem_cocinar=	          00000000100000

    sem_armarMilanesa=	          00000000001000

    sem_hornear=	          10000000000000
    
    sem_ganador=                  00000000000010




                                      lock(salar)		   	              lock(cocinar)	 lock(hornear)       v_armarMil
 
                    v_mezclar         v_salar        v_agrcarne      v_empanar	   v_cocinar                               v_aux                 v_ganador

      cortar()      mezclar()         salar()        agrcarne()       empanar()       cocinar()           hornear()          armarMilanesa()     ganar()

      s_mezc        s_salar           s_agrcarne     s_empanar        s_cocinar       s_armarMil          sem_aux.           sem_ganador

	                              unlock(salar)			              unlock(cocinar)     unlock(hornear)
 

sem_wait()=v;

sem_post()=s; 

mutex_lock()=lock

mutex_unlock()=unlock 


*MUTEX*

Como antes mencionamos, se tuvo que utilizar distintos mutex para controlar las acciones en el armado del sandwich pero tambien para coordinar otras acciones. A continuacion se detallan los mutex que se utilizaron:

	pthread_mutex_t mutex_salar = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_cocinar = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_hornear = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t escribir = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t ganador = PTHREAD_MUTEX_INITIALIZER;


*GUARDAR SALIDA*

Todos los procesos que se fueron desarrollando se van a guardar en un documento txt que se encuentra subido al proyecto. El archivo esta guardado con el nombre archivo.txt



*DIFICULTADES Y CONCLUSIONES*

Dentro de las principales consecuencias que encontré en el armado del proceso fue la de incluir los mutex y hacerlos funcionar de la manera adecuada. No podia descifrar la ubicacion dentro del proyecto. 
Por otra parte, me tomó mucho tiempo tratar de utilizar algun metodo para poder levantar los ingredientes de una receta de formato txt. Este fue un gran desafío que lo pude resolver.
Con respecto al trabajo, se trato de una experiencia bastante ludica y entretenida. Involucra mucha atencion, constante testeo y precision para no evitar fallas.

//Para compilar:   gcc subwayArgento.c -o ejecutable -lpthread
//Para ejecutar:   ./ejecutable

    
