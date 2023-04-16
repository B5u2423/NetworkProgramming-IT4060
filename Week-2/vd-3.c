#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char str[] = "227 Entering Passive Mode (213,229,112,130,216,4)"; // Input string
    char ip[30];        // IP address.
    int port[2];        // Port.

    // Parsing Socket info 
    char * addr_str = strchr(str, '(');
    printf("Address string: %s\n", addr_str);

    // First Octet
    char * tmp_str = strtok(addr_str, "(),");
    strcpy(ip, tmp_str);

    // The last 3 Octets
    int i = 0;
    while (i < 3) {
        tmp_str = strtok(NULL, "(),");
        strcat(ip, ".");            // Add '.' to separate octets.
        strcat(ip, tmp_str);
        ++i;
    }

    // Parsing and Converting.
    i = 0;
    while (i < 2) {
        tmp_str = strtok(NULL, "(),");
        port[i] = atoi(tmp_str);          // Convert 
        ++i;
    }

    // Final Result
    printf("Socket for FTP: %s:%u", ip, port[0] * 256 + port[1]);
}