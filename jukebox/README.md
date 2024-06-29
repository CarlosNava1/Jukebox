 LA MEJORA DE JUKEBOX

## Authors

* **Carlos Navarro Juan** 
 


Ponga una breve descripción del proyecto **aquí** en castellano e inglés.

En proyecto de jukebox es un sistema multifuncional que que integra interacción de usuario, comunicación serial y reproducción de sonido, todo ello optimizado para un consumo eficiente de energía.La idea inicial es intentar poder reproducir música pulsando un botón y además hacerlo de forma eficiente de energía. Para poder llevar a cabo el proyecto se van a implementar 5 versiones y en cada una de ellas se tratará de implementar una funcionalidad nueva.
 -**Versión 1**: Se llevará a cabo un botón con el que se podrá controlar la reproducción de canciones en el buzzer. Para ello se definirá una máquina de estados en el common y un port. Además de definir el manejo a la interrupción.
-**Versión 2**: En la segunda versión se implementará el funcionamiento de la USART que será necesario para la comunicación serial. 
-**Versión 3**: La implementación del buzzer será imprescindible para el correcto funcionamiento de la Jukebox, que es lo que se trata de implementar en esta versión
-**Versión 4**: En esta versión se tratará de hacer el funcionamiento de todo más eficiente desde el punto de vista energético, así como llevar a cabo la integración del software.
-**Versión 5**: Esta última versión es de libre elección por parte del alumnado y en nuestro caso hemos optado por desarrollar diversas funcionalidades del software, que van a mejorar nuestro proyecto. 


Puede añadir una imagen de portada **de su propiedad** aquí. Por ejemplo, del montaje final, o una captura de osciloscopio, etc.

**Las imágenes se deben guardar en la carpeta `docs/assets/imgs/` y se pueden incluir en el documento de la siguiente manera:**


![Captura de osciloscopio](docs/assets/imgs/captura.bmp)

![Montaje final](docs/assets/imgs/montaje.jpeg)



**Añada un enlace a un vídeo público de su propiedad aquí con la demostración del proyecto explicando lo que haya hecho en la versión V5.**

 


## Version 1
La Versión 1 del proyecto Jukebox se centra en la implementación inicial del sistema con la funcionalidad del botón de usuario. Este botón permite interactuar con el reproductor para encender/apagar el dispositivo y cambiar la canción. Para llevar a cabo funcionalidad del botón se ha divido en dos el código que será COMMON y PORT, en el Common se va a describir la máquina de estados (FSM) y en la parte de PORT se va a tener el Hardware del sistema. 
# COMMON
En la parte del common se define la máquina que tendrá los siguientes estados:
-**BUTTON_RELEASED** Estado inicial donde la FSM verifica si el botón ha sido pulsado.
-**BUTTON_PRESSED_WAIT** Espera durante el tiempo de antirrebotes.
-**BUTTON_PRESSED** Se detecta que el botón ha sido presionado completamente.
-**BUTTON_RELEASED_WAIT** Espera durante el tiempo de antirrebotes después de soltar el botón.
En esta máquina se van a definir las transiciones que harán posible el buen funcionamiento de nuestro botón. En esta parte se tendrán funciones como **check_button_pressed()** para ver si el botón está pulsado, **do_store_tick_pressed()** que almacena el tiempo de almacenamiento del botón,**check_button_released()** se verifica si el botón ha dejado de estar pulsado o **do_set_duration()** que almacena la duración de la ppulsación del botón. 
# PORT 
En la parte del Port se implementa el código teniendo en cuenta su hardware para ello se crea un array de botones, para después  utilizar funciones como **port_button_init()** para la inicialización,**port_button_is_pressed()** para ver si está pulsado y **port_button_get_tick()**  para obtener el tiempo de pulsación. En el Interr.C sse implementa **EXTI15_10_IRQHandler** para configurar el manejo de las interrupciones del botón. 



## Version 2
En la versión 2 se ha implementado la funcionalidad de la USART que será necesaria para hacer posible la comunicación serie entre el microcontrolador y otros dispositivos como por ejemplo ordenadores. Para la implementación de esta, será necesario configurar la USART de manera adecuada sabiendo que los pines serán PB10 para transmisión y PC11 para recepción, se habilita pull-up para ambos pines y el baudrate será de 9600 baudios entre otras caracteríticas.
# COMMON
 En la parte Common, se va a desarrollar la FSM que describirá la máquina de estados. Como estados se va a tener **WAIT_DATA** y **SEND_DATA**.En la máquina se van a definir los estados necesarios para hacer la transmisión y recepción de datos.Las funciones del fsm procesarán los datos recibidos-Además de mandar los datos necesarios-. 
# PORT y montaje
En la parte Port, se adapta la lógica de la comunicación al hardware con el que se cuenta.En el port_usart se puede habilitar las interrupciones de transmisión y recepción necesarias así como hacer uso de funciones tan importante como store_Data -que almacena los datos de salida en un buffer- o el write_data que escribe los datos de salida de la USART. En cuanto a la parte del interr.c mencionar que se va a desarrollar la función  **USART3_IRQHandler** que deberá manejar la interrrupción de la comunicación, limpiando el registro al final de esta función. Por último del montaje habrá que tener en cuenta que tanto el microcontrolador como la USART deben estar alimentadas y conectadas. 


