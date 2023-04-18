#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SRV_ADDR "127.0.0.1"
#define PORT 9999
#define MAX_SIZE 256

int main() 
{
    // Initializing socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_fd == -1) {
        perror("[**ERROR] - socket()");
        exit(1);
    }

    // Updating address info
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // inet_addr(SRV_ADDR); 
    addr.sin_port = htons(PORT);

    // Binding socket
    if (bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("[**ERROR] - bind()");
        exit(1);
    }

    // Listening for connection
    if (listen(sock_fd, 5)) {
        perror("[**ERROR] - listen()");
        exit(1);
    }
    printf("Listening on port %d...\n", PORT);

    // Accepting connection
    // Without Client IP
    int acpt_client = accept(sock_fd, NULL, NULL);
    if (acpt_client == -1) {
        perror("[**ERROR] - accept()");
        exit(1);
    }
    
    puts("New Client Connected.\n");

    // Receiving from Client
    char buff[MAX_SIZE];
    puts("Received from Client:");
    puts("----------------------");

    while(1) {
        char sub_buff[20];
        memset(sub_buff, 0, sizeof(sub_buff));      // Clear sub_buff

        int rcv_byte = recv(acpt_client, buff, MAX_SIZE, 0);
        if (rcv_byte <= 0) {
            printf("Connection closed.\n");
            break;
        }
        if (rcv_byte < MAX_SIZE) 
            buff[rcv_byte] = 0;

        char * tmp_string  = strtok(buff, " ");
        printf("Device Name: %s\n", tmp_string);

        tmp_string = strtok(NULL, " ");
        if (atoi(tmp_string) == 1) {
            printf("Client has %s disk.\n", tmp_string);
        } else {
            printf("Client has %s disks.\n", tmp_string);
        }

        while ((tmp_string = strtok(NULL, " ")) != NULL) {
            memset(sub_buff, 0, sizeof(sub_buff));
            strncpy(sub_buff, tmp_string, strlen(tmp_string));
            printf("\t+) %c - %s\n", toupper(sub_buff[0]), sub_buff + 1);
        }
    }

    // Closing connection
    close(acpt_client);
    close(sock_fd);
}