#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX_SIZE 2048

int main(int argc, char * argv[])
{
    // Verify Input Arguments
    if (argc != 3) {
        printf("[ERROR]: Incorrect Command Format!\nUsage: ./tcp_client serverIP port\n");
        exit(100);
    }
    int port = atoi(argv[2]);
    if (port < 1 || port > 65535) {
        printf("[ERROR]: Invalid Port!\n");
        exit(1);
    }

    // Init Socket
    int s_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Socket Details 
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(port);

    // Connect to Server
    int srv_connect = connect(s_sock, (struct sockaddr *)&addr, sizeof(addr));
    if (srv_connect == -1) {
        printf("[ERROR]: connect() - %d - %s", errno, strerror(errno));
        exit(0);
    }

    // Send - Receive Data
    char buff[MAX_SIZE];
    int rcv_size = recv(s_sock, buff, MAX_SIZE, 0);
    if (rcv_size < 0) {
        printf("[ERROR]: recv() - %d - %s", errno, strerror(errno));
        exit(1);
    }
    if (rcv_size < MAX_SIZE) {
        buff[rcv_size] = 0;
    }

    // Close Socket
    close(s_sock);

    printf("%d\n", argc);
    for (int i = 1; i < argc; ++i) {
        printf("%s\n", argv[i]);
    }
    
    return  0;
}