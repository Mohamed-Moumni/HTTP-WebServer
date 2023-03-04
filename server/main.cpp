#include "socket.hpp"

int main(int ac, char *av[])
{
    if (ac < 2)
    {
        std::cout << "Input Error: Usage <Host> <port> <port> ...." << std::endl;
        exit(EXIT_FAILURE);
    }
    std::vector<Socket> sockets;
    std::vector<pfd>    pfds;
    int                 connection;
    sockStorage         so_storage;
    pfd                 temp;

    for (int i = 2; i < ac; i++)
    {
        Socket sock(av[1], av[i]);
        sockets.push_back(sock);
    }
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
        temp.fd = sockets[i].getSocketId();
        temp.events = POLLIN | POLLOUT;
        pfds.push_back(temp);
    }
    socklen_t   socket_len;
    char        buff[3000];
    while (1)
    {
        poll(&pfds[0], pfds.size(), -1);
        for (unsigned int i = 0; i < pfds.size(); i++)
        {
            if (pfds[i].revents & POLLIN)
            {
                if (i < sockets.size() && pfds[i].fd == sockets[i].getSocketId())
                {
                    connection = accept(pfds[i].fd, (sockaddr *)&so_storage, &socket_len);
                    // sockets[i].connectionId.push_back(connection);
                    temp.fd = connection;
                    temp.events = (POLLIN | POLLOUT);
                    pfds.push_back(temp);
                }
                else
                {
                    std::string request = "";
                    memset(buff, 0, 3000);
                    recv(pfds[i].fd, buff, 3000, 0);
                    request += std::string(buff);
                    std::cout << request << "\n";
                    request.clear();
                }
            }
            if (pfds[i].revents & POLLOUT)
            {
                std::string response = "HTTP/1.1 200 OK\nContent-Length: 10\nContent-Type: text/plain\n\nHelloWorld";
                const char *buffer;

                memset(buff,0, 71);
                buffer = response.c_str();
                send(pfds[i].fd, buffer, 71, 0);
                response += std::string(buff);
                // std::cout << response << "\n";
                response.clear();
                // close(pfds[i].fd);
                // pfds.erase(pfds.begin()+i);
                // i--;
            }
            if (pfds[i].revents & (POLLERR | POLLHUP))
            {
                close(pfds[i].fd);
                pfds.erase(pfds.begin()+i);
                i--;
            }
            // send_response(pfds[i].fd, "HTTP/1.1 200 OK\nContent-Length: 1\nContent-Type: text/plain\n\na", sended);
        }
    }
    return (0);
}