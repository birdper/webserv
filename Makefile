GREEN = \033[1;92m
YELLOW = \033[3;93m
BLUE = \033[3;34m
TEXT_RESET = \033[0;0m

NAME = webserv
NAME_LIB = webserv.a

### COMPILE ###
CC = clang++
#FLAGS = -Wall -Wextra -Werror --std=c++98
FLAGS = -g --std=c++98
NOT_PATH = -not -path

### DIRECTORY/PATH ###
TEST_DIR = ./src/test
OBJ_DIR = ./obj/main
GIT_DIR = ./.git
CMAKE_DIR = ./cmake-build-debug
IGNORE_PATHS = $(NOT_PATH) "$(GIT_DIR)/*" \
               $(NOT_PATH) "$(OBJ_DIR)/*" \
               $(NOT_PATH) "$(TEST_DIR)/*" \
               $(NOT_PATH) "$(CMAKE_DIR)/*"

### SEARCH FILES/PATHS ###
HPP_DIR = $(shell find ./src/main -type d $(IGNORE_PATHS))
SRC_PATHS = $(shell find ./src/main -type d $(IGNORE_PATHS))
CPP_FILES = $(shell find ./src/main -name "*.cpp" $(IGNORE_PATHS) -execdir echo {} ';')

### LINK ###
OBJ = $(CPP_FILES:%.cpp=%.o)
O_FILES = $(addprefix $(OBJ_DIR)/, $(OBJ))
INCLUDE = $(addprefix -I, $(HPP_DIR))

vpath %.cpp $(SRC_PATHS)
vpath %.o $(OBJ_DIR)
vpath %.hpp $(INCLUDE)

.PHONY: all clean fclean re

all: $(OBJ_DIR) $(NAME)

create_lib: $(O_FILES)
	@ar -rc $(NAME_LIB) $^
	@ranlib $(NAME_LIB)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(INCLUDE) $(O_FILES) -o $@
	@echo "$(GREEN)Success!$(TEXT_RESET)"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

%.o: %.cpp
	@echo "$(BLUE)Compiling $<"
	@$(CC) $(FLAGS) $(INCLUDE) -c $< -o $(OBJ_DIR)/$@

clean:
	@echo "$(TEXT_RESET)"
	@rm -rf $(OBJ_DIR)
	@make clean -C $(TEST_DIR)
	@echo "$(YELLOW)Delete 'o' files in '$(NAME)'$(TEXT_RESET)"

fclean: clean
	@echo "$(TEXT_RESET)"
	@rm -f $(NAME)
	@rm -f $(NAME_LIB)
	@make fclean -C $(TEST_DIR)
	@echo "$(YELLOW)Delete the binary file '$(NAME)'$(TEXT_RESET)"
	@echo

test: create_lib
	@rm -rf ./obj/test
	@make -C $(TEST_DIR)
	@echo "\n\n"

re: fclean all
