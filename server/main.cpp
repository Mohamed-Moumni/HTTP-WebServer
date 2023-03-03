#include "socket.hpp"

int main(int ac, char *av[])
{
    if (ac < 2)
    {
        std::cout << "Input Error: Usage <Host> <port> <port> ...." << std::endl;
        exit(EXIT_FAILURE);
    }
    std::vector<Socket> sockets;
    std::vector<pfd>    pfds(ac - 2);
    int                 connection;
    sockStorage         so_storage;
    pfd                 temp;

    for (int i = 2; i < ac; i++)
        sockets.push_back(Socket(av[1],av[i]));
    for (unsigned int i = 0; i < sockets.size(); i++)
    {
        if (listen(sockets[i].getSocketId(), SOMAXCONN) < 0)
        {
            std::cout << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    for (unsigned int i = 0; i < sockets.size() ; i++)
    {
        pfds[i].fd = sockets[i].getSocketId();
        pfds[i].events = POLLIN | POLLOUT;
    }
    socklen_t socket_len;
    while (1)
    {
        poll(&pfds[0], pfds.size(), -1);
        for (unsigned int i = 0; i < pfds.size(); i++)
        {
            if (pfds[i].revents & POLLIN)
            {
                if (pfds[i].fd == sockets[i].getSocketId())
                {
                    connection = accept(pfds[i].fd, (sockaddr *)&so_storage, &socket_len);
                    sockets[i].connectionId.push_back(connection);
                    temp.fd = connection;
                    temp.events = POLLIN;
                    pfds.push_back(temp);
                }
                else
                {
                    std::cout << recv_request(pfds[i].fd) << std::endl;
                }
            }
            if (pfds[i].revents & POLLOUT)
            {
                send_response(pfds[i].fd, "HTTP/1.1 200 OK\nContent-Length: 1\nContent-Type: text/plain\n\na");
            }
        }
    }
    return (0);
}