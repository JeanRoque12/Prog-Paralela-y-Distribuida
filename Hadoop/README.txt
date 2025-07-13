Documentación del código WordCount en Hadoop MapReduce

Descripción general

Este programa implementa un clásico ejemplo de MapReduce: el conteo de palabras (Word Count). Su propósito es contar cuántas veces aparece cada palabra en un conjunto de archivos de texto distribuidos.

El código está basado en el tutorial oficial de Apache Hadoop, disponible en:
https://hadoop.apache.org/docs/current/hadoop-mapreduce-client/hadoop-mapreduce-client-core/MapReduceTutorial.html?utm_source=chatgpt.com

Componentes principales

1. Clase TokenizerMapper

- Extiende: Mapper<Object, Text, Text, IntWritable>
- Función: Procesa cada línea de entrada y emite pares (palabra, 1) para cada palabra encontrada.
- Método clave:
  - map(Object key, Text value, Context context): Tokeniza la línea de texto y escribe cada palabra con valor 1 en el contexto.

2. Clase IntSumReducer

- Extiende: Reducer<Text, IntWritable, Text, IntWritable>
- Función: Recibe todas las ocurrencias de una palabra (como valores 1) y las suma para obtener el total.
- Método clave:
  - reduce(Text key, Iterable<IntWritable> values, Context context): Suma los valores para la clave dada y escribe el resultado final.

3. Método main

- Configura y ejecuta el job MapReduce.
- Pasos clave:
  - Crear configuración y job.
  - Definir clases de Mapper, Reducer y Combiner.
  - Establecer tipos de salida (clave y valor).
  - Definir rutas de entrada y salida según argumentos pasados.
  - Ejecutar el job y finalizar con código de salida.

Flujo de ejecución

1. El framework Hadoop divide el archivo de entrada en fragmentos.
2. Cada fragmento es procesado por un Mapper que emite (palabra, 1) por cada palabra.
3. Las salidas del mapper son agrupadas por palabra (clave).
4. Los valores asociados a cada palabra son procesados por el Reducer para sumar las ocurrencias.
5. Los resultados se almacenan en la ruta de salida especificada.

Notas importantes

- El programa utiliza un combiner (la misma clase que el reducer) para optimizar el proceso reduciendo la cantidad de datos transferidos entre mappers y reducers.
- Se espera que los argumentos de línea de comando indiquen la ruta de entrada y la ruta de salida en el sistema de archivos Hadoop (HDFS).

Referencia

Código original tomado de: https://hadoop.apache.org/docs/current/hadoop-mapreduce-client/hadoop-mapreduce-client-core/MapReduceTutorial.html?utm_source=chatgpt.com
