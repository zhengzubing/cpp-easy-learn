#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

void setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    // 创建监听套接字
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // 设置地址重用
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 绑定地址
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_port = htons(8080);
    if (bind(listen_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(listen_fd);
        return 1;
    }

    // 监听连接
    if (listen(listen_fd, SOMAXCONN) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        close(listen_fd);
        return 1;
    }

    // 创建epoll实例
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Failed to create epoll instance" << std::endl;
        close(listen_fd);
        return 1;
    }

    // 将监听套接字添加到epoll实例中
    epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = listen_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) == -1) {
        std::cerr << "Failed to add listen socket to epoll" << std::endl;
        close(listen_fd);
        close(epoll_fd);
        return 1;
    }

    std::cout << "Server started, listening on port 8080..." << std::endl
              << std::endl;

    while (true) {
        // 等待事件发生
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            std::cerr << "epoll_wait failed" << std::endl;
            continue;
        } else {
            std::cout << "epoll_wait success" << std::endl;
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == listen_fd) {
                // 处理新连接
                sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int conn_fd = accept(listen_fd, (sockaddr*)&client_addr,
                                     &client_addr_len);
                if (conn_fd == -1) {
                    std::cerr << "Failed to accept connection" << std::endl;
                    continue;
                }

                setNonBlocking(conn_fd);

                // 将新连接的套接字添加到epoll实例中
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev) == -1) {
                    std::cerr << "Failed to add connection socket to epoll"
                              << std::endl;
                    close(conn_fd);
                }

                std::cout << "New connection from "
                          << inet_ntoa(client_addr.sin_addr) << ":"
                          << ntohs(client_addr.sin_port) << std::endl;
            } else {
                // 处理客户端数据
                std::string buffer;
                char temp[BUFFER_SIZE];
                ssize_t n = read(events[i].data.fd, temp, BUFFER_SIZE);
                if (n <= 0) {
                    // 客户端关闭连接
                    std::cout << "Connection closed by client" << std::endl;
                    close(events[i].data.fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd,
                              nullptr);
                } else {
                    std::cout << "Received: " << temp << " -- from client"
                              << std::endl;
                    buffer.assign(temp, n);
                    // 给buffer添加内容
                    const std::string append_str = " -- from server";
                    buffer += append_str;

                    // 回显数据给客户端
                    write(events[i].data.fd, buffer.c_str(), buffer.size());
                }
            }
            std::cout << std::endl;
        }
    }

    // 关闭监听套接字和epoll实例
    close(listen_fd);
    close(epoll_fd);

    return 0;
}