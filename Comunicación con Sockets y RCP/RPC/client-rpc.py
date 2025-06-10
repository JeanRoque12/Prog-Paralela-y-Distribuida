# cliente_rpc.py
import xmlrpc.client

# Conectar con el servidor
proxy = xmlrpc.client.ServerProxy("http://localhost:8000/")

# Pedir número al usuario
numero = int(input("Ingrese un número entero: "))

# Llamar a la función remota
resultado = proxy.cuadrado(numero)

# Mostrar el resultado
print(f"El cuadrado de {numero} es: {resultado}")
