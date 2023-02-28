# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/24 08:55:26 by mkarim            #+#    #+#              #
#    Updated: 2023/02/28 08:14:30 by mkarim           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CC = c++

FLAGS = -Wall -Wextra -Werror --std=c++98 -g

#####		CONFIG FILES		#####
CONFIG_HEADERS = ./configfile/configfile.hpp ./configfile/server.hpp ./configfile/location.hpp 

CONFIG_SRCS = ./configfile/configfile.cpp ./configfile/server.cpp ./configfile/location.cpp ./configfile/parse.cpp ./configfile/print_servers.cpp ./configfile/check_errors.cpp
#####		CONFIG FILES		#####

#####		UTILS FILES			#####

UTILS_SRC = ./utils/str_trim.cpp ./utils/str_split.cpp

HEADERS = $(CONFIG_HEADERS)

SRCS = main.cpp $(CONFIG_SRCS) $(UTILS_SRC)

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