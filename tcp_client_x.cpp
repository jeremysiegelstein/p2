#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>

#define PORT 12345
//CLIENTS X AND Y HAVE THE SAME CODE
void foo(int sockfd){
    char buff[1024];
    int n;
    bzero(buff, sizeof(buff));
    std::cout << "Client X: ";
    n = 0;
    //Populate buffer with your message
    while ((buff[n++] = getchar()) != '\n'){}
    //Get rid of the newline character
	buff[n-1] = '\0';
    //Write to the server
    write(sockfd, buff, sizeof(buff));
    //Clear the buffer
    bzero(buff, sizeof(buff));
    //Read from the server
    read(sockfd, buff, sizeof(buff));
    //Print from the server
    std::cout << buff;
}

int main(){
    int sockfd;
    struct sockaddr_in servaddr;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //Check that the socket is successfully created
    //If not, error
    if (sockfd == -1) {
        std::cout << "Socket creation failed...\n" << std::endl;;
        exit(0);
    }

    //If the socket is created, print. Used for debugging
    /*else
        std::cout << "Socket successfully created..\n");*/

    //Zero out the server address
    bzero(&servaddr, sizeof(servaddr));

    // assign IP and PORT information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    //Check that the connection is completed successfully
    if (connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        std::cout << "Connection with the server failed..."  << std::endl;
        exit(0);
    }
    //If the connection is successful, print
    else
        std::cout << "Connected to the server" << std::endl;

    //function to allow writing to server
    foo(sockfd);

    // close the socket
    close(sockfd);
}
