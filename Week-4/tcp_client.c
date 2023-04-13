#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_SIZE 2048

int main(int argc, char * argv[])
{
    if (argc != 3) {
        printf("[ERROR]: Incorrect command\n");
        printf("Usage: ./tcp_client serverIP port");
        exit(1);
    }

    int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    int port = atoi(argv[2]);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(port);

    int c_state = connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (c_state == -1) {
        perror("[ERROR] - connect()");
        exit(1);
    }

    char buff[MAX_SIZE];
    int rcv_byte = recv(sock_fd, buff, MAX_SIZE, 0);
    if (rcv_byte <= 0) {
        printf("Connection closed\n");
        exit(1);
    }

    if (rcv_byte < MAX_SIZE)
        buff[rcv_byte] = 0;

    printf("%s\n", buff);

    while (1)
    {
        printf("[MESSAGE]: ");
        fgets(buff, MAX_SIZE, stdin);               // Get input from terminal

        send(sock_fd, buff, strlen(buff), 0);

        if (strncmp(buff, "exit", 4) == 0) {         // Stop if input == "exit"
            printf("Disconnecting....\n");
            break;
        }      
    }

    // Closing connection
    close(sock_fd);
}