#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 8080
#define B_SIZE 128
#define SA struct sockaddr

// client interaction function
int read_from_client(int connfd, FILE *f) {
    char buff[B_SIZE];

    while(1) {
        read(connfd, buff, B_SIZE);
        if(strncmp(buff, "HALT\n", B_SIZE) == 0) return(1);
        if(strncmp(buff, "EXIT\n", B_SIZE) == 0) return(0);
        printf("%s", buff);
        fprintf(f, "%s", buff);
    }
}

// server handler function
int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli; 
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed\n");
        close(sockfd);
        return(1);
    }

    // assign IP address and port number
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    servaddr.sin_port = htons(PORT);

    // bind socket to IP
    if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Socket bind failed, check network processes\n");
        close(sockfd);
        return(1);
    }

    // begin listening for client connections, only allow 1 connection at a time
    if (listen(sockfd, 1) != 0) {
        printf("Failed to listen\n");
        close(sockfd);
        return(1);
    }
    else {
        printf("Server running...\n");
    }

    FILE *f = fopen("serv_log.txt", "w");

    while(1) {
        // accept client data
        unsigned int len = sizeof(cli);
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0) {
            printf("Failed to accept client\n");
            close(sockfd);
            return(1);
        }

        // log client data

        // loop client message functionality, end main() loop if client sent shutdown
        if (read_from_client(connfd, f) != 0) {
            close(sockfd);
            return(0);
        }
    }

    // close log file, the socket, and end the program
    fclose(f);
    close(sockfd);
    return(0);
}