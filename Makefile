# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/24 08:55:26 by mkarim            #+#    #+#              #
#    Updated: 2023/03/25 14:30:45 by mmoumni          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CC = c++

FLAGS = -Wall -Wextra -Werror --std=c++98 -g -fsanitize=address

#####		CONFIG FILES		#####
CONFIG_HEADERS = ./configfile/configfile.hpp ./configfile/server.hpp ./configfile/location.hpp 

CONFIG_SRCS = ./configfile/configfile.cpp ./configfile/server.cpp ./configfile/location.cpp \
			./configfile/parse.cpp ./configfile/print_servers.cpp ./configfile/check_errors.cpp \
			./configfile/preprocessing.cpp ./configfile/errors_handling.cpp
#####		CONFIG FILES		#####

#####		UTILS FILES			#####

####		SERVER FILES		#####

SERVER_SRCS = ./server/socket.cpp ./server/ConnectSocket.cpp

SERVER_HEADER = ./server/socket.hpp ./server/ConnectSocket.hpp

UTILS_SRC = ./utils/str_trim.cpp ./utils/str_split.cpp

HEADERS = $(CONFIG_HEADERS) $(SERVER_HEADER)

SRCS = main.cpp $(CONFIG_SRCS) $(UTILS_SRC) $(SERVER_SRCS)

OBJ = $(SRCS:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) $^ -o $(NAME)

%.o : %.cpp $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all