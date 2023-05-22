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
#define MAX_CLIENTS FD_SETSIZE

// Xóa client ra khỏi mảng
void removeClient(int *clients, int *numClients, int clientIndex) 
{
    if (clientIndex < *numClients - 1) 
        clients[clientIndex] = clients[*numClients - 1];
    *numClients = *numClients - 1;
}

int main()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(sock_fd, 5);

    fd_set fdread;
    
    // Mảng clients lưu các socket đã được chấp nhận
    // Sử dụng trong việc thăm dò sự kiện
    int clients[MAX_CLIENTS];
    int numClients = 0;

    char buf[MAX_SIZE];

    while (1)
    {
        // Khởi tạo lại tập fdread
        FD_ZERO(&fdread);

        // Gắn các socket listener và clients vào tập fdread
        // maxdp lưu giá trị descriptor lớn nhất 
        FD_SET(sock_fd, &fdread);
        int maxdp = sock_fd;
        for (int i = 0; i < numClients; i++)
        {
            FD_SET(clients[i], &fdread);
            if (clients[i] > maxdp)
                maxdp = clients[i];
        }

        // Chờ đến khi sự kiện xảy ra hoặc hết giờ
        int ret = select(maxdp + 1, &fdread, NULL, NULL, NULL);
        if (ret < 0) 
        {
            printf("select() failed.\n");
            return 1;
        }

        // Thăm dò sự kiện có yêu cầu kết nối
        if (FD_ISSET(sock_fd, &fdread)) 
        {
            int client = accept(sock_fd, NULL, NULL);

            if (numClients < MAX_CLIENTS)
            {
                printf("New client connected %d\n", client);
                // Lưu vào mảng để thăm dò sự kiện
                clients[numClients] = client;
                numClients++;
            }
            else
            {
                // Đã vượt quá số kết nối tối đa
                close(client);
            }
        }
        
        // Thăm dò sự kiện có dữ liệu truyền đến các socket client
        for (int i = 0; i < numClients; i++)
            if (FD_ISSET(clients[i], &fdread)) 
            {
                ret = recv(clients[i], buf, sizeof(buf), 0);
                if (ret <= 0)
                {
                    printf("Client %d disconnected\n", clients[i]);
                    // Xóa client khỏi mảng
                    removeClient(clients, &numClients, i);
                    i--;
                    continue;
                }
                buf[ret] = 0;
                printf("Received data from client %d: %s\n", clients[i], buf);
            }
    }
}