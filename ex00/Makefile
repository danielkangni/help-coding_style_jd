##
## EPITECH PROJECT, 2025
## makefile
## File description:
## makefile
##

SRC	=	*.cpp


OBJ	=	$(SRC:.c=.o)


NAME	=	Z


all:	$(NAME)

$(NAME):	$(OBJ)
	g++ -o $(NAME) $(SRC) -std=gnu17 -Wall -Wextra

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(OBJ)
	rm -f *~
	rm -f *#
	rm -f coding-style-reports.log
	rm -f $(NAME)