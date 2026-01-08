##
## EPITECH PROJECT, 2025
## makefile
## File description:
## makefile
##

SRC	=	*.c


OBJ	=	$(SRC:.c=.o)


NAME	=	z


NAME_TEST	=	test_minishell


TEST	=	tests/*.c

all:	$(NAME)

$(NAME):	$(OBJ)
	epiclang -o $(NAME) $(SRC) -std=gnu17 -Wall -Wextra

tests_run:
	gcc -o $(NAME_TEST) $(TEST) $(SRC_T) -lcriterion -Wextra -g --coverage
	./$(NAME_TEST)

clean:
	rm -f $(OBJ)

clean_test:
	rm -f *no
	rm -f *da
	rm -f $(NAME_TEST)

fclean: clean
	rm -f $(OBJ)
	rm -f *~
	rm -f *#
	rm -f coding-style-reports.log
	rm -f $(NAME)

re:	fclean all