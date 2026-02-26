NAME        = myftp

CXX         = gcc
CFLAGS      = -Wall -Wextra -Werror -g

SRC         = main.c commands.c client.c server.c transfer.c

OBJ         = $(SRC:.c=.o)

all:	$(OBJ)
	$(CXX) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
