SRCS = main.cpp\
	AHost.cpp\
	Channel.cpp\
	Client.cpp\
	Server.cpp\
	Initialization.cpp\
	Invit_command.cpp\
	Join_command.cpp\
	Kick_command.cpp\
	Messaging.cpp\
	Messaging_2.cpp\
	Mode_command.cpp\
	Multyplexing.cpp\
	Part_command.cpp\
	Registration_command.cpp\
	Server_utils.cpp\
	Topic_command.cpp\



OBJS	:=	${addprefix obj/,${notdir $(SRCS:.cpp=.o)}}

# Compiler and flags
CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98

# Executable name
NAME = ircserv

# Object files (replace .cpp with .o)
# OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(NAME)

# Link step — create executable
$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

# Compilation rule for .cpp → .o
obj/%.o: %.cpp
	@mkdir -p obj
	$(CC) $(FLAGS) -c $< -o $@

# Clean object files
clean:
	rm -rf $(OBJS)

# Clean everything (objects + executable)
fclean: clean
	rm -f $(NAME)

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re
