#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0)
    {
        perror("connect");
        return 1;
    }

    std::string msg = "Hello Server";

    send(sock, msg.c_str(), msg.size(), 0); //non-blocking

    char buffer[1024];
    int n = recv(sock, buffer, sizeof(buffer), 0);

    std::cout << "Server replied: "
              << std::string(buffer, n) << "\n";

    sleep(5);
    close(sock);
}
