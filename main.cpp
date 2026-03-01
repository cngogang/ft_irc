


#include "Server.hpp"
#include "Client.hpp"


int main(int argc, char **argv)
{
    if (argc != 3)
        return (1);


// std::string dns(argv[1]);
int number = 0;
int i = 0;
while(argv[1][i])
{
    number = number * 10 + (argv[1][i] - 48);
    ++i;
}

std::string password(argv[2]);
Server IRC(number, password);

return (0);

}
