SRC =	main.cpp\
		Connection.cpp\
		Addrinfo.cpp\
		FSocket.cpp

OBJ = $(SRC:%.cpp=%.o)

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

NAME = server

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) -c $^ -o $@ $(CFLAGS)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean
	make all

.PHONY : all clean fclean re