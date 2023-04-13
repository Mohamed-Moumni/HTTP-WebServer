 #include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream> 

#define PORT 8081
std::string		read_file(std::string file_name)
{
	std::string	data;
	std::string	tmp;

	std::ifstream	file(file_name);
	while (getline(file, tmp))
	{
		data += tmp;
		data += "\r\n";
	}
	return data;
}

int main(void)
{
   a: int sock = 0; long valread;
    struct sockaddr_in serv_addr;
    std::string request = "DELETE /file.txt HTTP/1.1\r\nHost: localhost:8081\n\r\n\r" ;
    // std::cout << "request is: "<<request << std::endl;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        // printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , request.c_str() , strlen(request.c_str()) , 0);
    printf("request message sent\n");
        goto a;
    // read(sock , buffer, 1024);
    // std::cout << valread<< " Readed!" << std::endl;
    std::cout << "+++++++++++++++++++++++++++++++++\n";
    // printf("%s\n",buffer );
    std::cout << "\n+++++++++++++++++++++++++++++++++\n";
    sleep(1);
    return 0;
}

//  config
// server {
//         listen localhost:8080;
//         listen localhost:8080;

//         root /var/www/html;

//         index index.html index.htm index.nginx-debian.html;
// }

