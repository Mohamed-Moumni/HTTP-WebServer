NAME = NGINY	
CC = c++

# FLAGS = -Wall -Wextra -Werror -std=c++98
FLAGS = -std=c++98 

HEADERS = INCLUDES/request.class.hpp  INCLUDES/request.hpp        INCLUDES/response.class.hpp

SOURCES =  request.cpp \
../configfile/check_errors.cpp    ../configfile/location.cpp        ../configfile/print_servers.cpp   ../utils/header_spliter.cpp       ../utils/str_split.cpp            HTTP_METHODS/GET.cpp              REQUEST_PARSING/respond_error.cpp \
../configfile/configfile.cpp      ../configfile/parse.cpp           ../configfile/server.cpp          ../utils/remove_side_spaces.cpp   ../utils/str_trim.cpp             HTTP_METHODS/POST.cpp             RESPONDING/respond.cpp REQUEST_PARSING/possible_error.cpp\
../configfile/errors_handling.cpp ../configfile/preprocessing.cpp   ../server/ConnectSocket.cpp       ../utils/side_whitespaces.cpp     HTTP_METHODS/DELETE.cpp           REQUEST_PARSING/pars_request.cpp  RESPONDING/response_generator.cpp \


OBJECTS = $(SOURCES:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJECTS) $(HEADERS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(NAME)

%.o: %.cpp $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

go: all 
	./NGINY