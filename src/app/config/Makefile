NAME = webserv

SRC_ROOT = $(wildcard ./*.cpp)
# SRC_PARSER = $(wildcard ./src/app/parser/*.cpp)
ALL_SRC = $(SRC_ROOT) #$(SRC_PARSER) 
OBJ=$(ALL_SRC:.cpp=.o)

CPPFLAGS = -std=c++98 #-fsanitize=address # -Wall -Wextra -Werror

RM = rm -rf

all: $(NAME)

$(NAME) : $(OBJ)
	c++ $(CPPFLAGS) $(OBJ) -o $@

fclean:
	$(RM) $(OBJ) $(NAME)

clean:
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re