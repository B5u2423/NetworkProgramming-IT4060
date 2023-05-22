#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// TESTING MACROS
#define SRV_ADDR "127.0.0.1"
#define PORT 9999
#define MAX_SIZE 2048

/** @function: VerifyingAddress() 
* @brief: Checking IPv4 address
* @param cmd_address: Input argument for server IPv4 address
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
        for (unsigned int iNum = 0; iNum < oclen; ++iNum) {
            int tmp = sub_buff[iNum] - '0';     // Number only
            if (tmp < 0 || tmp > 9) {
                return 1;
            }
        }

        octet_str = strtok(NULL, ".");
    }
    
    return 0;
}

/** @function: VerifyingPort()
* @brief: Checking port
* @param cmd_port: Input argument for port
*
* @return: Port number if valid.
*          -1 if invalid       
**/
int VerifyPort (char * cmd_port)
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

/** @function: NameOperation()
 * @brief: Parsing and Checking the NAME substring.
 * @param buff: Buffer contains input from user command line.
 * 
 * @return: 0 - NAME is valid
 *          -1 - NAME is invalid.
*/
int NameOperation (char * buff) 
{
    // Parsing Name string
    int dq_cnt, dq_pos[2], iPos;
    dq_cnt = iPos = 0;
    char name[50];
    memset(name, 0, sizeof(name));  // Clear name array

    char * tmp = strchr(buff, '"');

    while (tmp != NULL) {
        ++dq_cnt;      // # of double quotes
        if (dq_cnt <= 2) {
            dq_pos[iPos] = tmp - buff;
            ++iPos;
        }
        tmp = strchr(tmp + 1, '"');
    }
    if (dq_cnt != 2) {  // Double quotes condition
        return -1;
    } else {
        strncpy(name, buff + dq_pos[0] + 1, dq_pos[1] - dq_pos[0] - 1); // Parsing NAME substring w/o double quotes
    }

    // Verify Name
    for(unsigned int iChar = 0; iChar < strlen(name); ++iChar) {
        if ( !isalpha(name[iChar]) && name[iChar] != ' ' ) {    // Characters and space only
            return -1;
        } 
    }

    return 0;
}

/** @funtion: VerifyStuID()
 * @brief: Checking STUDENT_ID 
 * @param cmd_id: Array contains STUDENT_ID string 
 *
 * @return: 0 - STUDENT_ID is valid
 *          -1 - STUDENT_ID is invalid.
*/
int VerifyStuID(char * cmd_id)
{
    unsigned int id_len = strlen(cmd_id);
    if (id_len != 8)
        return -1;
    for(unsigned int iDigit = 0; iDigit < id_len; ++iDigit) {
        if ( !isdigit(cmd_id[iDigit]) )     // Number only
            return -1; 
    }
    return 0;
}

/** @funtion: VerifyDoB()
 * @brief: Checking DATE_OF_BIRTH
 * @param cmd_id: Array contains DATE_OF_BIRTH string 
 *
 * @return: 0 - DATE_OF_BIRTH is valid
 *          -1 - DATE_OF_BIRTH is invalid.
*/
int VerifyDoB(char * cmd_DoB)
{
    char DoB[20];
    strcpy(DoB, cmd_DoB);

    time_t tick = time(NULL);
    struct tm t_info = *localtime(&tick);
    int cur_year = t_info.tm_year + 1900;   // Get Current Year

    char * token = strtok(DoB, "-");    // Parsing Y, M, D
    while (token != NULL) {
        char tmp_token[10];

        if (strlen(token) > 4)      // String length check (4 is max for YYYY)
            return -1;
        strcpy (tmp_token, token);
        for (unsigned int iChar = 0; iChar < strlen(tmp_token); ++iChar) {
            if ( !isdigit(tmp_token[iChar]) )   // Number only
                return -1;
        }  

        token = strtok(NULL, "-");       
    }

    int year, month, day;
    year = month = day = 0;
    sscanf(cmd_DoB, "%d %d %d", &year, &month, &day);
    // Failed conditions
    if (year > cur_year)            return -1;  // Year
    if (-month < 1 || -month > 12)  return -1;  // Month
    if (-day < 1 || -day > 31)      return -1;  // Day
    if (-month == 2 && -day > 29)   return -1;  // Exception February 

    return 0;
}

