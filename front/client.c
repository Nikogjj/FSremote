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
#include "../utils/global.h"

#define DEFAULT_CLIENT_PORT 5000
#define SERVER_IP "127.0.0.1"


int main(int argc,char** argv){
    int port_client=DEFAULT_CLIENT_PORT;
    if (argc>1) // Si l'utilisateur précise un port dans le terminal
    {
        port_client=atoi(argv[1]);
    }


    /**
     * Ask file name to user
     */
    printf("Quel document voulez vous transférer ?\n");
    char file_name[BUFSIZ];memset(file_name,0,BUFSIZ);
    fgets(file_name,BUFSIZ,stdin);
    file_name[strlen(file_name)-1]=0;


    /**
     * Initalisation du socket client
     */
    struct sockaddr_in client_addr={
        .sin_addr.s_addr=INADDR_ANY,
        .sin_family=AF_INET,
        .sin_port=htons(port_client)
    };

    struct sockaddr_in server_addr={
        .sin_addr.s_addr=inet_addr(SERVER_IP),
        .sin_family=AF_INET,
        .sin_port=htons(PORT_SERVER)
    };

    socklen_t len;

    int client_fd = socket(AF_INET,SOCK_STREAM,0);perror("socket");
    if (client_fd==-1)return EXIT_FAILURE;

    int check_error = bind(client_fd,(struct sockaddr*)&client_addr,sizeof client_addr);perror("bind");
    if(check_error==-1)return EXIT_FAILURE;


    /**
     * Connexion au serveur
     */
    check_error = connect(client_fd,(struct sockaddr*)&server_addr,sizeof server_addr);perror("connect");
    if(check_error == -1)return EXIT_FAILURE;
    

    /**
     * Chargement du fichier à envoyer
     */
    // Ouverture du fichier
    FILE * file_fd=fopen(file_name,"rb");

    // Get file length
    fseek(file_fd,0,SEEK_END);
    int size_fd = ftell(file_fd);
    char size_fd_buf[10];memset(size_fd_buf,0,10);
    sprintf(size_fd_buf,"%d",size_fd);

    // Replace cursor fd to the start of the file
    fseek(file_fd,0,SEEK_SET);

    // Read image to image array
    char image[size_fd];memset(image,0,size_fd);
    fread(image,size_fd,1,file_fd);
    fclose(file_fd);


    char size_fd_char[BUFSIZ];memset(size_fd_char,0,BUFSIZ);
    sprintf(size_fd_char,"%d",size_fd);
    printf("size_fd : %d, char image : %ld, size_fd_char %s\n",size_fd,sizeof image,size_fd_char);
    send(client_fd,size_fd_buf,strlen(size_fd_buf),0);
    // Send file size
    send(client_fd,size_fd_char,strlen(size_fd_char),0);

    // Send file
    send(client_fd,image,size_fd,0);

    close(client_fd);

}