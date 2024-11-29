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

int main(int argc,char** argv){
    int port_client=0;
    if (argc>1)
    {
        port_client=atoi(argv[1]);
    }
    


    struct sockaddr_in client_addr={
        .sin_addr.s_addr=INADDR_ANY,
        .sin_family=AF_INET,
        .sin_port=htons(port_client)
    };

    struct sockaddr_in server_addr={
        .sin_addr.s_addr=INADDR_ANY,
        .sin_family=AF_INET,
        .sin_port=htons(PORT_SERVER)
    };

    socklen_t len;

    int client_fd = socket(AF_INET,SOCK_STREAM,0);perror("socket");
    if (client_fd==-1)return EXIT_FAILURE;

    int check_error = bind(client_fd,(struct sockaddr*)&client_addr,sizeof client_addr);perror("bind");
    if(check_error==-1)return EXIT_FAILURE;

    check_error = connect(client_fd,(struct sockaddr*)&server_addr,sizeof server_addr);perror("connect");
    FILE * file_fd=fopen("massi.jpeg","rb");
    fseek(file_fd,0,SEEK_END);
    int size_fd = ftell(file_fd);
    fseek(file_fd,0,SEEK_SET);
    char image[size_fd];
    fread(image,size_fd,1,file_fd);
    char size_fd_char[BUFSIZ];memset(size_fd_char,0,BUFSIZ);
    sprintf(size_fd_char,"%d",size_fd);

    send(client_fd,size_fd_char,strlen(size_fd_char),0);
    
    send(client_fd,image,size_fd,0);

    close(client_fd);

}