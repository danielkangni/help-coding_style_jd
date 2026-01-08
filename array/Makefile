##
## EPITECH PROJECT, 2024
## makefile
## File description:
## makefile
##

NAME 	=	libarray.a

SRC	=	$(wildcard *.c)

OBJ	=	$(SRC:.c=.o)

all	:	$(OBJ)
		ar rcs $(NAME) $(OBJ)
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f libarray.a

re:     fclean all