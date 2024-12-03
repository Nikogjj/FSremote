#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include "global.h"
#define TEST 10


int main(){


    struct sockaddr_in server_addr={
        .sin_addr.s_addr=INADDR_ANY,
        .sin_family=AF_INET,
        .sin_port=htons(PORT_SERVER)
    };


    struct sockaddr_in client_addr;

    socklen_t len;

    int server_fd = socket(AF_INET,SOCK_STREAM,0);perror("socket");
    if (server_fd==-1)return EXIT_FAILURE;

    int check_error = bind(server_fd,(struct sockaddr*)&server_addr,sizeof server_addr);perror("bind");
    if(check_error==-1)return EXIT_FAILURE;

    check_error = listen(server_fd,1000);perror("listen");
    if(check_error==-1)return EXIT_FAILURE;

    int client_fd = accept(server_fd,(struct sockaddr*)&client_addr,&len);perror("accept");
    if(client_fd==-1)return EXIT_FAILURE;

    // pthread_t thread;
    // pthread_create(&thread,NULL,recv_server,NULL);
    char reponse[255];
    check_error=recv(client_fd,reponse,sizeof reponse,0);perror("fist recv");
    if(client_fd==-1)return EXIT_FAILURE;
    int file_size=atoi(reponse);
    printf("taille du prmeier envoi : %d\n",file_size);

    char final_file[file_size];
    int taille_second_recv=recv(client_fd,final_file,4745,0);perror("second recv");
    printf("%d\n",taille_second_recv);
    if(client_fd==-1)return EXIT_FAILURE;

    FILE * fd=fopen("image.jpeg","wb+");
    fwrite(final_file,1,4745,fd);
    fclose(fd);
}