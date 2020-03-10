#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 8888
void func(int sockfd)
{
    char buff[MAX];
    int n;
        bzero(buff, sizeof(buff));
        printf("Client Y: ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
		buff[n-1] = '\0';
        write(sockfd, buff, sizeof(buff)-1);
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf(buff);
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    //function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
}
