# Programa de Imagen Secreta Compartida en C

Este es un programa en lenguaje C que implementa el algoritmo de Imagen Secreta Compartida descrito en el documento _(k,n) secret image sharing scheme capable of cheating detection_ de Yan-Xiao Liu, Quin-Dong Sun y Ching-Nung Yang de la Universidad de Tecnología de Xi'an (China).

## Descripción del algoritmo

El algoritmo de Imagen Secreta Compartida es un método de criptografía que permite dividir una imagen BMP en múltiples fragmentos, conocidos como sombras, y distribuirlas entre un grupo de personas. Para recuperar la imagen original, es necesario reunir una cantidad mínima de sombras, establecida previamente, y combinarlas. En esta implementación, las sombras están ocultas estenográficamente en otras imágenes.

Este programa implementa el algoritmo descrito en el documento mencionado anteriormente, que utiliza un esquema (k,n) capaz de detectar intentos de fraude o manipulación. El programa permite generar sombras a partir de una imagen de entrada y recuperar la imagen original a partir de un conjunto de sombras proporcionado por los usuarios.

# Instrucciones de uso

Para compilar y ejecutar el programa:

1. Clonar el repositorio en local.
2. Crear un directorio bin/ para almacenar los archivos compilados: `mkdir bin`
3. Compilar el proyecto: `make all`
4. Ejecutar el programa: `./bin/main [-d | -r] [options]`, donde los argumentos posibles (cuyo orden es independiente) son:
   - `-d`: Distribuye una imagen secreta en un conjunto de imágenes.
   - `-r`: Recupera una imagen secreta a partir de un conjunto de imágenes.
   - `-k [número]`: Especifica el número de imágenes necesarias para recuperar la imagen secreta.
   - `-i [path]`: Especifica la ruta de entrada para la imagen secreta (un directorio cuando se usa -r, un archivo cuando se usa -d).
   - `-o [path]`: Especifica la ruta de salida para la imagen secreta (un archivo cuando se usa -r, un directorio cuando se usa -d).
5. Se puede correr `./bin/main -h` para obtener una pantalla de ayuda con los parámetros soportados por el ejecutable.
   ## Ejemplo de uso

- En caso de necesitar distribuir un secreto `secret.bmp` en un directorio `shares` con un `k` de 5, se podría correr:
  ```
  ./bin/main -d -i ./secret.bmp -o ./shares -k 5
  ```
- En caso de necesitar recuperar un secreto a partir de un set de sombras almacenadas en el directorio `shares` y obtener el secreto en un archivo `secret.bmp` (nuevamente `k=5`), se podría correr:

  ```
  ./bin/main -r -i ./shares -o ./result.bmp -k 5
  ```

# Testing

Para correr los tests, es suficiente con correr la regla `make test`. Esto compila los fuentes, la librería de testeo CuTest y corre todos los tests según lo establecido en `test/test.c`.
