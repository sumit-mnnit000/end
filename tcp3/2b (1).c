#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_EXPR_LEN 100

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_hostname> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *serverHostname = argv[1];
    int port = atoi(argv[2]);

    int sockfd;
    struct sockaddr_in serverAddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints, *server;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(serverHostname, NULL, &hints, &server);
    if (status != 0) {
        fprintf(stderr, "Error: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    memcpy(&serverAddr, server->ai_addr, sizeof(serverAddr));
    serverAddr.sin_port = htons(port);

    freeaddrinfo(server);

    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("TCP client connected to %s on port %d\n", serverHostname, port);

    char inputExpr[MAX_EXPR_LEN];
    while (1) {
        printf("Enter an expression in the following format (operand1 operator operand2): ");
        fgets(inputExpr, sizeof(inputExpr), stdin);
        if (strcmp(inputExpr, "-1\n") == 0) {
            break;
        }

        ssize_t bytesSent = write(sockfd, inputExpr, strlen(inputExpr));
        if (bytesSent < 0) {
            perror("Write failed");
            exit(EXIT_FAILURE);
        }

        char buffer[MAX_EXPR_LEN];
        ssize_t bytesRead = read(sockfd, buffer, sizeof(buffer));
        if (bytesRead < 0) {
            perror("Read failed");
            exit(EXIT_FAILURE);
        }

        buffer[bytesRead] = '\0';
        printf("ANS: %s\n", buffer);
    }

    close(sockfd);

    return 0;
}
