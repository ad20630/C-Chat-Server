#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 80
#define SERVER_PORT 8080
#define SA struct sockaddr

// function to handle client-server communication
void handle_client_server_communication(int socket_fd)
{
    char message_buffer[MAX_BUFFER_SIZE];
    int buffer_index;

    for (;;) {
        bzero(message_buffer, sizeof(message_buffer));
        printf("Enter the string : ");
        buffer_index = 0;
        
        // read input until newline
        while ((message_buffer[buffer_index++] = getchar()) != '\n')
            ;
        
        // send message to server
        write(socket_fd, message_buffer, sizeof(message_buffer));
        
        // clear buffer and receive server response
        bzero(message_buffer, sizeof(message_buffer));
        read(socket_fd, message_buffer, sizeof(message_buffer));
        printf("From server : %s", message_buffer);

        // check for exit command
        if ((strncmp(message_buffer, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main()
{
    int client_socket, connection_status;
    struct sockaddr_in server_addr, client_addr;

    // create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else {
        printf("Socket successfully created..\n");
    }

    // initialize server address structure
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(SERVER_PORT);

    // connect to server
    connection_status = connect(client_socket, (SA*)&server_addr, sizeof(server_addr));
    if (connection_status != 0) {
        printf("Connection with the server failed...\n");
        exit(0);
    }
    else {
        printf("Connected to the server..\n");
    }

    // start client-server communication
    handle_client_server_communication(client_socket);

    //close socket
    close(client_socket);
    return 0;
}
