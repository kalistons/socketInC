#include<pthread.h>
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <stdbool.h>

#define dominio  AF_INET
#define tipoSocket  SOCK_STREAM
#define todasPortas INADDR_ANY
#define localHost inet_addr("127.0.0.1")
#define backlog  10

char client_message[2000] = "";

char* buscaPalvra() {
    return "nome:sofa,dica:lugar de sentar.";
}

void strToChar(char* msn) {
    for(int it = 0; it < sizeof(*msn); it++) {
        client_message[it] = &msn[it];
    }
}

bool isVazio(char msn[]) {
    int cont = 0;
    for(int it = 0; msn[it] != '\0'; it++) {
        cont++;
    }
    return cont > 0;

}

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;



    socket_desc = socket(dominio , tipoSocket , 0);
    if (socket_desc == -1)
    {
        printf("Erro ao criar o socket.");
    }
    puts("Socket criado com sucesso.");

    //Prepare the sockaddr_in structure
    server.sin_family = dominio;
    //inet_addr("127.0.0.1")
    server.sin_addr.s_addr = todasPortas;
    server.sin_port = htons( 3000 );

    //Bind do servidor
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("Erro ao realizar bind....");
        return 1;
    }
    puts("bind concluido..");

    //Listen
    if (listen(socket_desc , backlog) < 0) {
        perror("Erro no modulo de Listen...");
    }


    puts("Esperando conexao...");
    c = sizeof(struct sockaddr_in);

    //aceita uma conexao com cliente
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("Conexao com cliente nao aceita");
        return 1;
    }
    puts("Conexao aceita");

    //Msn do cliente
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
        if(isVazio(client_message)) {
            strcpy(client_message, "lugar de sentar:sofa");

        }
        //Envia msn de volta para o cliente
        write(client_sock , client_message , strlen(client_message));
    }

    if(read_size == 0)
    {
        puts("Cliente desconectado");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}
