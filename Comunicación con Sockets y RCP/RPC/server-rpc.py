# servidor_rpc.py
from xmlrpc.server import SimpleXMLRPCServer

# Función que se expondrá remotamente
def cuadrado(numero):
    return numero * numero

# Crear el servidor en localhost y puerto 8000
servidor = SimpleXMLRPCServer(("localhost", 8000))
print("Servidor RPC listo en puerto 8000...")

# Registrar la función para acceso remoto
servidor.register_function(cuadrado, "cuadrado")

# Mantener el servidor en espera
servidor.serve_forever()
