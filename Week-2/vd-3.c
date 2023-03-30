#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char str[] = "227 Entering Passive Mode (213,229,112,130,216,4)"; // Input string
    char ip[30];        // String for ip address.
    int port[2];        // Variable for ports.

    // Parsing the Socket info string.
    char * addr_str = strchr(str, '(');
    printf("Address string: %s\n", addr_str);

    // First Octet
    char * p = strtok(addr_str, "(),");
    strcpy(ip, p);

    // Loop for the last 3 Octet
    int i = 0;
    while (i < 3) {
        p = strtok(NULL, "(),");
        strcat(ip, ".");            // Add '.' to separate octets.
        strcat(ip, p);
        ++i;
    }

    // Parsing and Convert string -> integer for port number.
    i = 0;
    while (i < 2) {
        p = strtok(NULL, "(),");
        port[i] = atoi(p);          // Convert 
        ++i;
    }

    // Final Result
    printf("Socket for FTP: %s:%u", ip, port[0] * 256 + port[1]);
}