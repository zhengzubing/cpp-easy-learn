#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#define BUFFER_SIZE 1024

int main() {
    // 创建套接字
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // 设置服务器地址
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(sock_fd);
        return 1;
    }

    // 连接到服务器
    if (connect(sock_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Connection failed" << std::endl;
        close(sock_fd);
        return 1;
    }

    std::cout << "Connected to server" << std::endl;

    // 发送数据
    const char* message = "Hello, server!";
    ssize_t sent = send(sock_fd, message, strlen(message), 0);
    if (sent == -1) {
        std::cerr << "Failed to send data" << std::endl;
        close(sock_fd);
        return 1;
    }

    std::cout << "Sent: " << message << std::endl;

    // 接收服务器回显
    char buffer[BUFFER_SIZE];
    ssize_t received = recv(sock_fd, buffer, BUFFER_SIZE, 0);
    if (received == -1) {
        std::cerr << "Failed to receive data" << std::endl;
    } else if (received > 0) {
        buffer[received] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }

    // 关闭套接字
    close(sock_fd);

    return 0;
}
