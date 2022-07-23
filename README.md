# CI0117 - Proyecto 2: Battle Royale de Procesos [super_mario_mpi]

**Fecha de entrega:** Viernes 4 de Diciembre 11:50pm.

## Equipo de desarrollo

* **Juan Ignacio Pacheco (B85841)**
* **David Xie Li (B88682)**

Este proyecto es parte del curso de CI-0117 Programación Paralela y Concurrente de la Universidad de Costa Rica. Realizado durante el segundo semestre del año 2020.

## Manual de usuario

Información sobre cómo utilizar el programa (entradas esperadas e interpretación de resultados).

Primero que todo, en caso de no tener la biblioteca de mpi instalado, hacerlo con el siguiente comando:

        $ sudo apt-get install -y mpi


### Compilación

En caso de que se quiera correr en el clúster o en su computadora, si no se está compilando correctamente, debe poner un 1 en la constante definida por ```#define JOCAN ``` que se encuentra en el archivo GameController.cpp, de forma que la biblioteca de mpi pueda ser encontrada.

La manera de compilar la solución es a travéz del Makefile. Para compilar solo ocupa poner el siguiente comando desde la raíz del proyecto:

        $ make

Una vez hecho make, aparecerán dos carpetas extra, una llamada **bin** que contendrá el archivo ejecutable y otra llamada **build** que tendrá los archivos .o.

Para borrar las carpetas **bin** y **build** se usa el siguiente comando:

        $ make clean


### Ejecución

El nombre del ejecutable, una vez compilado, es **super_mario_mpi**, y se encuentra dentro de la carpeta **bin**, el cual para ejecutarse se invoca desde la raíz del proyecto y de la siguiente forma:

        $ mpiexec -n numProcesses ./super_mario_mpi marioToSee strategy [cluster]

Cada uno de los argumentos anteriores representa lo siguiente:
- numProcesses: indica el número de procesos que ejecutará el programa.
- marioToSee: es el número del rango para el cuál se mostrará la información de la simulación, este debe ser un número entre \[1, numProcesses - 1\].
- strategy: es un carácter que representará la estrategia de ataque a usar, las opciones que admite son *R* (aleatorio), *L* (menos monedas), *M* (más monedas) y *A* (atacante). En caso de que se introduzca un carácter que no está disponible, se pondrá *R* por default.
- cluster: si este programa se correrá en clúster, o si se quiere dejar a decisión del programa la decisión de cuál jugador visualizar una vez que haya muerto un Mario, se deberá poner como último parámetro la palabra *cluster* con todo en minúscula.

Note que únicamente el parámetro \[cluster\] es opcional, los otros son obligatorios para que el programa pueda correr.


### Entradas esperadas

No se esperan entradas adicionales al programa, a menos que se esté corriendo en terminal y no en clúster, una vez que el Mario a visualizar muere y si quedan Marios vivos, se le pedirá escoger un valor númerico, entre la lista de Marios vivos mostrado, con el número del próximo Mario que se quiera visualizar. Ejemplos de ejecución del programa son los siguientes:

```

$ mpiexec -n 8 bin/super_mario_mpi 2 R
$ mpiexec -n 10 bin/super_mario_mpi 5 L cluster
```

Para el primer comando, se indica que se quieren correr 8 procesos, que se quiere visualizar el Mario 2 y que la estrategia de ataque de este Mario será aleatorio. Además, correrá en terminal por lo que seremos los encargados de indicar el próximo Mario a ver si quedan más de 1 vivo.

Por otra parte, para el segundo ejemplo, se indica que se correrán 10 procesos, que se visualizará el proceso 5, su estrategia de ataque será al que posea menos monedas y correrá en clúster, por lo que el siguiente Mario a visualizar será seleccionado al azar.


### Interpretación de resultados

Conforme el programa corre, se va mostrando en la terminal todas las acciones que realiza el Mario indicado en la línea de comandos, un ejemplo es el siguiente

```
World pos. 5: Mario #1 is walking. Coins: 0 | atacking # 4 | being attacked by #2 | attack strategy: LESS COINS | Total playing: 4
```

