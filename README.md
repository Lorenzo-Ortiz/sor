# SOR-Semáforos-1S-2021
Trabajo Práctico Semáforos primer semestre año 2021

# Alumno: Lorenzo Ortiz,

En primer lugar vamos a definir las acciones que tenemos que llevar a cabo, para luego convertirlas en funciones y sincronizarlas.

Acciones:

cortar ajo perejil.

mezclar con el huevo.

salar.

agregar carne.

empanar.

cocinar.

hornear pan.

armar milanesa.

Un dato importante a tener en cuenta es que hay acciones que los equipos no van a poder implementarlas en simultaneo.
En tal caso van a tener que aguardar que, en el caso de que se esten llevando a cabo, que otro equipo termine.
Para ello resulta necesario utilizar los semaforos Mutex asi de esta forma evitamos el uso simultaneo de las acciones/funciones.
En nuestro ejercicio, las acciones limitadas en el uso son las de salar, cocinar y hornear pan.
Las demas las sincronizamos con semáforos.

# Pseudocodigo y prueba de escritorio:

    sem_aux=    	          001111111110 

    sem_mezclar=	       	  100000000000

    sem_salar=	        	  001000000000

    sem_agrcarne=	          000010000000

    sem_empanar=	          000000100000

    sem_cocinar=	          000000001000

    sem_armarMilan=	          000000000010

    sem_hornear=	          100000000000




                                      lock(salar)		   	              lock(cocinar)	 lock(hornear)       v_armarMil

                    v_mezclar         v_salar        v_agrcarne      v_empanar	      v_cocinar                              v_aux

      cortar()      mezclar()         salar()        agrcarne()       empanar()       cocinar()           hornear()          armarMilanesa()

      s_mezc        s_salar           s_agrcarne     s_empanar        s_cocinar       s_armarMil          sem_aux.     

	                              unlock(salar)			              unlock(cocinar)     unlock(hornear)
 

sem_wait()=v;

sem_post()=s; 

mutex_lock()=lock

mutex_unlock()=unlock 
