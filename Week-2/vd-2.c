#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main()
{
    char cmd[8];
    char * tmp;     // Additional parameter to check the correct format
    char buff[256];
    float param_1, param_2;

    fgets(buff, sizeof(buff), stdin);       // Pass input to buffer
    int count = sscanf(buff ,"%s %f %f %s", cmd, &param_1, &param_2, tmp);  // Parsing parameters

    /****
     * [USAGE]: "Operaion num1 num2"
     * 
     * - Correct parameter count is 3, return [ERROR] message otherwise.
     * - 4 commands, return [ERROR] message if input command matches with none. 
     * - Return [OK] message if input correctly.
    ****/ 
    
    if ( count != 3 ) {
        printf("[ERROR]: Incorrect Input Format. Please follow the format \"Command num1 num2\".\n");
        return 100;
    } else if ( strcmp(cmd, "ADD") && strcmp(cmd, "SUB") &&
                strcmp(cmd, "MUL") && strcmp(cmd, "DIV") ) {                    
        printf("[ERROR]: Command Doesn't Exist. Please check your typo.\n");  
        return 200;
    } else {
        printf("[OK]: Correct Input\n");
        return 0;
    }
}