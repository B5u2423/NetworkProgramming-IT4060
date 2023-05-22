#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SRV_ADDR "127.0.0.1"
#define PORT 9999
#define MAX_SIZE 2048

int main() {
    // Khai báo socket client
    int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Khai báo địa chỉ server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(SRV_ADDR);
    addr.sin_port = htons(PORT); 

    // Thực hiện kết nối đến server
    int c_state = connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (c_state == -1) {
        perror("[**ERROR] - connect()");
        exit(1);
    }
    
    // Khai báo tập fdset
    fd_set fdread;
    char buf[MAX_SIZE];

    while (1)
    {
        // Khởi tạo lại tập fdread
        FD_ZERO(&fdread);

        // Gắn các descriptor vào tập fdread
        FD_SET(STDIN_FILENO, &fdread);
        FD_SET(sock_fd, &fdread);
        
        // Chờ đến khi sự kiện xảy ra
        int ret = select(sock_fd + 1, &fdread, NULL, NULL, NULL);
        if (ret == -1)
        {
            perror("select() failed");
            break;
        }
        
        // Kiểm tra sự kiện có dữ liệu từ bàn phím
        if (FD_ISSET(STDIN_FILENO, &fdread))
        {
            fgets(buf, sizeof(buf), stdin);
            send(sock_fd, buf, strlen(buf), 0);

            // Nếu nhập "exit" thì kết thúc
            if (strncmp(buf, "exit", 4) == 0)
                break;
        }

        // Kiểm tra sự kiện có dữ liệu truyền đến qua socket
        if (FD_ISSET(sock_fd, &fdread))
        {
            ret = recv(sock_fd, buf, sizeof(buf), 0);

            // Nếu ngắt kết nối thì kết thúc
            if (ret <= 0)
                break;
            
            buf[ret] = 0;
            printf("Received: %s\n", buf);
        }
    }

    // Kết thúc, đóng socket
    close(sock_fd);
}