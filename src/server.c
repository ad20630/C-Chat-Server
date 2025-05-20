#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 80
#define SERVER_PORT 8080
#define SA struct sockaddr

// function to handle client-server communication
void handle_client_server_communication(int client_connection_fd)
{
    char message_buffer[MAX_BUFFER_SIZE];
    int buffer_index;

    for (;;) {
        // clear buffer and read client message
        bzero(message_buffer, MAX_BUFFER_SIZE);
        read(client_connection_fd, message_buffer, sizeof(message_buffer));
        printf("From client: %s\t To client : ", message_buffer);

        // clear buffer and get server response
        bzero(message_buffer, MAX_BUFFER_SIZE);
        buffer_index = 0;
        while ((message_buffer[buffer_index++] = getchar()) != '\n')
            ;

        // send response to client
        write(client_connection_fd, message_buffer, sizeof(message_buffer));

        // check for exit command
        if (strncmp("exit", message_buffer, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

int main()
{
    int server_socket, client_connection_fd, client_addr_len;
    struct sockaddr_in server_addr, client_addr;

    // create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else {
        printf("Socket successfully created..\n");
    }

    // initialize server address structure
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    // bind socket to address
    if ((bind(server_socket, (SA*)&server_addr, sizeof(server_addr))) != 0) {
        printf("Socket bind failed...\n");
        exit(0);
    }
    else {
        printf("Socket successfully binded..\n");
    }

    // listen for connections
    if ((listen(server_socket, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else {
        printf("Server listening..\n");
    }

    // accept client connection
    client_addr_len = sizeof(client_addr);
    client_connection_fd = accept(server_socket, (SA*)&client_addr, &client_addr_len);
    if (client_connection_fd < 0) {
        printf("Server accept failed...\n");
        exit(0);
    }
    else {
        printf("Server accept the client...\n");
    }

    // start client-server communication
    handle_client_server_communication(client_connection_fd);

    // close socket
    close(server_socket);
    return 0;
}