/** @funtion: VerifyGPA()
 * @brief: Checking GPA
 * @param cmd_id: Array contains GPA string 
 *
 * @return: 0 - GPA is valid
 *          -1 - GPA is invalid.
*/
int VerifyGPA(char * cmd_GPA)
{
    int dot_cnt = 0;    // # of decimal points
    for (unsigned int iDigit = 0; iDigit < strlen(cmd_GPA); ++iDigit) {
        if (dot_cnt > 1) 
            return -1;
        if (cmd_GPA[iDigit] == '.') {
            ++dot_cnt;
            continue;
        }
        if ( !isdigit(cmd_GPA[iDigit]) )    // Number only
            return -1;
    }

    float gpa = strtof(cmd_GPA, NULL);
    if (gpa > 4.0)  // GPA Limit
        return -1;        
    return 0;
}

int main(int argc, char * argv[])
{ 
    char buff[MAX_SIZE];

    // Verifying command
    if (argc != 3) {
        printf("[**ERROR]: Incorrect command!\n");
        printf("[USAGE]: ./sv_client server_IPv4 port\n");
        exit(1);
    }

    // Verifying Address
    if (VerifyAddress(argv[1])) {
        printf("[**ERROR]: Invalid Address.\n");
        exit(1);
    } 

    // Verifying Port
    int port = VerifyPort(argv[2]);
    if (port == -1) {
        printf("[**ERROR]: Invalid Port.\n");
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
    addr.sin_addr.s_addr = inet_addr(SRV_ADDR);
    addr.sin_port = htons(port);

    // Connecting to server
    int c_state = connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (c_state == -1) {
        perror("[**ERROR] - connect()");
        exit(1);
    }

    // Opening instruction if connect sucessfully
    FILE * f_handler;
    f_handler = fopen("sv_instruction.txt", "rb");
    char ch;

    while((ch = fgetc(f_handler)) != EOF) {
        putchar(ch);
    }
    putchar('\n');
    fclose(f_handler);
    

    // Get input from user
    char stu_id[15], dob[15], gpa[5], sub_buff[40], extra[20];
    while(1) {
        printf("[INFO]: ");
        fgets(buff, MAX_SIZE, stdin);

        if( !strncmp(buff, "exit", 4) || !strncmp(buff, "quit", 4)) {   // Disconnect condition
            printf("Diconnecting....\n");
            break;
        }

        if (NameOperation(buff)) {  // Checking name
            printf("[**ERROR]: Invalid Name Format.\n");
            continue;
        }

        char * tmp_string = strrchr(buff, '"');
        strcpy(sub_buff, tmp_string);

        // Parsing STUDENT_ID, DATE_OF_BIRTH, GPA substrings
        int param_cnt = sscanf(sub_buff + 1, "%s %s %s %s", stu_id, dob, gpa, extra);   // Extra to check # of arguments
        if (param_cnt != 3) {
            printf("[**ERROR]: Incorrect Input Format.\n");
            continue;            
        } 
        if (VerifyStuID(stu_id)) {
            printf("[**ERROR]: Invalid Student ID.\n");
            continue;
        }
        if (VerifyDoB(dob)) {
            printf("[**ERROR]: Invalid Date of Birth. [FORMAT]: YYYY-MM-DD\n");
            continue;
        }
        if (VerifyGPA(gpa)) {
            printf("[**ERROR]: Invalid GPA.\n");
            continue;
        }
        // Sending to server

        send(sock_fd, buff, strlen(buff), 0);
    }
    
    // Closing connetion
    close(sock_fd);
}