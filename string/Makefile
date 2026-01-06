##
## EPITECH PROJECT, 2024
## makefile
## File description:
## makefile
##

NAME 	=	libstring.a

SRC	=	$(filter-out test_main.c,$(wildcard *.c))

OBJ	=	$(SRC:.c=.o)

all	:	$(OBJ)
		ar rcs $(NAME) $(OBJ)
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f libstring.a

re:     fclean all