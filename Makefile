# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/24 08:55:26 by mkarim            #+#    #+#              #
#    Updated: 2023/04/10 15:57:12 by mmoumni          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CC = c++

# FLAGS = -Wall -Wextra -Werror --std=c++98 -g -fsanitize=address

#####		CONFIG FILES		#####

CONFIG_SRCS = ./configfile/configfile.cpp ./configfile/server.cpp ./configfile/location.cpp \
			./configfile/parse.cpp ./configfile/print_servers.cpp ./configfile/check_errors.cpp \
			./configfile/preprocessing.cpp ./configfile/errors_handling.cpp \

#####		CONFIG FILES		#####

#####		UTILS FILES			#####

####		SERVER FILES		#####

SERVER_SRCS = ./server/socket.cpp ./server/ConnectSocket.cpp ./server/ConnectSocketUtils.cpp

REQUEST_SRCS = ./request/HTTP_METHODS/DELETE.cpp ./request/REQUEST_PARSING/possible_error.cpp \
				./request/HTTP_METHODS/GET.cpp ./request/REQUEST_PARSING/respond_error.cpp \
				./request/HTTP_METHODS/POST.cpp ./request/RESPONDING/respond.cpp ./request/RESPONDING/response_generator.cpp \
               ./request/REQUEST_PARSING/pars_request.cpp ./request/request.cpp ./request/RESPONDING/cgi_handler.cpp

UTILS_SRC = ./utils/str_trim.cpp ./utils/str_split.cpp ./utils/header_spliter.cpp ./utils/remove_side_spaces.cpp \
			./utils/side_whitespaces.cpp ./utils/str_split.cpp ./utils/str_trim.cpp


SERVER_HEADER = ./server/socket.hpp ./server/ConnectSocket.hpp

REQUEST_HEADER = ./request/INCLUDES/request.hpp ./request/INCLUDES/request.class.hpp ./request/INCLUDES/response.class.hpp 

CONFIG_HEADERS = ./configfile/configfile.hpp ./configfile/server.hpp ./configfile/location.hpp 

HEADERS = $(CONFIG_HEADERS) $(SERVER_HEADER) $(REQUEST_HEADER)

SRCS = main.cpp $(CONFIG_SRCS) $(UTILS_SRC) $(SERVER_SRCS) $(REQUEST_SRCS)

OBJ = $(SRCS:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) $^ -o $(NAME)

%.o : %.cpp $(HEADERS)
	$(CC) $(FLAGS) -c $<  -o $@

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all