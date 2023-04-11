#include <stdio.h>
#include <string.h>

/* Input format: tcp_client <IP_addr> <port_number>
*/

int main(int argc, char * argv[])
{
    if (argc > 3) {
        printf("[ERROR]: Incorrect Input Format!\n");
        return 100;
    }

    printf("%d\n", argc);
    for (int i = 1; i < argc; ++i) {
        printf("%s\n", argv[i]);
    }
    
    return  0;
}