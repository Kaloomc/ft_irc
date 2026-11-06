NAME = ft_irc

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

OBJ_DIR = obj

SRC = main.cpp Server.cpp Client.cpp

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

all: $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@echo "Compilation de $(NAME)..."
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
