NAME = webserv


CPP = c++

FLAGS = -Wall -Wextra -Werror --std=c++98

HEADERS = ./Headers/

SRCS = ./Sockets/

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME):

clean:

fclean:

re: