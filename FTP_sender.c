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
    int socket_fd;
    struct sockaddr_in server_addr;

    printf("Enter the name of the file to send: ");
    scanf("%s", filename);

    // Open the file for reading
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    // Create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        printf("Error creating socket.\n");
        return 1;
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Change to receiver IP address
    server_addr.sin_port = htons(PORT);

    // Connect to the receiver
    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error connecting to the receiver.\n");
        return 1;
    }

    // Read file contents and send to the receiver
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        // Send the buffer to the receiver
        if (send(socket_fd, buffer, strlen(buffer), 0) < 0) {
            printf("Error sending data to the receiver.\n");
            return 1;
        }
    }

    // Close the file
    fclose(file);

    // Close the socket
    close(socket_fd);

    printf("File sent successfully.\n");

    return 0;
}
