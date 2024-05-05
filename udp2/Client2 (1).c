#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 8080
#define BUFFER_SIZE 1024
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <server_ip_address> <server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int sockfd;
    struct sockaddr_in servaddr;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }
    char buffer[BUFFER_SIZE];
    printf("Sending request to server for time of day information...\n");
    sendto(sockfd, "TimeRequest", strlen("TimeRequest"), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    int n;
    socklen_t len;
    n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    printf("Time from server: %s\n", buffer);
    close(sockfd);
    return 0;
}