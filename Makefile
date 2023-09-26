# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/24 08:55:26 by mkarim            #+#    #+#              #
#    Updated: 2023/09/26 09:05:09 by mmoumni          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CC = c++

FLAGS = --std=c++11

CONFIG_SRCS = ./src/configfile/configfile.cpp ./src/configfile/server.cpp ./src/configfile/location.cpp \
			./src/configfile/parse.cpp ./src/configfile/print_servers.cpp ./src/configfile/check_errors.cpp \
			./src/configfile/preprocessing.cpp ./src/configfile/errors_handling.cpp \


INCLUDES = ./includes/request.hpp ./includes/request.class.hpp ./includes/response.class.hpp ./includes/socket.hpp ./includes/ConnectSocket.hpp \
			./includes/configfile.hpp ./includes/server.hpp ./includes/location.hpp

SERVER_SRCS = ./src/server/socket.cpp ./src/server/ConnectSocket.cpp ./src/server/ConnectSocketUtils.cpp

REQUEST_SRCS = ./src/request/HTTP_METHODS/DELETE.cpp ./src/request/REQUEST_PARSING/possible_error.cpp \
				./src/request/HTTP_METHODS/GET.cpp ./src/request/REQUEST_PARSING/respond_error.cpp \
				./src/request/HTTP_METHODS/POST.cpp ./src/request/RESPONDING/respond.cpp ./src/request/RESPONDING/response_generator.cpp \
               ./src/request/REQUEST_PARSING/pars_request.cpp ./src/request/request.cpp ./src/request/RESPONDING/cgi_handler.cpp

UTILS_SRC = ./src/utils/str_trim.cpp ./src/utils/str_split.cpp ./src/utils/header_spliter.cpp ./src/utils/remove_side_spaces.cpp \
			./src/utils/side_whitespaces.cpp ./src/utils/str_split.cpp ./src/utils/str_trim.cpp

SRCS = main.cpp $(CONFIG_SRCS) $(UTILS_SRC) $(SERVER_SRCS) $(REQUEST_SRCS)

OBJ = $(SRCS:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) $^ -o $(NAME)

%.o : %.cpp $(INCLUDES)
	$(CC) $(FLAGS) -c $<  -o $@

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all