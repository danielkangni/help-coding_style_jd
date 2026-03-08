NAME        = nanotekspice

CXX         = g++

CXXFLAGS    = -Wall -Werror -std=c++20 -Iinclude

SRC_DIR     = src

INC_DIR     = include

TEST_NAME   = unit_tests

UNIT        = tests/test_input_component.cpp \
              tests/test_output_component.cpp \
              tests/test_true_component.cpp \
              tests/test_false_component.cpp

UNIT_SRC    = $(SRC_DIR)/circuit/components/input_component.cpp \
              $(SRC_DIR)/circuit/components/output_component.cpp \
              $(SRC_DIR)/circuit/components/true_component.cpp \
              $(SRC_DIR)/circuit/components/false_component.cpp


SRC         = $(SRC_DIR)/main.cpp \
              $(SRC_DIR)/parsing/to_token.cpp \
              $(SRC_DIR)/parsing/parser.cpp \
              $(SRC_DIR)/circuit/circuit.cpp \
              $(SRC_DIR)/circuit/components/input_component.cpp \
              $(SRC_DIR)/circuit/components/output_component.cpp \
              $(SRC_DIR)/circuit/components/clock_component.cpp \
              $(SRC_DIR)/circuit/components/true_component.cpp \
              $(SRC_DIR)/circuit/components/false_component.cpp \
              $(SRC_DIR)/circuit/components/elementary_gates.cpp \
              $(SRC_DIR)/circuit/components/quad_gates.cpp \
              $(SRC_DIR)/circuit/components/not_gate.cpp \
              $(SRC_DIR)/circuit/components/4008_adder.cpp \
              $(SRC_DIR)/circuit/components/4512_selector.cpp \
              $(SRC_DIR)/shell/shell.cpp

OBJ         = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

tests_run:
	clang++ -o $(TEST_NAME) $(UNIT) $(UNIT_SRC) -lcriterion -Iinclude --coverage
	./$(TEST_NAME)

clean:
	rm -f $(OBJ)
	rm -f $(SRC_DIR)/**/*.o $(SRC_DIR)/*.o
	rm -f $(TEST_NAME) *.gcda *.gcno

fclean: clean
	rm -f $(NAME)

re: fclean all

GUI_NAME    = nanotekspice-gui

GUI_SRC     = bonus/src/main_bonus.cpp \
              bonus/src/gui_app.cpp \
              bonus/src/circuit_schema.cpp \
              bonus/src/process.cpp \
              $(SRC_DIR)/parsing/to_token.cpp \
              $(SRC_DIR)/parsing/parser.cpp

bonus: $(NAME)
	$(CXX) $(CXXFLAGS) -Ibonus/include -o $(GUI_NAME) $(GUI_SRC) \
		-lsfml-graphics -lsfml-window -lsfml-system

push:
	git add .; git commit -m "$(C)"; git push
