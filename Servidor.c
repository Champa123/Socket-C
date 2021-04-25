#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

//Devuelve un socket servidor
int crearSocketServidor(int puerto){
	struct sockaddr_in direccion;
	direccion.sin_family = AF_INET;
	direccion.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccion.sin_port = htons(puerto);

	//Tipo Socket: tipo stream o de tipo datagram. En nuestro caso debemos usar la constante
	//SOCK_STREAM para utilizar un socket de tipo stream (para el otro caso, se utiliza la constante SOCK_DGRAM).
	//Stream conexion segura pero manejarlo Bueno para no perder paquetes datagram envio async envia en algun momento llega (Espera ACK sino retry)

	int servidor = socket(AF_INET, SOCK_STREAM, 0);
	if(bind(servidor, (void*) &direccion, sizeof(direccion)) != 0){
		perror("Fallo el bind");
		return -1;
	}
	return servidor;
}

int acceptConnection(int servidor){
	struct sockaddr_in direccionCliente;//Esto tengo la sospecha que se usa para configurar al cliente Direcc ip y otra config. 
	direccionCliente.sin_family = AF_INET;
	direccionCliente.sin_addr.s_addr = inet_addr("127.0.0.1");
	unsigned int tamanioDireccionCliente = 20;
	int cliente = accept(servidor, (void*) &direccionCliente, &tamanioDireccionCliente );//(void*)&direccionCliente, &tamanoDireccionCliente);
	if(cliente == -1){
		printf("Hubo un error con la creacion del cliente\n");
		return -1;
	}
	printf("Recibi una conexion en %d!!\n", cliente);
	return cliente;
}

void setReuseAddress(int servidor){ //Para reiniciar la consola y que no falle el bind.
	int activado = 1;
	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));
}

void sendMessage(int cliente, char* mensaje){
	send(cliente, mensaje, strlen(mensaje) + 1, 0);  // Tercer argumento la long del mensaje 
									//El ultimo argumento de la funcion son flags (Asumo que para algo de configuracion de send)
}

void receiveData(int cliente){ // la parte interesante de esto es usar el Header del mensaje enviado para saber la cant memoria para el malloc
	//uint32_t tamañoPaquete;
	//recv(cliente, &tamañoPaquete, size(uint32_t), 0) //size(uint32_t) == 4bytes. El 0 final es para "comportamiento por defecto"
	//char* buffer = malloc(tamañoPaquete);
	//recv(cliente, buffer, tamañoPaquete, 0)
	char* buffer = malloc(1000); // almacenar msje
	int i = 0;
	while(i <3){

		int bytesRecibidos = recv(cliente, buffer, 1000, 0); //Hasta 4 bytes te leo, mas no. "\0" ocupa un lugar!
		if(bytesRecibidos <= 0){
			perror("El cliente se desconecto :( ! ");
			//return -1;
		}

		buffer[bytesRecibidos] = '\0';
		printf("Me llegaron %d bytes con %s\n", bytesRecibidos, buffer);
		i++;
	}

	free(buffer);

}	

int main(int argc, char **argv) {

	int servidor = crearSocketServidor(8080);
	setReuseAddress(servidor);
	printf("Socket escuchando el %d\n", 8080);
	listen(servidor, 100);
	int cliente = acceptConnection(servidor);
	sendMessage(cliente, "Hola MUNDO!!\n");
	sendMessage(cliente, ":) :P :D B) xD\n");
	receiveData(cliente);
	//sendMessage(cliente, "SI QUIERO MANDO UN MENSAJE MAS LARGO????");
	return 0;



}
