#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SRV_ADDR "127.0.0.100"
#define MAX_SIZE 2048

int main(int argc, char * argv[])
{
    // Verifying Input Arguments
    if (argc == 4) 
    {
        printf("Creating Socket...\n");
    } else {
        printf("[ERROR]: Incorrect Command Format!\nUsage: ./tcp_server port sendfile savefile\n");
        exit(1);
    }

    int port = atoi(argv[1]);
    if (port < 1 || port > 65535) 
    {
        printf("[ERROR]: Invalid Port!\n");
        exit(1);
    }

    // Init Socket
    int c_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (c_sock == -1) {
        printf("Unable to Initiliaze Socket...\n");
        exit(1);
    }

    // Address Info
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(SRV_ADDR); //htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    // Binding and Listening for Connection
    if (bind(c_sock, (struct sockaddr *)&addr, sizeof(addr))) {
        printf("[ERROR]: bind() - %d - %s\n", errno, strerror(errno));
        exit(1);
    }

    if (listen(c_sock, 5)) {
        printf("[ERROR]: listen() - %d - %s\n", errno, strerror(errno));
        exit(1);
    } else {
        printf("Listening on port %d\n", port);
    }

    // Accepting connection
    // socklen_t addr_size;
    int acpt_client = accept(c_sock, NULL, NULL);
    if (acpt_client == -1) {
        printf("[ERROR]: accept() - %d - %s\n", errno, strerror(errno));
        exit(1);
    }

    // Sending Greeting
    FILE * f_handler;
    char buff[MAX_SIZE];
    int send_size;

    f_handler = fopen("tcp_greeting.txt", "r");
    if (f_handler == NULL) {
        printf("[ERROR]: Unavailable greeting file.\n");
        exit(1);
    }
    while(!feof(f_handler)) {
       send_size = fread(buff, 1, MAX_SIZE, f_handler);
       send(c_sock, buff, send_size, 0);
    }
    fclose(f_handler);

    // Communicating with client
    while(1)
    {
        // Receiving messages from client
        int rcv_size = recv(c_sock, buff, MAX_SIZE, 0);
        if (rcv_size <= 0) {
            printf("[ERROR]: recv() - %d - %s\n", errno, strerror(errno));
            break;
        }
        if (rcv_size < MAX_SIZE){
            buff[rcv_size] = 0; // Add ending character
        }

        // Append to log file.
        f_handler = fopen("tcp_msg.log", "a");
        fwrite(buff, rcv_size, 1, f_handler);
        fclose(f_handler);
    }
    
    // Closing connection
    close(acpt_client);
    close(c_sock);
}
