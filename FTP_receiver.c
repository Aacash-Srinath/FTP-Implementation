#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define PORT 8888

int main() {
    FILE *file;
    char filename[100];
    char buffer[BUFFER_SIZE];
    int socket_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;

    printf("Enter the name of the file to receive: ");
    scanf("%s", filename);

    // Create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        printf("Error creating socket.\n");
        return 1;
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the server address
    if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error binding socket.\n");
        return 1;
    }

    // Listen for incoming connections
    if (listen(socket_fd, 1) < 0) {
        printf("Error listening for connections.\n");
        return 1;
    }

    // Accept a client connection
    client_addr_len = sizeof(client_addr);
    client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        printf("Error accepting connection.\n");
        return 1;
    }

    // Open the file for writing
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    // Receive file contents from the sender and write to the file
    while (1) {
        // Receive data from the sender
        ssize_t num_bytes = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (num_bytes < 0) {
            printf("Error receiving data from the sender.\n");
            return 1;
        } else if (num_bytes == 0) {
            break;  // Connection closed by the sender
        }

        // Write the received buffer to the file
        fwrite(buffer, sizeof(char), num_bytes, file);
    }

    // Close the file
    fclose(file);

    // Close the client socket
    close(client_fd);

    // Close the server socket
    close(socket_fd);

    printf("File received successfully.\n");

    return 0;
}
