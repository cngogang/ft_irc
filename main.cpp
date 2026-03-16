


#include "Server.hpp"
#include "Client.hpp"


int main(int argc, char **argv)
{

    int number = 0;
    int i = 0;
        if (argc != 3)
            return (1);
    while(argv[1][i])
    {
        number = number * 10 + (argv[1][i] - 48);
        ++i;
    }

    std::string password(argv[2]);
    if (has_white_space(password))
    {
        std::cerr << "Wrong password format: White space." << std::endl;
        return (1);
    }
    Server IRC(number, password);
    return (0);

}
