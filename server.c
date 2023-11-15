/* Filename:
 *      server.c
 *
 * Descrition:
 *      This program is used in tandem with client.c
 *      This demonstrates the use of sockets to create a TCP server.
 *
 * Compile Instructions:
 *      `gcc -o server server.c' or use the accompanying Makefile 
 * 
 * Author:
 *      Glenn G. Fabia
 *      glennfabia@adnu.edu.ph
 *      Justin Ira A. Natividad
 *      junatividad@gbox.adnu.edu.ph
 *      Ateneo de Naga University
 *
 * Notes:
 *      Made for academic purposes, edited from original code by Sir Glenn
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void die_with_error(char *error_msg){
    printf("%s", error_msg);
    exit(-1);
}

int main(int argc, char *argv[]){
    int server_sock, client_sock, port_no, client_size, n;
    char buffer[256];
    char buffer2[256];
    struct sockaddr_in server_addr, client_addr;
    if (argc < 2) {
        printf("Usage: %s port_no", argv[0]);
        exit(1);
    }

    printf("Server starting ...\n");
    // Create a socket for incoming connections
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) 
       die_with_error("Error: socket() Failed.");
       
    // Bind socket to a port
    bzero((char *) &server_addr, sizeof(server_addr));
    port_no = atoi(argv[1]);
    server_addr.sin_family = AF_INET; // Internet address 
    server_addr.sin_addr.s_addr = INADDR_ANY; // Any incoming interface
    server_addr.sin_port = htons(port_no); // Local port
    
    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
       die_with_error("Error: bind() Failed.");
       
    // Mark the socket so it will listen for incoming connections
    listen(server_sock, 5);
    printf("Server listening to port %d ...\n", port_no);
    
    printf("Waiting for connection(s) ...\n");

    // Accept new connection
    client_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *) &client_addr, (socklen_t *) &client_size);
    if (client_sock < 0) 
        die_with_error("Error: accept() Failed.");

    printf("Client succesfully connected ...\n");    

    // Communicate    
    int pid = fork();
    while(pid == 0){
        bzero(buffer, 256);
        n = recv(client_sock, buffer, 255, 0);
        if (n <= 0) die_with_error("Error: recv() Failed.");
        printf("[client] > %s", buffer);
    }
    while(pid > 0){
        bzero(buffer2, 256);
        fgets(buffer2, 255, stdin);
        n = send(client_sock, buffer2, strlen(buffer2), 0);
        if (n <= 0) die_with_error("Error: send() Failed.");
    }

    printf("Closing connection ...\n");
    close(client_sock);
    
    close(server_sock);
    
    return 0; 
}
