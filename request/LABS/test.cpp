#include <iostream>
#include <dirent.h>

int listdir(std::string s)
{
    DIR *dir;
    struct dirent *ent;

    dir = opendir(s.c_str());
    if(!dir)
        return 0;
    while(ent = readdir(dir))
        std::cout << ent->d_name << std::endl;
    closedir(dir);
}

int main()
{
    listdir(".");
}