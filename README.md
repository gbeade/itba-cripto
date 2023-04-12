# Programa de Imagen Secreta Compartida en C

Este es un programa en lenguaje C que implementa el algoritmo de Imagen Secreta Compartida descrito en el documento *(k,n) secret image sharing scheme capable of cheating detection* de Yan-Xiao Liu, Quin-Dong Sun y Ching-Nung Yang de la Universidad de Tecnología de Xi'an (China).

## Descripción del algoritmo

El algoritmo de Imagen Secreta Compartida es un método de criptografía que permite dividir una imagen BMP en múltiples fragmentos, conocidos como sombras, y distribuirlas entre un grupo de personas. Para recuperar la imagen original, es necesario reunir una cantidad mínima de sombras, establecida previamente, y combinarlas. En esta implementación, las sombras están ocultas estenográficamente en otras imágenes.   

Este programa implementa el algoritmo descrito en el documento mencionado anteriormente, que utiliza un esquema (k,n) capaz de detectar intentos de fraude o manipulación. El programa permite generar sombras a partir de una imagen de entrada y recuperar la imagen original a partir de un conjunto de sombras proporcionado por los usuarios.

# Instrucciones de uso
Para compilar y ejecutar el programa:

1. Clonar el repositorio en local.
2. Crear un directorio bin/ para almacenar los archivos compilados: `mkdir bin`
3. Compilar el proyecto: `make all`
4. Ejecutar el programa: ./bin/main

