#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void verifyServer(const int func, const char *name) {
    if (func < 0) {
        perror(name);
        exit(EXIT_FAILURE);
    }
}

int main(void) {

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    verifyServer(server_fd, "socket");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    int server_bind = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    verifyServer(server_bind, "bind");

    int listen_fd = listen(server_fd, 3);
    verifyServer(listen_fd, "listen");

    while (1) {
        int new_socket = accept(server_fd, NULL, NULL);
        verifyServer(new_socket, "accept_connection");

        printf("New connection received.\n");

        char buffer[1024];
        long read_bytes = read(new_socket, buffer, 1024);

        if (read_bytes > 0) {
            buffer[read_bytes] = '\0';
            printf("Browser msg:\n%s\n", buffer);
        }

        char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello World";
        long write_bytes = write(new_socket, hello, strlen(hello));

        close(new_socket);
    }
}