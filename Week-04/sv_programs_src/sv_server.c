#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// TESTING MACROS
#define PORT 9999
#define MAX_SIZE 2048

/** @function: VerifyPort()
* @brief: Checking port
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

/** @function: GetTime()
 * @brief: Get system time, format to YYYY-MM-DD Time
 * @param buff: A character array to save the formated time string.
 * 
 * @return: None
*/
void GetTime(char * buff)
{
    time_t ticks = time(NULL);
    printf("%.30s", ctime(&ticks));
    struct tm * t_info = localtime(&ticks);
    strftime(buff, 50, "%Y-%m-%d %X", t_info);  // Formatting
}

int main (int argc, char * argv[])
{
    FILE * f_handler;
    char buff[MAX_SIZE];

    // Verifying command
    if (argc != 3) {
        printf("[**ERROR]: Incorrect comamnd!\n");
        printf("[USAGE]: ./sv_server port logFile\n");
        exit(1);
    }

    // Verifying port
    int port = VerifyPort(argv[1]);
    if (port == -1) {
        printf("[**ERROR]: Invalid port!\n");
        exit(1);
    }

    // Initializing socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock_fd == -1) {
        perror("[**ERROR] - socket()");
        exit(1);
    }

    // Updating address info
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    // Binding socket
    if( bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) ) {
        perror("[**ERROR] - bind()");
        exit(1);
    }

    // Listening for connection
    if( listen(sock_fd, 5) ) {
        perror("[**ERROR] - listen()");
        exit(1);
    }
    printf("Listening on port %d...\n", port);

    // Accepting request
    struct sockaddr_in clientAddr;  // Save client Address
    socklen_t c_addrLen = sizeof(clientAddr);   

    int acpt_client = accept(sock_fd, (struct sockaddr *)&clientAddr, &c_addrLen);
    printf("Connection from: %s\n\n", inet_ntoa(clientAddr.sin_addr));

    // Receiving from client
    f_handler = fopen(argv[2], "a");
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
        
        // Save to file
        GetTime(sub_buff);
        fprintf(f_handler, "%s ", inet_ntoa(clientAddr.sin_addr));  // Add IP to file
        fprintf(f_handler, "%s ", sub_buff);                        // Add Date and Time
        fprintf(f_handler, "%s", buff);                             // Add Client's info
        printf("Received from Client: %s\n", buff);        
    }
    fprintf(f_handler, "%s", "===========\n");  // Separate sessions' logs
    fclose(f_handler);


    // Closing connection
    close(acpt_client);
    close(sock_fd);
}