## Version 3
En la versión 3 se va a desarrollar el buzzer que será el encargado de reproducir las canciones pedidas de melodies.c. El pin utilizado será el PIN PA6, así como los temporizadores 2 y 3-cada uno de ellos para una función diferente-. La reproducción de una nota, se realiza mediante una señal periódica que se repite a una frecuencia determinada, esta señal determina la nota musical que se está reproduciendo y se genera como una señal digital PWM. 
# COMMON
De nuevo se va desarrollar una parte de common -fsm para la máquina de estados- y una parte de port teniendo la parte de hardware en cuenta. En el fsm se harán funciones check y do para la máquina de estados. Los estados definidos serán:
-WAIT START
-WAIT NOTE
-PLAY NOTE
-PAUSE NOTE 
-WAIT MELODY
# PORT y montaje
En la parte de Port se declara un array de buzzers-al igual que en anteriores versiones con sus respectivas implementaciones-, que contiene las características de cada buzzer. En esta parte se utilizá funciones como **port_buzzer_set_note_duration()** -para establecer de la nota correspondiente o **port_buzzer_set_note_frequency( )** para establecer la frecuencia de la nota. También se añadiran funciones para parar o inicializar el buzzer, además de funciones privadas para configurar el canal o para configurar la interrupción del fin de la nota. En el *interr.C* se implementará la función **TIM2_IRQHandler()** para las interrupciones del buzzer. En cuanto al montaje se debe conectar el buzzer al micro para poder recibir sus indicaciones.


## Version 4
En la Versión 4 del proyecto Jukebox, se implementan modos de bajo consumo para mejorar la eficiencia energética del sistema.Esta versión, también incluye la integración final de todos los componentes desarrollados en las versiones anteriores (botón, USART y buzzer) en una máquina de estados central. 
# COMMON
De nuevo se va a desarrollar un fsm donde se va implementar la máquina de estados, con e estados como:
-OFF
-START_UP
-WAIT_COMMAND
-SLEEP_WHILE_ON 
-SLEEP_WHILE_OFF.
 El sistema entra en **SLEEP_WHILE_ON** si detecta que todas las FSM  de los distintos componentes están inactivas mientras el dispositivo está encendido. En el caso de **SLEEP_WHILE_OFF** es Similar al estado anterior, pero se aplica cuando el dispositivo está apagado. También cabe remarcar que el Systick es desactivado durante estos estados evitando que se despierte inecesariamente. Los estados inactivos de las FSM son **BUTTON_RELEASED** para el botón,**WAIT_DATA** para usart y **PLAY_NOTE** cuando el buzzer es el único activo.
 # INTEGRACIÓN
Para la integración final la FSM jukebox actuará como controlador central. En el main.c se creará las distintas componentes para luego aplicar un 'fire' en un bucle infinito, para después 'destruir' cada una de ellas. En cuanto al montaje se 


## Version 5

En la versión 5 hemos implementado *5 funcionalidades extra de software*. 

# PRIMERA IMPLEMENTACIÓN: COMANDO PARA RETRASAR LAS CANCIONES A LA ANTERIOR
Para nuestra versión 5 hemos implementado la función **_set_before_song()**  para poder ir a la canción anterior, para ello hemos desarrollado un código en el que realizamos un bucle for para poder saber cuantas canciones disponemos en melodies.c y saber cual es la melodía anterior a la actual. Con este nuevo índice ya podemos saber cual es la canción que se debe reproducir para cumplir lo que se desea. Además de hacer esta función, también se modifica el **_execute_command()** para que la jukebox detecte el commando before, de la siguiente manera: **else if (strcmp(p_command, "before") == 0){_set_before_song(p_fsm_jukebox); return;}** Así se puede detectar el comando before.

# SEGUNDA IMPLEMENTACIÓN: COMANDO PARA ELEGIR LA CANCIÓN MÁS LARGA
También se ha implementado la opción de poder elegir la canción más larga escribiendo el parámetro "longest", para ello se ha creado una función llamada **_set_longest_song()** que recorre todas las canciones existentes para saber cual de todas es la de mayor tamaño y guardar su índice en una variable para después seleccionarla y hacer que suene. Como antes también, será necesario añadir en el **_execute_command()** el código **else if (strcmp(p_command, "longest") == 0){_set_longest_song(p_fsm_jukebox); return;}** para que al utilizar *longest* se llame a la función correspondiente. 

# TERCERA IMPLEMENTACIÓN: COMANDO PARA ELEGIR LA CANCIÓN MÁS CORTA
En esta tercera implementación se ha apostado por una función que haga sonar a la canción más corta existente con la función **_set_shortest_song()** que se recorren todas las canciones que existen para determinar cual es la de menor tamaño, en esta función se debe comprobar con un if que la longitud de la canción es mayor que cero para evitar problemas futuros. Una vez más será necesario añadir en el **_execute_command()** la parte de código **else if (strcmp(p_command, "shortest") == 0){_set_shortest_song(p_fsm_jukebox); return;}**

# CUARTA IMPLEMENTACIÓN: COMANDO PARA MOSTRAR POR CONSOLA TODAS LAS CANCIONES CONTENIDAS EN melodies.c
En la cuarta implementación se va a tratar de crear una función que va a devolver una lista con todas las canciones que tiene melodies.c. Esta función se llamará **album()** que va a mirar todas las canciones que tiene melodies.c y las va a devolver por pantalla con su nombre. Una vez más para llevar a cabo esta implementación también será necesario modificar el **_execute_command()** añadiendo **else if (strcmp(p_command, "album") == 0)** que llamará a la función album para depués devolver todas las canciones.

# QUINTA IMPLEMENTACIÓN:  AÑADIR CANCIONES AL melodies.c
Como quinta implementación se han añadido tres canciones a melodies.c con lo que se podrán reproducir más canciones. Las tres canciones que se han añadido son de las películas *HARRY POTTER*, *STAR WARS* Y *TITANIC*. Para añadir estas canciones, se ha buscado las partituras de cada una de ellas y sus tiempos, estos valores se han introducido en nuevos arrays del melodies.c para hacer que suenen estas canciones.
