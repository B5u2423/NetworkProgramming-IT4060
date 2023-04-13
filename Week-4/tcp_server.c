#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SRV_ADDR "127.0.0.100"
#define MAX_SIZE 2048

int main(int argc, char * argv[])
{
    FILE * f_handler;
    char buff[MAX_SIZE];

    // Verifying command format
    if (argc != 4) {
        printf("[ERROR]: Incorrect command.\n");
        printf("Usage: ./tcp_server port greetFile logFile\n");
        exit(1);
    }
    int port = atoi(argv[1]);
        
    f_handler = fopen(argv[2], "r");        // Check welcome file existence.
    if(f_handler == NULL) {
        printf("[ERROR]: Greeting file doesn't exist!\n");
        exit(1);
    }

    // Initialize socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_fd == -1) {
        perror("[ERROR] - socket()");
        exit(1);
    }

    // Updating address info
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // inet_addr(SRV_ADDR); 
    addr.sin_port = htons(port);

    // Binding socket
    if (bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("[ERROR] - bind()");
        exit(1);
    }

    // Listening for connection
    if (listen(sock_fd, 5)) {
        perror("[ERROR] - listen()");
        exit(1);
    }
    printf("Listening on port %d...\n", port);
    
    // Accepting connection
    int acpt_client = accept(sock_fd, NULL, NULL);
   
    // Copying file content to buffer
    while(!feof(f_handler)) {
        fread(buff, MAX_SIZE, 1, f_handler);
    }
    fclose(f_handler);

    send(acpt_client, buff, strlen(buff), 0);    // Transfering welcome text

    // Receiving file from client
    while (1)
    {
        int rcv_byte = recv(acpt_client, buff, MAX_SIZE, 0);
        if (rcv_byte <= 0) {
            printf("Connection closed.\n");
            break;    
        }
        if (rcv_byte < MAX_SIZE)
            buff[rcv_byte] = 0;

        printf("Saved: %s", buff);
    }
    
    // Closing connection
    close(acpt_client);
    close(sock_fd);
}