note que se muestra la posición en el mundo en el que está actualmente, el número del rango del Mario que está corriendo, la acción que está realizando dependiendo de los elementos del mundo con que se encuentre, la cantidad de monedas que lleva, a cuál Mario está atacando, por cuál Mario está siendo atacado, su estrategia de ataque, y la cantidad total de Marios que siguen vivos, todo esto hasta que sea el último sobreviviente o muera, contra cada posición del mundo. En caso de que muera, se selecciona otro Mario de la lista de vivos que se despliega, en caso de no correr en clúster, y una vez que queda sólo un proceso vivo, el programa termina y muestra el rango del proceso ganador con el siguiente mensaje de ejemplo:

```
|**HAY UN GANADOR: es el Mario #2 \o/**|
```

## Detalles de la solución

### Estructura del programa

El programa se encuentra estructurado en clases, cada una con su respectivo .h y .cpp. En el .h se declaran las funciones, los atributos de la clase y contiene la documentación del programa y en el .cpp se encuentran la implementación de dichas funciones.
Estos archivos .cpp y .h se encuentran en la carpeta src/.
Una vez que se compila el archivo ejecutable, este se encuentra en la carpeta bin/ y los distintos .o de cada clase se encuentra en build/.
Además, dentro de la carpeta Worlds, tenemos los archivos .txt de los stages de los mundos, actualmente sólo se cuenta con el archivo *Stage1-1.txt*, el cuál se adaptó de la imagen proveída por el profesor Jose Andrés Mena. El formato de los mundos consiste en el número de posiciones del mundo en la primera línea, y en cada una de las siguientes líneas se indica el número de elementos que se encuentran en la posición y letras separadas por espacios que representan cada elemento. Las letras son

- N: Empty.
- G: Goomba.
- K: Koopa.
- C: Coin.
- H: Holes.


Los archivos que pertenecen al folder src son: CoinElement.cpp, CoinElement.h, GameController.cpp, GameController.h, GoombaElement.cpp, GoombaElement.h, HoleElement.cpp, HoleElement.h, KoopaTroopaElement.cpp, KoopaTroopaElement.h, main.cpp, Mario.cpp, Mario.h, World.cpp, World.h, WorldElement.cpp y WorldElement.h.

Los archivos que pertenecen al folder World son: Stage1-1.txt.

### Descripción de los componentes

- **Game Controller:**

Este módulo corresponde al controlador del juego y contiene la función que se encarga de la creación del World, el Mario, los WorldElements, de la sincronización de los procesos y se encarga también de la destrucción de todas las estructuras creadas en memoria dinámica para evitar fugas de memoria.

- **Mario:**

Este módulo corresponde al Mario y al jugador. Contiene todos los atributos de Mario: monedas, posición de Mario en el mundo, la identificación (rango), la identificación del enemigo que estoy atacando, el número de procesos, si esta Mario vivo y la estrategia de ataque.

- **World:**

Este módulo corresponde al Mundo. Contiene todos los atributos de World: la posición actual a procesar del mundo, la posición de la bandera y el vector de colas que representa lo que se encuentra en cada posición. Se encarga de inicializar el mundo, de crear y agregar en el mundo los World Elements del archivo provisto, y de agregar goombas y koopas conforme corre el juego según corresponda.

- **World Element:**

Este módulo corresponde a una clase abstracta de la cuál se heredarán sus funciones y que corresponde cada uno de los elementos del mundo que interactuarán con Mario. Contiene por definir las funciones que va a hacer cada uno de los obstáculos que se encuentre Mario en el mundo. Más adelante vamos a ver como diferentes clases hijas definen su funciones para sobreescribir las de su clase padre. Estas funciones consisten en la probabilidad que tiene por acción, qué acción puede hacer y el string asociado con tal acción para ser impreso.

- **Coin Element:**

Este módulo hereda de World Element. Contiene las acciones de Mario sobre si obtiene una moneda o no. Para ello también tiene las probabilidades entre obtenerla o no.

- **Goomba Element:**

Este módulo hereda de World Element. Contiene las acciones de Mario sobre si muere ante un goomba, esquiva el goomba o mata el goomba. Para ello también tiene las probabilidades entre los distintos casos. En caso de matar a Mario le asigna un 0 al Alive. En caso de morir se fija a quien le tiene que sumar 1 goomba al contrincante.

- **Koompa Troopa Element:**

Este módulo hereda de World Element. Contiene las acciones de Mario sobre si muere ante un koopa, esquiva el koopa o mata el koopa. Para ello también tiene las probabilidades entre los distintos casos. En caso de matar a Mario le asigna un 0 al Alive. En caso de morir se fija a quien le tiene que sumar 1 koopa al contrincante.

- **Hole Element:**

