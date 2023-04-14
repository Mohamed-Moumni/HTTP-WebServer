#include "../INCLUDES/request.hpp"
#include <fcntl.h>

static std::string  readFromPipe(int PipeId)
{
    int             readedChar;
    std::string     data;
    char            buffer[BUFFER] = {0};

    // readedChar = read(PipeId, buffer, BUFFER);
    while ((readedChar = read(PipeId, buffer, BUFFER)) > 0)
    {
        data.append(buffer, readedChar);
    }
    return (data);
}

void cgi_handler(ConnectSocket &socket, location location, ConfigFile configfile)
{
    int         pid;
    int         fds[2];
    std::string body;
    int         tmpfile;

    if (pipe(fds) < 0)
    {
        std::cout << "PIPE Error\n";
        return ;
    }
    tmpfile = open("/tmp/tempfd", O_CREAT | O_RDWR , 0644);
    write(tmpfile, socket._request.request_body.c_str(), socket._request.request_body.size());
    lseek(tmpfile, 0, SEEK_SET);
    pid = fork();
    if (pid == 0)
    {
        dup2(tmpfile, STDIN_FILENO);
        close(tmpfile);
        dup2(fds[1], STDOUT_FILENO);
        close(fds[0]);
        close(fds[1]);
    }
    // read data written from the pipe
    body = readFromPipe(fds[0]);
    close(fds[0]);
    close(fds[1]);
    unlink("/tmp/tempfd");
}