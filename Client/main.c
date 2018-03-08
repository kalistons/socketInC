#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <unistd.h>
#include <stdlib.h>

#define dominio  AF_INET
#define tipoSocket  SOCK_STREAM
#define todasPortas INADDR_ANY
#define localHost inet_addr("127.0.0.1")
int posicaoInicialPalavra = 0;
int totalDeTentativas = 0;
char txt_tentativas[200] = "\nNUMERO DE TENTATIVAS: 10\n";
char dica[2000] = "";
char palavra[2000] = "";
char resposta[2000] = "";

void preencherDica(char resposta[]){
    for(int it = 0;resposta[it] != '\0'; it++) {
        if((resposta[it]) == ':') {
            posicaoInicialPalavra = it + 1;
            break;
        }
        dica[it] = resposta[it];
    }

}
void preencherPalavra(char resp[]){
    int i = 0;
    for(int it = posicaoInicialPalavra;resp[it] != '\0'; it++) {
        palavra[it] = resp[it];
        resposta[i++] = ' ';
        resposta[i++] = '_';
    }
}
void getTentativas() {

}
//void tratarPalvra(char msn[1000]) {
//    int acertou = 0;
//    totalDeTentativas++;
//    for(int it = 0, pos = 0; palavra[it] != '\0' && msn[0] != '\0'; it++) {
//        if(palavra[it] != msn[it]) {
//            resposta[pos++] = ' ';
//            resposta[pos++] = '_';
//
//        } else {
//            acertou = 1;
//            resposta[pos++] = ' ';
//            resposta[pos++] = msn[it];
//        }
//    }
//    txt_tentativas
//}
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] = "1" , server_reply[2000] = "";
    int initGame = 0;

    //Criando o socket
    sock = socket(dominio , tipoSocket , 0);
    if (sock == -1)
    {
        printf("Erro ao criar o socket. ");
    }
    puts("Socket criado.");

    server.sin_addr.s_addr = localHost;
    server.sin_family = dominio;
    server.sin_port = htons( 3000 );

    //Conect com o servidor
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("Erro de conexao..");
        return 1;
    }

    puts("Connected\n");

    printf("Digite 1 para iniciar o jogo : ");
    scanf("%d",&initGame);
    //Comunicacao com o servidor
    while( initGame == 1)
    {
        puts("buscando dados no servidor......");

        //Enviar msn
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Erro ao enviar enviar. ");
            return 1;
        }

        //Resposta do servidor
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }

        puts("Server responde \n\n");
        preencherDica(server_reply);
        puts("\t **** DICA ****\n");
        puts(dica);
        puts(txt_tentativas);
        preencherPalavra(server_reply);
        puts(resposta);

        while (1 && message[0] != '*') {
            puts("\nPARA CANCELAR O JOGO: * ");
            printf("DIGITE UMA LETRA: ");
            scanf("%s",message);
//            tratarPalvra(message);
        }
        message[0] = '1';

    }

    close(sock);
    return 0;
}
