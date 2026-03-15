NAME        = myftp

CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -g

SRC         = src/main.c \
              src/serv.c \
              src/client.c \
              src/parse.c \
              src/cmd_auth.c \
              src/cmd_nav.c \
              src/cmd_transfer.c \
              src/cmd_dispatch.c \
              src/pasv_socket.c \
              src/data_conn.c \
              src/list_transfer.c

INCLUDES    = -I./include

all:
	$(CC) $(CFLAGS) -o $(NAME) $(SRC) $(INCLUDES)

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all
