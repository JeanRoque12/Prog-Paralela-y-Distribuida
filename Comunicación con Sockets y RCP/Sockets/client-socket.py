# cliente.py
import socket

# Crear socket TCP
cliente = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Conectar con el servidor en localhost y puerto 5000
cliente.connect(('localhost', 5000))

# Enviar mensaje al servidor
mensaje = "Hola, servidor. Soy el cliente."
cliente.send(mensaje.encode())

# Esperar respuesta del servidor
respuesta = cliente.recv(1024).decode()
print("Respuesta del servidor:", respuesta)

# Cerrar conexión
cliente.close()
