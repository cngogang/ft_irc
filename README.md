*This project has been created as part
of the 42 curriculum by cngogang, ehattab*

# Description:

IRC(Internet Relay Chat) server, build to be able to communicate with the IRC client HexChat in CPP 98.

The communication established between the server and the client lays on the TCP protocol and supports IPv6/IPv4.

This server can handle simultaneously and without multi-threading up to 3000 connections, thanks to the multyplexing tools Epoll.

A socket is created, binded to a port (given as argument) and then binded to a an epoll instance. 

This instance provides two lists of file descriptors : ready and interest.

When a connection request is accepted, the fd returned by the syscall accept is given to the interest list where it is monitored by the epoll instance.

When an event occurs on one of the fds from the interest list, the fd goes to the ready list which is given back by the epoll instance.

The flag used, when a fd is added to the interest list, define an event as the presence of bytes no readed in the fd. 

Time complexity wise, the Epoll instance enables to fetch a ready fd in O(1), where as the other multiplexing tools like select and poll does it in O(n).

Once readed, the bytes received are parsed according to the IRC protocol describe in the RFC 1459 - Internet Chat Relay Protocol.

The following command have been implemented:
MODE (+/- itkol), PART, JOIN, KICK, TOPIC, NAMES, INVITE, WHO.

# Instruction:

Create the executable with the command "make" in the project directory.

The executable will be run as follows:

./ircserv <port> <password>

• port: The port number on which your IRC server will be listening for incoming
IRC connections.
• password: The connection password. It will be needed by any IRC client that tries
to connect to your server.

# Ressouces

Internet Relay Chat Protocol - RFC 1459 (https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.2).
Programmation réseau via socket en C - (https://www.codequoi.com/programmation-reseau-via-socket-en-c/).
Cpp librairies and object - (https://cppreference.com).

The IA has been used to know return value of different syscall, meanings of errno values and some console error message.  


