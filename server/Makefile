NAME = main

CPP = c++

FLAGS = -Wall -Wextra -Werror 

SRCS = ./socket.cpp ../configfile/server.cpp ./main.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(FLAGS) $^ -o $(NAME)

%.o: %.cpp
	$(CPP) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(NAME) $(OBJS)