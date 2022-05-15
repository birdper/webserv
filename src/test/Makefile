GREEN = \033[1;92m
YELLOW = \033[3;93m
BLUE = \033[3;34m
TEXT_RESET = \033[0;0m

NAME = unit_test

### COMPILE ###
CC = clang++
FLAGS = -Wall -Wextra -Werror --std=c++98
NOT_PATH = -not -path

### DIRECTORY/PATH ###
OBJ_DIR = ../../obj/test
#IGNORE_PATHS = $(NOT_PATH) "$(OBJ_DIR)/*"
# TODO IGNORE_PATHS unnecessary?
IGNORE_PATHS = ""
WEBSERVER_LIB_PATH = ../../webserv.a

### SEARCH PATHS ###
HPP_DIR = $(shell find ../main -type d )
HPP_DIR_LIB = $(shell find ../ -type d )
SRC_PATHS = $(shell find . -type d  )
CPP_FILES = $(shell find . -name "*.cpp"  -execdir echo {} ';')

### LINK ###
OBJ = $(CPP_FILES:%.cpp=%.o)
O_FILES = $(addprefix $(OBJ_DIR)/, $(OBJ))
INCLUDE = $(addprefix -I, $(HPP_DIR))
INCLUDE_LIB_HPP = $(addprefix -I, $(HPP_DIR_LIB))

vpath %.cpp $(SRC_PATHS)
vpath %.o $(OBJ_DIR)
vpath %.hpp $(INCLUDE)

.PHONY: all clean fclean re

all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJ)
	@echo "$(GREEN)START TESTING\n$(TEXT_RESET)"
	@for i in $(O_FILES); do \
		echo "$(GREEN)$$i $(TEXT_RESET)" ; \
		$(CC) $(FLAGS) $(INCLUDE) $(WEBSERVER_LIB_PATH) $$i -o $@ && ./$(NAME) ; \
	done


$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

%.o: %.cpp
	@$(CC) $(FLAGS) $(INCLUDE_LIB_HPP) -c $< -o $(OBJ_DIR)/$@

clean:
	@echo "$(TEXT_RESET)"
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)Delete 'o' files in '$(NAME)'$(TEXT_RESET)"

fclean: clean
	@echo "$(TEXT_RESET)"
	@rm -f $(NAME)
	@echo "$(YELLOW)Delete the binary file '$(NAME)'$(TEXT_RESET)"
	@echo

re: fclean all
