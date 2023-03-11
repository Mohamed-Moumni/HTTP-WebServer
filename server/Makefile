NAME = main

CPP = c++

FLAGS = -Wall -Wextra -Werror -fsanitize=address

SRCS = ./socket.cpp ../configfile/server.cpp ./ServerRoutine.cpp ./ConnectSocket.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME) 

$(NAME): $(OBJS) 
	$(CPP) $(FLAGS) $^ -o $(NAME)

%.o: %.cpp ConnectSocket.hpp socket.hpp
	$(CPP) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(NAME) $(OBJS)
