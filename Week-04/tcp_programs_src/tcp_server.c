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

/** @function: VerifyPort() 
* @brief:  Checking port 
* @param cmd_port: Input argument for port
*
* @return: -1 if invalid
*          Port number if valid.
**/
int VerifyPort (const char * cmd_port)
{
    char buff[10];
    strcpy(buff, cmd_port);
    for(unsigned int iBuff = 0; iBuff < strlen(buff); ++iBuff) {
        int tmp = buff[iBuff] - '0';
        if (tmp < 0 || tmp > 9) {       // Number only
            return -1;
        }
    }
    int port = atoi(cmd_port);
    if(port < 1 || port > 65535) {      // Port range
        return -1;
    }

    return port;
}

int main(int argc, char * argv[])
{
    FILE * f_handler;
    char buff[MAX_SIZE];

    // Verifying command 
    if (argc != 4) {
        printf("[**ERROR]: Incorrect command.\n");
        printf("[USAGE]: ./tcp_server port greetFile logFile\n");
        exit(1);
    }

    // Verifying port number
    int port = VerifyPort(argv[1]);
    if (port == -1) {
        printf("[**ERROR]: Invalid port!\n");
        exit(1);
    }
        
    // Verifying welcome file
    f_handler = fopen(argv[2], "r");        
    if(f_handler == NULL) {
        printf("[**ERROR]: Greeting file doesn't exist!\n");
        exit(1);
    }    

    while(!feof(f_handler)) {       // Copying file content to buffer
        fread(buff, MAX_SIZE, 1, f_handler);
    }
    fclose(f_handler);

    // Verifying log file
    f_handler = fopen(argv[3], "r");
        // Giving error if log file doesn't exist
    if(f_handler == NULL) {
        printf("[**ERROR]: Log file doesn't exist!\n");
        exit(1);
    }
    fclose(f_handler);

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
    addr.sin_port = htons(port);

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
    printf("Listening on port %d...\n", port);
    
    // Accepting connection
    int acpt_client = accept(sock_fd, NULL, NULL);
   
    send(acpt_client, buff, strlen(buff), 0);    // Transfering welcome text
    memset(buff, 0, MAX_SIZE);

    // Receiving file from client
    f_handler = fopen(argv[3], "a");
    while (1)
    {
        int rcv_byte = recv(acpt_client, buff, MAX_SIZE, 0);
        if (rcv_byte <= 0) {
            printf("Connection closed.\n");
            break;    
        }
        if (rcv_byte < MAX_SIZE)    // Adding end of line character
            buff[rcv_byte] = 0;

        fprintf(f_handler, "- %s", buff);
        printf("Saved: %s", buff);
    }
    fprintf(f_handler, "%s", "===========\n");
    fclose(f_handler);

    // Closing connection
    close(acpt_client);
    close(sock_fd);
}