Este módulo hereda de World Element. Contiene las acciones de Mario sobre si salta y esquiva el hueco o no salta y muere en el hueco. Para ello también tiene las probabilidades entre los distintos casos.

### Interacción entre los componentes

El GameController, que es llamado por el main, empieza la ejecución del programa. Una vez inicializado MPI, GameController crea un World llamado myWorld y un Mario llamado myMario. Tiene varios componentes de MPI para sincronizarse con los demás procesos. Los jugadores (que está en el GameController) mientras van corriendo crean una cola llamada myQueue que tiene todos los elementos de la siguiente posición.

Cada elemento del WorldElement se crea en un método de GameController llamado advancePosition. Y conforme va por cada posición, se le pide al mundo por la siguiente a procesar, esto ocurre con todos los procesos y una vez que queda un único sobreviviente, el programa elimina todo lo alocado en memoria dinámica y finaliza MPI junto con la ejecución del programa, después de haber anunciado al ganador.


## MPI

Todas las funciones de MPI que se usaron se encuentra dentro del archivo GameController.cpp.

- Se tienen las funciones ```MPI_Init(&argc, &argv);``` para inicializar MPI, ```MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);``` para saber la cantidad de procesos y ```MPI_Comm_rank(MPI_COMM_WORLD, &myRank);``` para que cada proceso sepa qué número de rango es.

- En el método validateInputData hay un ```MPI_Bcast(&go, 1, MPI_CHAR, 0, MPI_COMM_WORLD);``` para que todos los procesos sepan si los parámetros de inicio fueron correctos, en caso de no serlo todos terminan.

- Después del validateInputData, en el método run, se tiene dos MPI\_Bcast, ```MPI_Bcast(&strategy, 1, MPI_CHAR, 0, MPI_COMM_WORLD);``` y ```MPI_Bcast(&marioToSee, 1, MPI_INT, 0, MPI_COMM_WORLD);```. El primero, para darle a saber a todos la estrategia que va a usar el proceso que el usuario quiere ver y el segundo para pasar el número del proceso que el usuario quiere observar.

Antes de entrar a la sección que divide el réferi de los jugadores, se entra a un método que es el synchronize. Este método es el encargado de que todos los procesos se pasen toda la información que ocupan y es el que tiene las funciones importantes de MPI.

- De primero en syncronize tenemos ```MPI_Allgather(&coins, 1, MPI_INT, this->mariosCoins, 1, MPI_INT, MPI_COMM_WORLD);``` para recolectar en un array la cantidad de monedas que tienen todos. Esto se va a usar después para que los jugadores con estrategia que tienen que ver con monedas apunten correctamente.

- De segundo tenemos ```MPI_Allgather(&alive, 1, MPI_INT, this->mariosAlive, 1, MPI_INT, MPI_COMM_WORLD);``` para recolectar en un array cuáles marios están vivos.

- De tercero y cuarto tenemos dos MPI\_Allreduce, ```MPI_Allreduce(goombasSendBuffer, this->goombas, numProcesses, MPI_INT, MPI_SUM, MPI_COMM_WORLD);``` y ```MPI_Allreduce(koopasSendBuffer, this->koopaTroopas, numProcesses, MPI_INT, MPI_SUM, MPI_COMM_WORLD);```, que lo que hacen es sumar los arreglos de goombasSendBuffer y koopasSendBuffer a los arreglos que todos tienen de this->goombas y this->koopas. De esta manera todos los mundos de cada Mario, después de estos MPI\_Allreduce, saben cuántos goombas y koopas tienen que sumarse.

- De quinto está el ```MPI_Allgather(&enemyRank, 1, MPI_INT, this->attacking, 1, MPI_INT, MPI_COMM_WORLD);``` del enemigo que cada Mario esta atacando y se hace un array que tiene a quién se está atacando.

- De sexta función está el ```MPI_Bcast(&marioToSee, 1, MPI_INT, 0, MPI_COMM_WORLD);``` que mantiene igual o actualiza el mario que se quiere ver en terminal. Se mantiene igual si el mario no muere, si muere, el mario es cuando se actualiza.

Este método de synchronize lo corre tanto el réferi como los jugadores, como los jugadores que quedan eliminados para que no ocurra un starvation.

Finalmente, se cierra el ambiente de MPI utilizando la función ```MPI_Finalize();```.


## Licencia

CC BY Proyecto 02 Programación Paralela y Concurrente - Battle Royale de Procesos [super_mario_mpi]
