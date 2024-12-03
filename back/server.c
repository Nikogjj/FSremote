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


int main(){

    /**
     * Définition des infos du serveur
     * port et ip
     */
    struct sockaddr_in server_addr={
        .sin_addr.s_addr=INADDR_ANY,
        .sin_family=AF_INET,
        .sin_port=htons(PORT_SERVER)
    };


    int server_fd = socket(AF_INET,SOCK_STREAM,0);perror("socket");
    if (server_fd==-1)return EXIT_FAILURE;

    int check_error = bind(server_fd,(struct sockaddr*)&server_addr,sizeof server_addr);perror("bind");
    if(check_error==-1)return EXIT_FAILURE;

    check_error = listen(server_fd,BUFSIZ);perror("listen");
    if(check_error==-1)return EXIT_FAILURE;
   
   
    struct sockaddr_in client_addr;
    socklen_t len;

    int client_fd = accept(server_fd,(struct sockaddr*)&client_addr,&len);perror("accept");
    if(client_fd==-1)return EXIT_FAILURE;

    // pthread_t thread;
    // pthread_create(&thread,NULL,recv_server,NULL);


    /**
     * Receive the size of the file to be uploaded
     * The client is supposed to use send to send to length of the file in bytes.
     */
    char reponse[255];
    check_error=recv(client_fd,reponse,sizeof reponse,0);perror("recv() - recevie file length");
    if(check_error==-1 || check_error == 0)
    {
        return EXIT_FAILURE;
    }

    int file_size=atoi(reponse);
    if(file_size == 0){
        printf("atoi(reponse) failed : reponse is not a valid integer");
        return EXIT_FAILURE;
    }
    printf("taille du fichier réçu par le premier recv() : %d\n",file_size);


    /**
     * Reception du fichier
     */
    char* final_file = malloc(file_size);
    int taille_second_recv=recv(client_fd,final_file,file_size,0);perror("recv() - recpetion du fichier à uploader");
    printf("taile 1st recv : %d\n",taille_second_recv);

    taille_second_recv=recv(client_fd,final_file,file_size,0);perror("recv() - recpetion du fichier à uploader");
    printf("taile 2nd recv : %d\n",taille_second_recv);
    return 0;

    printf("taille second recv() %d\n",taille_second_recv);
    if(taille_second_recv==-1 || taille_second_recv == 0)return EXIT_FAILURE;

    FILE * uploaded_file_fd=fopen("image.jpg","wb+");perror("fopen()");
    fwrite(final_file,1,file_size,uploaded_file_fd);perror("fwrite()");
    
    fclose(uploaded_file_fd);perror("fclose()");
    free(final_file);

}