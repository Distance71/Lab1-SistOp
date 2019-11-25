Preguntas

Parte 1

1) Pregunta: ¿cuáles son las diferencias entre la syscall execve(2) y la familia de wrappers proporcionados por la librería estándar de C (libc) exec(3)?

Rta: La syscall execve es una syscall que el programador puede realizar explicitamente al kernel a través de la biblioteca unistd. Como tal, es un modo de acceder directamente al nivel más bajo del sistema operativo. Luego, su implementación es genérica. Las funciones de la familia exec, por otro lado, son implementaciones “front end” de la syscall antes mencionada, y como tales poseen implementaciones con ciertas caracteristicas y funcionalidades puntuales, además de operar con otro mecanismo.

Parte 2

1) Pregunta: ¿entre cd y pwd, alguno de los dos se podría implementar sin necesidad de ser built-in? ¿por qué? ¿cuál es el motivo, entonces, de hacerlo como built-in? (para esta última pregunta pensar en los built-in como true y false)

Rta: En principio, no. Esto es debido a que una función externa no tendria la posibilidad de modificar el entorno de una shell (cambiar el directorio de trabajo implica modificar el entorno). El motivo esta relacionado a una cuestión de permisos y diseño de la Shell.

2) Pregunta: ¿por qué es necesario hacerlo luego de la llamada a fork(2) ?

Rta: De no hacerlo, no impactaría en el proceso a ejectuar. 

Responder:
    • 3) ¿El comportamiento es el mismo que en el primer caso? Explicar qué sucede y por qué.
      Rta: En principio, diría que no daría, además, acceso a las variables de environ, pero debería probarlo.
    • 4) Describir brevemente (sin implementar) una posible implementación para que el comportamiento sea el mismo. 
	Rta: Se podría hacer una inclusión de las variables de environ en la lista de envp.


5) Detallar cúal es el mecanismo utilizado.

Rta: Se modificó la función exec_cmd(). Se eliminó la espera de waitpid() en ese caso. 

Parte 3

1) Investigar el significado de este tipo de redirección y explicar qué sucede con la salida de cat out.txt. Comparar con los resultados obtenidos anteriormente.

Rta: Se obtiene el mismo resultado combinado de los casos anteriores, ya que se combina stderr con stdout, con el mismo canal de salida. 

2) Investigar, describir y agregar la funcionalidad del operador de redirección >> y &>

Rta: El operador >> tiene la misma funcionalidad que el operador >, con excepción del hecho de que concatena el contenido. El operador &> tiene el mismo efecto que 2>&1, con excepción del cambio en la sintaxis, que se traduce en colocar &>nomb_archivo. 

3) Investigar y describir brevemente el mecanismo que proporciona la syscall pipe(2), en particular la sincronización subyacente y los errores relacionados.

Rta: Permite la generación de un canal de comunicación. La dificultad radica en los tiempos y la sincronización de la función de salida, con la de entrada. Esto se resuelve operando con señales (en particular, wait del otro proceso). 

Parte 4
Investigar al menos otras dos variables mágicas estándar, y describir su propósito. 
Rta: Una variable es $$, que devuelve el pid del proceso. Otra es $#, que devuelve la cantidad de argumentos en linea de comando.

Preguntas:
    • Explicar detalladamente cómo se manejó la terminación del mismo.
      Rta: Se generó una alerta de un proceso. Una vez se produjo la finalización del proceso, se dispara una señal de alerta, que a su vez invoca a una función que emite un mensaje.
    • ¿Por qué es necesario el uso de señales?
      Rta: Porque de lo contrario no se puede determinar la finalización del proceso.

