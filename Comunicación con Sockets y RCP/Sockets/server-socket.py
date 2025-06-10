# servidor.py
import socket

# Crear socket TCP (AF_INET = IPv4, SOCK_STREAM = TCP)
servidor = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Asociar a IP y puerto (localhost, puerto 5000)
servidor.bind(('localhost', 5000))
servidor.listen(1)  # Máximo 1 conexión pendiente

print("Servidor en espera de conexiones en puerto 5000...")

# Aceptar conexión
conexion, direccion = servidor.accept()
print(f"Conexión recibida de: {direccion}")

# Recibir mensaje del cliente
mensaje = conexion.recv(1024).decode()
print("Mensaje recibido del cliente:", mensaje)

# Responder con mensaje de confirmación
respuesta = "Mensaje recibido correctamente"
conexion.send(respuesta.encode())

# Cerrar conexión
conexion.close()
servidor.close()
