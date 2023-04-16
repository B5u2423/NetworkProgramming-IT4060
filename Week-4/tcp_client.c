#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_SIZE 2048

/** @function: VerifyAddress()
* @brief: Checking IPv4 address
* @param cmd_address: Command line argument for server IPv4 address
*
* @return: 0 if valid
*          1 if invalid.
**/
int VerifyAddress (const char * cmd_address) 
{
    char buff[32], sub_buff[8];
    char * octet_str;
    
    strcpy(buff, cmd_address);
    octet_str = strtok(buff, ".");        // Parse octets

    while (octet_str != NULL) {
        strcpy(sub_buff, octet_str);

        unsigned int oclen = strlen(octet_str);       // Check octet string length
        if(oclen > 3) {
            return 1;
        }
        for (unsigned int iDigit = 0; iDigit < oclen; ++iDigit) {
            int tmp = sub_buff[iDigit] - '0';     // Number only
            if (tmp < 0 || tmp > 9) {
                return 1;
            }
        }

        octet_str = strtok(NULL, ".");
    }
    
    return 0;
}

/** @function: VerifyPort()
* @brief: Checking port
* @param cmd_port: Command line argument for port
*
* @return: Port number if valid.
*          -1 if invalid       
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
    char buff[MAX_SIZE];

    // Verifying command
    if (argc != 3) {
        printf("[**ERROR]: Incorrect command\n");
        printf("[USAGE]: ./tcp_client server_IPv4 port");
        exit(1);
    }

    // Verifying address
    if (VerifyAddress(argv[1])) {
        printf("[**ERROR]: Invalid IPv4 Address\n");
        exit(1);
    }

    // Verifying port
    int port = VerifyPort(argv[2]);
    if (port == -1) {
        printf("[**ERROR]: Invalid port!\n");
        exit(1);
    }

    // Initializing socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Updating address info
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    // inet_aton(argv[1], &addr.sin_addr);
    addr.sin_port = htons(port);

    // Connecting to the server
    int c_state = connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (c_state == -1) {
        perror("[**ERROR] - connect()");
        exit(1);
    }

    // Receiving welcome text from server
    int rcv_byte = recv(sock_fd, buff, MAX_SIZE, 0);
    if (rcv_byte <= 0) {
        printf("Connection closed.\n");
        exit(1);
    }

    if (rcv_byte < MAX_SIZE)    // Add NULL to end 
        buff[rcv_byte] = 0;

    printf("%s\n", buff);

    while (1)
    {
        printf("[MESSAGE]: ");
        fgets(buff, MAX_SIZE, stdin);               // Get user's input
   
        if (!strncmp(buff, "exit", 4) || !strncmp(buff, "quit", 4)) {         // Stop condition
            printf("Disconnecting....\n");
            break;
        }      

        send(sock_fd, buff, strlen(buff), 0);
    }

    // Closing connection
    close(sock_fd);
}