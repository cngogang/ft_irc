


#include "Server.hpp"
#include "Client.hpp"


int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);


// std::string dns(argv[1]);
int number = 0;
int i = 0;
while(argv[1][i])
{
    number = number * 10 + (argv[1][i] - 48);
    ++i;
}

Server IRC(number);

return (0);

}
