#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

int main(void){
    struct sockaddr_in direccionServidor;
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
    direccionServidor.sin_port = htons(8080); //Notar que es correlativo al puerto marcado en el servidor!!

    int servidor = socket(AF_INET, SOCK_STREAM, 0);
    if(connect(servidor, (void*) &direccionServidor, sizeof(direccionServidor)) != 0){
        perror("No se pudo conectar con el servidor :/");
        return 1;
    }


    while(1){
        char mensaje [1000];
        scanf("%s", mensaje);

        send(servidor, mensaje, strlen(mensaje), 0);
    }

    return 0;
}