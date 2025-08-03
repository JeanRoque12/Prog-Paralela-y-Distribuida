import time
from pyspark.sql import SparkSession
from pyspark.sql.functions import explode, split

# Crear SparkSession
spark = SparkSession.builder \
    .appName("WordCountRDDvsDataFrame") \
    .getOrCreate()

sc = spark.sparkContext

# Ruta del archivo de texto grande
file_path = "texto.txt"

# Word Count con RDD

start_rdd = time.time()

lines_rdd = sc.textFile(file_path)

word_counts_rdd = lines_rdd \
    .flatMap(lambda line: line.split()) \
    .map(lambda word: (word.lower(), 1)) \
    .reduceByKey(lambda a, b: a + b)

# Guardar resultados en archivo de texto
word_counts_rdd.saveAsTextFile("output/wordcount_rdd")

end_rdd = time.time()
rdd_time = end_rdd - start_rdd

# Word Count con DataFrame

start_df = time.time()

# Leer archivo como DataFrame de líneas
lines_df = spark.read.text(file_path)

# Contar palabras usando operaciones de DataFrame
words_df = lines_df.select(explode(split(lines_df.value, r"\s+")).alias("word"))
word_counts_df = words_df.groupBy("word").count().orderBy("count", ascending=False)

# Guardar resultados en CSV
word_counts_df.write.mode("overwrite").csv("output/wordcount_df")

end_df = time.time()
df_time = end_df - start_df


# Mostrar resultados de rendimiento

print(f"Tiempo usando RDD: {rdd_time:.2f} segundos")
print(f"Tiempo usando DataFrame: {df_time:.2f} segundos")

# Evitar división por cero
if df_time > 0:
    speedup = rdd_time / df_time
    print(f"Speedup (RDD / DataFrame): {speedup:.2f}x")
else:
    print("No se pudo calcular el speedup (tiempo del DataFrame = 0)")

# Finalizar Spark
spark.stop()

