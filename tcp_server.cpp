#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#define TRUE   1
#define PORT 12345
#define MAX_CLIENTS 2
#define MAX 1024

int main(int argc, char *argv[]){
    int opt = TRUE;
    int sockfd, new_socket, client_socket[MAX_CLIENTS], i, sd;
    int max_sd;
    struct sockaddr_in address;

    char buffer[MAX];  //data buffer of 1K

    //set of socket descriptors
    fd_set readfds;

    //initialize all client_socket[] to 0
    for (i = 0; i < MAX_CLIENTS; i++){
        client_socket[i] = 0;
    }

    //create a master socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0));

    //set master socket to allow multiple connections,
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

    //Assign IP and Port information
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    //bind the socket to localhost port 12345
    if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0){
        std::cerr << "bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    //the server socket can only have two pending connections; one for each server
    listen(sockfd, MAX_CLIENTS);

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    //Counter will be used to make sure that two messages have been received, on from each client
	int counter = 0;
    //Flag will indicate whether or not we are reading the first message
    //If we are reading the first message, we copy it. If not, we concatenate it.
	int flag = 0;
    //Temp used to create message to send to clients
	char temp[MAX] = {'\0'};

    while(counter < MAX_CLIENTS)
    {
        //Select setup from https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/
        //clear the socket set
        FD_ZERO(&readfds);

        //add server socket to set
        FD_SET(sockfd, &readfds);
        max_sd = sockfd;

        //add child sockets to set
        for ( i = 0 ; i < MAX_CLIENTS ; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd, &readfds);

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets
        //Set the timeout to NULL so that we wait indefinitely for activity
        activity = select( max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)){
            std::cerr << "select error" << std::endl;
        }

        //Something happening on the socket server indicates incoming connection
        if (FD_ISSET(sockfd, &readfds)){
            //Check that the incoming connection is accepted
            //If not exit the program
            if ((new_socket = accept(sockfd, (struct sockaddr *)&address, (socklen_t*)&sizeof(address))) < 0){
                std::cerr << "accept" << std::endl;
                exit(EXIT_FAILURE);
            }

            //add new socket to array of sockets
            for (i = 0; i < MAX_CLIENTS; i++){
                //Find empty index in array of sockets and add the new one
                if( client_socket[i] == 0 ){
                    client_socket[i] = new_socket;
                    break;
                }
            }
        }

        //else its some IO operation on some other socket
        for (i = 0; i < MAX_CLIENTS; i++){
            sd = client_socket[i];

            if (FD_ISSET( sd, &readfds)){
                //Check if it was for closing, and also read the
                //incoming message
                if ((read( sd, buffer, MAX)) == 0){
                    //Somebody disconnected, get his details and print
                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&sizeof(address));

                    //Close the socket and mark as 0 in list for reuse
                    close(sd);
                    client_socket[i] = 0;
                }

                else{
                    //Reaching this loop implies a message was received
                    //Counter indicates number of messages received
                    //When counter reaches 2, the while loop will terminate
					counter++;
                    //Flag indicates first message. Used if we wanted to abstract this idea
                    //Could have used the counter here and incremented after
                    //First message is copied from buffer to temp
					if(flag == 0) strcpy(temp, buffer);
                    else{
                        //If we enter this loop we've
                        //So temp is currently the first message received
                        //Concatenate " received before " as requested
						strcat(temp, " received before ");
                        //The buffer now contains the second message
                        //Concatenate the second message
						strcat(temp, buffer);
						strcat(temp, "\n");
					}
                    //Flag indicates whether or not a message received is the first
					flag = 1;
                }
            }
        }
		if(counter == MAX_CLIENTS){
            //Write to the first client that joined the server
			write(client_socket[0], temp, sizeof(temp));
            //Write to the second client that joined the server
			write(client_socket[1], temp, sizeof(temp));
            //Indicate that both acknowledgement messages have been sent
            std::cout << "Sent acknowledgement to both X and Y" << std::endl;
		}
    }

    //Close socket
    close(sockfd);

    return 0;
}
