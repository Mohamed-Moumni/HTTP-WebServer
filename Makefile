# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/24 08:55:26 by mkarim            #+#    #+#              #
#    Updated: 2023/02/25 16:09:19 by mmoumni          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CPP = c++

FLAGS = -Wall -Wextra -Werror --std=c++98

#####		CONFIG FILES		#####
CONFIG_HEADERS = configfile.hpp server.hpp location.hpp 

CONFIG_SRCS = configfile.cpp server.cpp location.cpp parse_conf_file.cpp
#####		CONFIG FILES		####

HEADERS = $(CONFIG_HEADERS)

SRCS = main.cpp $(CONFIG_SRCS)

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