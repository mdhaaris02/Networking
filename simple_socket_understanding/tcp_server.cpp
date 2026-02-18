#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET -> ipv4, SOCK_STREAM -> tcp, 0 -> auto protocol, socket create a end point for process(we should make explicitly non-blocking)
    if (server_fd < 0)
    {
        perror("socket");
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);      // port
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) /*Bind Helps kernel to know which port it wants to receive(means assigning address)(non-blocking)
                                                             Typecasting to (sockaddr) it is the generic type used by the core socket API functions (like bind(), connect(), and accept()) to maintain a protocol-independent interface. */
    {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 5) < 0)/*non-blocking, kernel change socket state + tell kernel to start accepting connections, Here '5' means max clients waiting to be accepted It does not mean max 5 clients can be connected.
                                After listen we are telling kernel to start accepting the connections by client, the kernel creates two queues(1.SYN queue and 2.accept queus)
                                accept queue contains the clients who completed handshake, SYN queue contains half-open handshakes.*/
    {
        perror("listen");
        return 1;
    }

    std::cout << "Server listening on port 9000...\n";

    while (true)
    {
        int client_fd = accept(server_fd, nullptr, nullptr); /*accept blocks until a fully established connection(means a complete handshake) is available in the accept queue.*/
        if (client_fd < 0)
        {
            perror("accept");
            continue;
        }

        std::cout << "Client connected\n";

        char buffer[1024] = {};

        while (true)
        {
            int n = recv(client_fd, buffer, sizeof(buffer), 0); /*recv blocks until data receives or the client side communication is closed, kernel puts this to sleep so cpu usage = 0. 
                                                                  if n>0 -> data received, n==0 -> connection closed, n<0 -> error*/

            std::cout << "server class: " << buffer << std::endl;

            if (n <= 0)
                break;

            send(client_fd, buffer, n, 0); // echo
        }

        close(client_fd);
        std::cout << "Client disconnected\n";
    }

    close(server_fd);
}
