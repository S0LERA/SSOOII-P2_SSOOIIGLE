# SSOOII-P2_SSOOIIGLE
Repositorio para la segunda pŕactica de la Asignatura Sistemas Operativos II de la ESI (UCLM).

Se desea implementar un buscador de palabras multihilo en un fichero, que cuente con gestión de la concurrencia entre los hilos que funcione por línea de comandos.

## Uso:
`make compilar`
 - Para compilar el programa

 El archivo donde se quiere buscar debe estar en la ruta del makefile.

`make SS00GLE FICHERO=prueba2 PALABRA=trabajo HILOS=4`
 - FICHERO: es la ruta del fichero donde se va a buscar la palabra, sin el .txt.
 - PALABRA: es la palabra a buscar.
 - HILOS = es el numero de hilos que se desea que busquen.

## Versiones:
### Versión 1.5:
- Versión del programa a falta de retoques finales y formateo de la salida.

### Versión 2.0:
- El programa está completo. Se puede ejecutar desde el make, el formateo de salida está listo.
